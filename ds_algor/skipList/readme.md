
### From Google Leveldb

skiplist的实现代码 中规中矩，基本和教科书中的一致。

值得注意的：

1. 对并发(多线程访问)的控制 比较 细致。
对 `prev`,`next`,`max_height` 用的`原子指针类型（AtomicPointer）`，并区分了 什么时候用 `AtomicPointer.Acquire_Load()`，什么时候用 `AtomicPointer.NoBarrier_Load()`。

2. 性能优化  
1）用了原子指针类型 来避免加锁。  
2）用专门的内存分配器 (arena对象) 来管理 Node的分配/释放。  

3. 测试比较细致。除了测试基本的接口，还对并发的各种case(多读者单写者)进行了测试。   



### From Tencent toft
代码和leveldb一致，只对 `原子指针类型AtomicPointer` 重写了一下。

AtomicPointer的实现要点：  
由`memory barrier`来保证原子访问，`memory barrier` 由 内嵌汇编实现：

	inline void MemoryBarrier() { __asm__ __volatile__("mfence": : :"memory"); }
	inline void MemoryReadBarrier() { __asm__ __volatile__("lfence" ::: "memory"); }
	inline void MemoryWriteBarrier() { __asm__ __volatile__("sfence" ::: "memory"); }

### From Fackbook Rocksdb

代码和leveldb一致，`原子类型`用了C++11的`std::atomic<T>`模板。

在`Insert()`函数中有一个优化：  
`prev[0]`保存 上一次插入元素的结点**指针**，`prev_height_`保存 上一次插入元素的结点的**高度**。

如果 `要插入的元素key` 比 `上一次插入的元素` 小的话，prev[]数组(0~`prev_height_`) 的值都为prev[0]，否则，调用 `FindLessThan()` 获取 每层比 key 小的最后一个元素的指针。

### From Redis

Redis中用skiplist实现了 `有序集合`，为支持更多的操作，它对原始的skiplist做了些修改（支持重复的元素，在第一层的链表中加入向前的指针）。

接口见`redis.h`，实现代码见`t_zset.c`。`t_zset.c`的代码量比较大(三千多行），以后有时间再研究一下。


### From skiplist-master 

Github上的一个[开源实现](https://github.com/begeekmyfriend/skiplist)。





