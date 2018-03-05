# item 结构与 LRU 链表基本操作

[items.c](https://github.com/steveLauwh/Memcached/blob/master/memcached-1.5.4/items.c) - LRU 链表基本操作

从 slab 内存分配器获取存储 item 地址，然后交给 LRU 链表统一管理。

## item 结构的组成

```c
/**
 * Structure for storing items within memcached.
 */
// item 实际存储数据的结构体
typedef struct _stritem {
    /* Protected by LRU locks */
    struct _stritem *next;       // 双向链表
    struct _stritem *prev;
    /* Rest are protected by an item lock */
    struct _stritem *h_next;    /* hash chain next */      // hash 表的冲突链
    rel_time_t      time;       /* least recent access */       // 最后一次访问时间
    rel_time_t      exptime;    /* expire time */   // 过期失效时间
    int             nbytes;     /* size of data */     // 该item 存放的数据长度
    unsigned short  refcount;  	// 引用次数
    uint8_t         nsuffix;    /* length of flags-and-length string */ // 后缀长度 
    uint8_t         it_flags;   /* ITEM_* above */  // item 的属性，标志
    uint8_t         slabs_clsid;/* which slab class we're in */ // 该 item 属于那个slab class
    uint8_t         nkey;       /* key length, w/terminating null and padding */  // 键值的长度
    /* this odd type prevents type-punning issues when we do
     * the little shuffle to save space when not using CAS. */
    //真实的数据信息
    union {
        uint64_t cas;
        char end;
    } data[];   
    /* if it_flags & ITEM_CAS we have 8 bytes CAS */
    /* then null-terminated key */
    /* then " flags length\r\n" (no terminating null) */
    /* then data with terminating \r\n (no terminating null; it's binary!) */
} item;
```

item 结构体中有个 it_flags 参数，代表 item 的标志：

```c
#define ITEM_LINKED 1  // 该 item 已经插入到 LRU 链表  
#define ITEM_CAS 2     // 该 item 使用 CAS  
#define ITEM_SLABBED 4 // 该 item 还在 slab 的空闲链表里面，没有分配出去  
#define ITEM_FETCHED 8 // 该 item 插入到 LRU 链表后，被 worker 线程访问过  
```

item 结构的组成：item header 大小 + 存储数据大小

```c
// item header 大小: item 自身结构体 + key 长度 + CAS 可选 + suffix
#define ITEM_ntotal(item) (sizeof(struct _stritem) + (item)->nkey + 1 \
         + (item)->nsuffix + (item)->nbytes \
         + (((item)->it_flags & ITEM_CAS) ? sizeof(uint64_t) : 0))
```
![](https://github.com/steveLauwh/Memcached/raw/master/The%20Annotated%20Memcached%20Sources/image/item.PNG)

## LRU 链表的基本操作

LRU 链表是一个双向链表，由 item 元素组成。

LRU 链表个数取决于 item 种类，意思是同一类型的 item 会插入到同一个 LRU 链表。

```c
// 重要: LRU 链表使用头插法，指针数组
static item *heads[LARGEST_ID];  // 指向每一个 LRU 链表的头部
static item *tails[LARGEST_ID];  // 指向每一个 LRU 链表的尾部
```

Memcached 设计两个指针数组：数组的索引 对应于 slabs_clsid

* heads[i] 指向第 i 类 LRU 链表的第一个 item
* tails[i] 指向第 i 类 LRU 链表的最后一个item

![](https://github.com/steveLauwh/Memcached/blob/master/The%20Annotated%20Memcached%20Sources/image/LRU%20Linked%20List.PNG)

### 为什么 Memcached 设计两个指针数组，分别存储各个 LRU 链表的头部和尾部地址?

item 种类不同，LRU 链表也不同，设计两个指针数组，便于管理 LRU 链表。

### 如何将 item 插入到 LRU 链表？

do_item_link_q (内部函数)：根据 item 所属 LRU 链表，在对应的 LRU 链表上头部插入该 item。

do_item_link (外部调用)：将 item 插入哈希表和 LRU 链表。

### 如何将 item 从 LRU 链表删除？

do_item_unlink_q (内部函数)：简单的双向链表删除节点操作。

do_item_unlink (提供外部调用)：将 item 从哈希表和 LRU 链表中删除，删除的 item 会判断是否归还到 slab 内存分配器。

### item 更新机制？

当 item 的 time 与当前时间相隔大于一分钟，才更新 item。

怎么更新，先将 item 从 LRU 链表删除，然后再插入。

do_item_update (提供外部调用)：更新 item 操作。
