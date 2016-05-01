
## From leveldb

### 特点  

1）线程安全  
2）只提供LRU的淘汰策略  
3）长度固定，key-value，non-copyable  
4) key和value都是堆上分配内存，value由用户在insert()前分配好，在insert()时 插入一个custom deleter（当结点销毁时，会回调这个deleter）。  
5)  节点中含有一个ref(引用计数)，相当于智能指针std::shared_ptr。 在insert(),Lookup()后，它需要显式调用release()来减少计数。（bzh: 这么做感觉不太方便，很容易出错。）  
Erase(handle)只删除LruCache中的节点指针，不删除实际的结点空间。只有当所有获取handle的例程调用release(handle)后，才会真正删除节点空间（调用注册的custom deleter）。


### 学习要点

1) 接口设计。  
对外的接口：`虚基类`  
实现：继承`虚基类`，实现虚函数。  
其中，Handle类对外定义的内容为空，`实现`中定义一个具体的LruHandle，然后用reinterpret_cast<T>来做强制转换。  

2）实现原理与教科书上的一致：`hash_table + double linked list`。

3） hash函数的设计、hash_table的设计  
hash_table是自己实现的，据测 比g++中的hash_table性能高5%以上。

4） 测试  
除了测试对外的接口，还要测试LruCache的淘汰策略、线程安全性。


## From toft

1）利用STL中的std::list和std::map实现 LruCache。

2) 在测试代码`TEST(ThreadSafeLruCacheTest, MultiThread)`中，用`NewClosure`函数来产生一个类似`boost::function`的函数对象，在thread_pool中会回调这个函数对象。
`NewClosure`的实现代码是由 `pump.py closure.h.pump` 生成，代码有点复杂，没看懂。


## From rocksdb




## From Oceanbase





## From lizardfs