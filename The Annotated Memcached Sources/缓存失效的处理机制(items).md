## 缓存失效的处理机制(items)

Memcached 缓存失效怎么处理？缓存对 Memcached 内部来说就是 items，那么对缓存处理，其实就是对 items 处理。

Memcached 在处理“已经过期或者失效的缓存(items)”是懒惰的，什么是懒惰？就是不会主动去检查 items 是否过期失效。

源码注释见：[items.c](https://github.com/steveLauwh/Memcached/blob/master/memcached-1.5.4/items.c)

### 方式一：

Memcached 并不主动去检查 items 是否失效，而是客户端再次请求该 items 才去判断是否失效或过期。

* 当该 items 被 flush 失效，这时候将 items 从 LRU 链表和哈希表上删除，同时从 slabs 内存中删除该 items(回收)

* 当该 items 没有被 flush，但是 items 已经过期了，也是从 LRU 链表和哈希表上删除，同时从 slabs 内存中删除该 items(回收)

具体实现：do_item_get 函数(get 命令获取一条数据)

### 方式二：

Memcached 分配 items 的时候去检查 LRU 链表上过期的 items。

先调用 slabs_alloc 分配一个 items，如果 slabs 内存管理器分配 items 失败，此时从该 items 所属的 LRU 链表尾部开始，尝试 5 次遍历

* 如果遍历的 items 过期或者被 flush 失效，就将遍历的 items 从 LRU 链表和哈希表上删除，同时从 slabs 内存中删除该 items

* 如果遍历的 items 没有过期，执行 LRU 淘汰(强制删除遍历的 items)

具体实现：do_item_alloc 函数(set、add 命令存储一条数据)

do_item_alloc_pull 函数

lru_pull_tail 函数

### 方式三

Memcached 使用一个独立的线程(LRU crawler 爬虫线程)对过期失效的缓存数据进行处理。

后面具体分析：[crawler.c](https://github.com/steveLauwh/Memcached/blob/master/memcached-1.5.4/crawler.c) 文件

原理：在 LRU 链表尾部插入一个伪 item，向前遍历，删除失效的 item。




