## Memcached 原理

### Memcached 的内存存储：使用 Slab Allocator 机制

Slab Allocator 基本原理是按照预先规定的大小，将分配的内存分割成特定长度的块(chunk)，长度相同的块分成组(chunk 的集合)，用来解决内存碎片的问题。

同时 Slab Allocator 还有重复使用已分配的内存目的。

### Memcached 的删除机制：使用 LRU(Least Recently Used) 算法

当 Memcached 的内存空间不足时，就从最近未使用的记录中搜索，并将空间分配给新的记录。

### Memcached 的分布式：由客户端程序库实现，一致性哈希算法(Consistent Hashing)

熟悉理解一致性哈希算法(Consistent Hashing)。

首先求出 Memcached 服务器（节点）的哈希值，并将其配置到 0～2^32 的圆（continuum）上。然后用同样的方法求出存储数据的键的哈希值，并映射到圆上。然后从数
据映射到的位置开始顺时针查找，将数据保存到找到的第一个服务器上。如果超过 2^32 仍然找不到服务器，就会保存到第一台 Memcached 服务器上。

## 下一步研究 Memcached 源码

## 参考

* [Memcached 安装使用和源码调试](http://blog.csdn.net/unix21/article/details/15501049)
