# Memcached 使用

## Memcached 是什么

Memcached 是一个高性能的分布式内存对象缓存系统，用于动态 Web 应用以减轻数据库负载。

它是基于内存的 key-value 存储，用来存储小块的任意数据（字符串、对象）。这些数据可以是数据库调用、API 调用或者是页面渲染的结果。

## Memcached 命令

### 1. Memcached 存储命令

> **set**

用于将 value(数据值) 存储在指定的 key(键) 中。

如果 set 的 key 已经存在，该命令可以更新该 key 所对应的原来的数据，也就是实现更新的作用。

```shell
set key flags exptime bytes [noreply] 
value 
```
参数说明如下：

* key：键值 key-value 结构中的 key，用于查找缓存值
* flags：可以包括键值对的整型参数，客户端使用它存储关于键值对的额外信息 
* exptime：在缓存中保存键值对的时间长度（以秒为单位，0 表示永远）
* bytes：在缓存中存储的字节数
* noreply（可选）：该参数告知服务器不需要返回数据
* value：存储的值（始终位于第二行）（可直接理解为 key-value 结构中的 value）

例如：

```shell
set firstname 0 60 3
Lau
```
> **add**

用于将 value(数据值) 存储在指定的 key(键) 中。

如果 add 的 key 已经存在，则不会更新数据(过期的 key 会更新)，保持之前的值。

```shell
add key flags exptime bytes [noreply]
value
```

例如：

```shell
add lastname 0 60 5
steve
```
> **replace**

用于替换已存在的 key(键) 的 value(数据值)。

如果 key 不存在，则替换失败，并且获得响应 NOT_STORED。

```shell
replace key flags exptime bytes [noreply]
value
```

例如：

```shell
replace lastname 0 60 4
poly
```

> **append**

用于向已存在 key(键) 的 value(数据值) 后面追加数据。

```shell
append key flags exptime bytes [noreply]
value
```

例如：

```shell
append lastname 0 60 4
redis
```

> **prepend**

用于向已存在 key(键) 的 value(数据值) 前面追加数据 。

```shell
prepend key flags exptime bytes [noreply]
value
```

例如：

```shell
prepend lastname 0 60 8
monogodb
```

> **CAS**

CAS(Check-And-Set 或 Compare-And-Swap)。

用于执行一个"检查并设置"的操作，它仅在当前客户端最后一次取值后，该 key 对应的值没有被其他客户端修改的情况下，才能够将值写入。

unique_cas_token 通过 gets 命令获取的一个唯一的64位值。

```shell
cas key flags exptime bytes unique_cas_token [noreply]
value
```

例如：

```shell
set cache 0 900 9
memcached
STORED

gets cache
VALUE tp 0 9 8
memcached
END

cas cache 0 900 5 8
redis
STORED

get cache
VALUE cache 0 5
redis
END
```

### 2. Memcached 查找命令

> **get**

获取存储在 key(键) 中的 value(数据值) ，如果 key 不存在，则返回空。

```shell
get key
或
get key1 key2 key3
```

> **gets**

获取带有 CAS 令牌存 的 value(数据值) ，如果 key 不存在，则返回空。

在 CAS 操作中，使用 gets 命令可以获得令牌值。

```shell
gets key
或
gets key1 key2 key3
```

> **delete**

用于删除已存在的 key(键)。

```shell
delete key [noreply]
```

例如：

```shell
set name 0 900 5
redis
STORED
get name
VALUE name 0 5
redis
END
delete name
DELETED
get name
END
get name
END
delete name
NOT_FOUND
```

> **incr 与 decr**

incr 与 decr 命令用于对已存在的 key(键) 的数字值进行自增或自减操作。
 
incr 与 decr 命令操作的数据必须是十进制的32位无符号整数。
 
```shell
incr key increment_value
 
decr key decrement_value
```
 
 例如：

```shell
set value 0 900 2
10
STORED
get value
VALUE value 0 2
10
END
incr value 5
15
get value
VALUE value 0 2
15
END
decr value 5
10
get value
VALUE value 0 2
10
END
```

### 3. Memcached 统计命令

> **stats**

用于返回统计信息例如 PID(进程号)、版本号、连接数等。

> **stats items**

用于显示各个 slab 中 item 的数目和存储时长(最后一次访问距离现在的秒数)。

> **stats slabs**

用于显示各个slab的信息，包括chunk的大小、数目、使用情况等。

> **stats sizes**

用于显示所有 item 的大小和个数。

该信息返回两列，第一列是 item 的大小，第二列是 item 的个数。

> **flush_all**

用于清理缓存中的所有 key=>value(键=>值) 对。

该命令提供了一个可选参数 time，用于在制定的时间后执行清理缓存操作。

```shell
flush_all [time] [noreply]
```

> 退出 

quit


