## Linux 下 Memcached 安装

Memcached 是以守护程序方式运行于一个或多个服务器中，随时会接收客户端的连接和操作。

Memcached 主要是基于 libevent 库进行开发的。

**1. 安装 libevent 库**

```
tar -zxvf libevent-2.1.8-stable.tar.gz

cd libevent-2.1.8-stable

./configure -prefix=/usr/local/memcache/libevent

make && make install
```

**2. 安装 Memcached**

--with-libevent: 指定 libevent 的安装目录，--prefix: 指定安装的路径。

```
tar -zxvf memcached-1.5.3.tar.gz

 cd memcached-1.5.3
 
./configure --prefix=/usr/local/memcache/ --with-libevent=/usr/local/memcache/libevent

make && make install
```

**3. 开启 Memcached**

// 命令帮助，查看启动参数
/usr/local/memcache/bin/memcached -h   

```
// 后台运行，分配给 Memcached 使用的内存数量为 64M，监听端口为 11211
/usr/local/memcache/bin/memcached -d -u root -m 64M -p 11211

// 测试
telnet 127.0.0.1 11211
```

为了方便在 Shell 中启动 Memcached，把 Memcached 可执行文件目录添加到环境配置中

```shell
// Memcached 可执行文件在 /usr/local/memcache/bin 目录下
// 然后在 /etc/bash.bashrc 文件最后添加
exportPATH=$PATH:/usr/local/memcache/bin

// man 查询 Memcached 的帮助手册
cp /usr/local/memcache/share/man/man1/memcached.1 /usr/local/share/man/man1
```

**4. 关闭 Memcached**

```
ps -ef|grep memcached

kill -9 pid
```
