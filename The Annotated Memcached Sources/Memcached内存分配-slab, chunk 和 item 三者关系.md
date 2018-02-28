## Memcached 内存分配—slab, chunk 和 item 三者关系

```
root@poly-Lenovo-Product:/home/poly# memcached -u root -m 64 -p 11211 -n 80 -vv
slab class   1: chunk size       112 perslab    9362
slab class   2: chunk size       144 perslab    7281
slab class   3: chunk size       184 perslab    5698
slab class   4: chunk size       232 perslab    4519
slab class   5: chunk size       296 perslab    3542
slab class   6: chunk size       376 perslab    2788
slab class   7: chunk size       472 perslab    2221
slab class   8: chunk size       592 perslab    1771
slab class   9: chunk size       744 perslab    1409
slab class  10: chunk size       936 perslab    1120
slab class  11: chunk size      1176 perslab     891
slab class  12: chunk size      1472 perslab     712
slab class  13: chunk size      1840 perslab     569
slab class  14: chunk size      2304 perslab     455
slab class  15: chunk size      2880 perslab     364
slab class  16: chunk size      3600 perslab     291
slab class  17: chunk size      4504 perslab     232
slab class  18: chunk size      5632 perslab     186
slab class  19: chunk size      7040 perslab     148
slab class  20: chunk size      8800 perslab     119
slab class  21: chunk size     11000 perslab      95
slab class  22: chunk size     13752 perslab      76
slab class  23: chunk size     17192 perslab      60
slab class  24: chunk size     21496 perslab      48
slab class  25: chunk size     26872 perslab      39
slab class  26: chunk size     33592 perslab      31
slab class  27: chunk size     41992 perslab      24
slab class  28: chunk size     52496 perslab      19
slab class  29: chunk size     65624 perslab      15
slab class  30: chunk size     82032 perslab      12
slab class  31: chunk size    102544 perslab      10
slab class  32: chunk size    128184 perslab       8
slab class  33: chunk size    160232 perslab       6
slab class  34: chunk size    200296 perslab       5
slab class  35: chunk size    250376 perslab       4
slab class  36: chunk size    312976 perslab       3
slab class  37: chunk size    391224 perslab       2
slab class  38: chunk size    489032 perslab       2
slab class  39: chunk size    611296 perslab       1
slab class  40: chunk size    764120 perslab       1
slab class  41: chunk size   1048576 perslab       1
```
### 分析上面打印的 slab class 1

首先 chunk size 为什么是 112？

计算 chunk size 公式：`chunk size(class i) = (default_size+item_size)*f^(i-1) + CHUNK_ALIGN_BYTES`

default_size：默认大小为 48 字节，可以通过 -n 参数来调节其大小；我们这里 -n 80，所以 default_size 变为 80 字节

item_size：默认大小为 32 字节

f：factor，是 chunk 变化大小的因素，默认值为 1.25，调节 f 可以影响 chunk 的步进大小，在启动时可以使用 -f 来指定

CHUNK_ALIGN_BYTES：修正值，在 32 位机器上要求 chunk 的大小是 4 的整数倍

所以可以计算 slab class 1 的 chunk size 大小为 80+32 = 112 字节

由于 slab 默认大小为 1M，可以通过 -I 参数修改；所以 perslab * (chunk size) + 空闲多余 = 1M = 1048576 bytes

### 总结

Memcached 默认的 slab 大小是 1M，所以不能存入大小超过 1M 的数据。

每一个 slab 包含相同大小的 chunk。

根据 chunk 大小不同，分为不同种类的 slab，也叫 slab class。

item 是用户存放的数据，需要存放到 chunk 中。

向 Memcached 添加一个 item 时候，Memcached 首先会根据 item 的大小，来选择最合适的 slab class。









