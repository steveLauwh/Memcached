## Memcached(版本 1.5.4)源码结构

Memcached 采用的是 C-S 架构，客户端不是"用户客户端"，而是能够处理用户请求并进行分布式处理的"中间服务器"。

客户端和服务器端采用 socket 通信，Memcached 服务器的主要工作就是处理客户端的命令请求，采用 master-worker 模式的多线程机制，主线程
和 工作线程采用管道通信，主线程负责监听事件请求，然后通过管道分发给不同的工作线程，工作线程根据主线程提供的相应信息，与 socket 端建立真正的连接，
进行数据接收和处理。

以下是 Memcached 源码(版本 1.5.4) 的结构预览：

```
├── aclocal.m4
├── assoc.c   // Hash 表的管理
├── assoc.h
├── AUTHORS
├── bipbuffer.c
├── bipbuffer.h  
├── cache.c
├── cache.h
├── ChangeLog
├── compile
├── config.guess
├── config.h.in
├── config.sub
├── configure
├── configure.ac
├── COPYING
├── crawler.c
├── crawler.h
├── daemon.c
├── depcomp
├── doc
│   ├── CONTRIBUTORS
│   ├── Doxyfile
│   ├── Makefile
│   ├── Makefile.am
│   ├── Makefile.in
│   ├── memcached.1
│   ├── new_lru.txt
│   ├── protocol-binary-range.xml
│   ├── protocol-binary.xml
│   ├── protocol.txt    // 帮助文档，参数解释
│   ├── readme.txt
│   ├── threads.txt
│   └── xml2rfc
│       ├── reference.RFC.0768.xml
│       ├── reference.RFC.2119.xml
│       ├── rfc2629.dtd
│       ├── rfc2629-noinc.xsl
│       ├── rfc2629-other.ent
│       ├── rfc2629-refchk.xsl
│       └── rfc2629-xhtml.ent
├── hash.c    // hash 算法
├── hash.h
├── install-sh
├── items.c   // 服务器端 item 对象管理
├── items.h
├── itoa_ljust.c
├── itoa_ljust.h
├── jenkins_hash.c
├── jenkins_hash.h
├── LICENSE.bipbuffer
├── linux_priv.c
├── logger.c
├── logger.h
├── m4
│   └── c99-backport.m4
├── Makefile.am
├── Makefile.in
├── memcached.c  // 主函数及控制逻辑
├── memcached_dtrace.d
├── memcached.h
├── memcached.spec
├── missing
├── murmur3_hash.c
├── murmur3_hash.h
├── NEWS
├── openbsd_priv.c
├── protocol_binary.h
├── README.md
├── sasl_defs.c
├── sasl_defs.h
├── scripts
│   ├── damemtop
│   ├── damemtop.yaml
│   ├── memcached-automove
│   ├── memcached-init
│   ├── memcached-server.upstart
│   ├── memcached.service
│   ├── memcached.sysv
│   ├── memcached-tool
│   ├── memcached-tool.1
│   ├── memcached.upstart
│   ├── README.damemtop
│   └── start-memcached
├── sizes.c
├── slab_automove.c
├── slab_automove.h
├── slabs.c    // 服务器端内存对象管理
├── slabs.h
├── solaris_priv.c
├── stats.c    // 数据统计
├── stats.h
├── t
│   ├── 00-startup.t
│   ├── 64bit.t
│   ├── binary-get.t
│   ├── binary-sasl.t
│   ├── binary.t
│   ├── bogus-commands.t
│   ├── cas.t
│   ├── chunked-items.t
│   ├── daemonize.t
│   ├── dash-M.t
│   ├── dyn-maxbytes.t
│   ├── evictions.t
│   ├── expirations.t
│   ├── flags.t
│   ├── flush-all.t
│   ├── getandtouch.t
│   ├── getset.t
│   ├── idle-timeout.t
│   ├── incrdecr.t
│   ├── inline_asciihdr.t
│   ├── issue_104.t
│   ├── issue_108.t
│   ├── issue_140.t
│   ├── issue_14.t
│   ├── issue_152.t
│   ├── issue_163.t
│   ├── issue_183.t
│   ├── issue_192.t
│   ├── issue_22.t
│   ├── issue_260.t
│   ├── issue_29.t
│   ├── issue_3.t
│   ├── issue_41.t
│   ├── issue_42.t
│   ├── issue_50.t
│   ├── issue_61.t
│   ├── issue_67.t
│   ├── issue_68.t
│   ├── issue_70.t
│   ├── item_size_max.t
│   ├── lib
│   │   └── MemcachedTest.pm
│   ├── line-lengths.t
│   ├── lru-crawler.t
│   ├── lru-maintainer.t
│   ├── lru.t
│   ├── malicious-commands.t
│   ├── maxconns.t
│   ├── misbehave.t
│   ├── multiversioning.t
│   ├── noreply.t
│   ├── quit.t
│   ├── refhang.t
│   ├── sasl
│   │   └── memcached.conf
│   ├── slabhang.t
│   ├── slabs-reassign2.t
│   ├── slabs-reassign-chunked.t
│   ├── slabs_reassign.t
│   ├── stats-conns.t
│   ├── stats-detail.t
│   ├── stats.t
│   ├── stress-memcached.pl
│   ├── touch.t
│   ├── udp.t
│   ├── unixsocket.t
│   ├── watcher.t
│   └── whitespace.t
├── testapp.c
├── thread.c   // 线程机制
├── timedrun.c
├── trace.h
├── util.c
├── util.h
└── version.m4
```
