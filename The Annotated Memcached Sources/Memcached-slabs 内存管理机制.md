# Memcached-slabs 内存管理机制

[slabs.c 注解](https://github.com/steveLauwh/Memcached/tree/master/memcached-1.5.4/slabs.c)

Memcached 声明一个 slabclass_t 结构体，如下：

```c
/* slab class(内存池)内存分配结构体*/
// slab class 包含若干个相同大小的slab 分配器
typedef struct {
    //当前slab class 分配一个item 的大小     
    unsigned int size;      /* sizes of items */

    // 每一个slab 分配器能分配多少个item
    unsigned int perslab;   /* how many items per slab */

    // 指向空闲item 的指针
    void *slots;           /* list of item ptrs */  

    // 总共多少个空闲的item
    unsigned int sl_curr;   /* total free items in list */

    // slab class 的可用slab 分配器个数
    unsigned int slabs;     /* how many slabs were allocated for this class */ 

    //slab 数组，数组的每一个元素就是一个slab分配器，这些分配器都分配相同尺寸的内存  
    void **slab_list;       /* array of slab pointers */

    // slab 数组的大小
    unsigned int list_size; /* size of prev array */

    // 总共请求的bytes
    size_t requested; /* The number of requested bytes */
} slabclass_t;

//定义一个 slabclass 数组，用于存储最大 64 个的slabclass_t的结构。  
static slabclass_t slabclass[MAX_NUMBER_OF_SLAB_CLASSES];  
```

## 问题：Memcached 如何设计 slabs 内存分配？

为了防止内存碎片，设计不同规格的内存块，内存块在 Memcached 里就是 slabclass，slabclass[0] 是预留的内存块，所以最大支持有 63 种不同规格。

对一个规格的内存块 slabclass 进行细分，分成若干相同大小的 slab(1M 内存页)，每一个 slab 会按照 slabclass_t->size 切分成 N 个 chunk，每一个 chunk 存储一个 item，使用双向链表结构连接 item。

item 是缓存数据存储的基本单元，根据用户需要存储数据(item)来选择合适的内存块 slabclass。

chunk 的增加因子由 -f 指定，默认为 1.25。

删除的 item 并不是真正删除，而是使用链表连接空闲的 item。

## 问题：Memcached 内存管理的操作？

**1. 向内存池申请内存**

当用户向缓存 Memcached 中存储数据，Memcached 会根据用户存储数据大小，来选择合适的 slabclass，该内存下有空闲的 item，就分配；否则对该 slabclass 申请一个内存页 slab，从新申请的内存页中分配空闲的 item。

**2. 怎么释放内存**

Memcached 释放内存，并不是真正释放，将释放的 item用空闲链表维护。

**3. 如何预分配内存**

用户需要预先分配一些内存，而不是等到客户端发送存储数据命令的时候才分配内存。

预分配内存是对每个规格的 slabclass 进行分配一个内存页 slab(从 slabclass[0] 获取)；然后分别将内存页 slab 进行切分 N 个 chunk，用来存储 item。

![](https://github.com/steveLauwh/Memcached/raw/master/The%20Annotated%20Memcached%20Sources/image/slab.PNG)