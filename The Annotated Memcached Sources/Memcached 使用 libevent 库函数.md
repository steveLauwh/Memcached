## Memcached 使用 libevent 库函数

Memcached 是基于 libevent 网络库开发的，在阅读 Memcached 源码的时候，需要了解 libevent 常用函数作用。

其实正确应该先学习 libevent 网络库，再去阅读 Memcached 源码。

为了聚焦对 Memcached 源码的学习，暂时初步了解 libevent，后续深入学习 libevent 网络库。

### event_base

event_base 是事件 event 的集合(事件池)，主要用来管理和实现事件 event 的监听循环。

一般情况下，一个线程对应一个 event_base。

### event_init

```c
struct event_base *event_init(void);
```

作用是初始化一个全局的变量 current_base，该变量类型就是 event_base。

### event_add

```c
int event_add(struct event *ev, const struct timeval *tv);
```

作用是用于向 event_base 注册事件 event。

### event_base_loop

```c
int event_base_loop(struct event_base *base, int flags); 
```

作用是不断循环监听注册的事件 event。

### event_base_set

```c
int event_base_set(struct event_base *base, struct event *ev);
```

作用是修改 event 和 event_base 关联。

### event_set

```c
void event_set(struct event *ev, evutil_socket_t fd, short events,
               void (*callback)(evutil_socket_t, short, void *), void *arg)
```

作用是初始化 event 对象，将 event 对象所属 event_base 指向全局的 current_base。

### event_del

```c
int event_del(struct event *ev);
```

作用是清理 event 的内存。这个方法并不是真正意义上的释放内存。

## 参考

* [libevent 官网](http://libevent.org/)
* [Linux c 开发-libevent](http://blog.csdn.net/initphp/article/details/41946061)
* [libevent 源码深度剖析](http://blog.csdn.net/sparkliang/article/category/660506)
