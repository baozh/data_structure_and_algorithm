## Bloom Filter 基础知识

可参考《大数据日知录-架构与算法》，或者参考[这篇文章](http://blog.csdn.net/hguisu/article/details/7866173)

**特点：**  
1）有一定的误判率，不适合“零错误”的场合。  
2）节省空间，且空间利用率 较高，查询速度快。  
3) 不支持删除，只支持添加、查找。  
   改进：counting Bloom Filter  支持删除操作  
4）适合在线下计算出 所有元素产生的位数组（经过hash函数计算），在线上 使用这个位数组 来加快 查询速度。

**实现时要考虑的:**  
1）集合大小n应该是确定的，而 位数组大小m、哈希函数的个数k如何确定？  
给定m,n =>  可根据公式，计算出 在可容忍的误判率下，最优的 哈希函数的个数k。  
2）哈希函数如何确定，k个哈希函数都一样吗？  
3) 每个key用几个位 存放count？

## From Leveldb

特点：  
1）哈希函数都使用同一个。  
2）根据 每个key占用的位个数`bits_per_key` 来计算 哈希函数的个数k:  

	k_ = static_cast<size_t>(bits_per_key * 0.69);  // 0.69 =~ ln(2)

3) 在根据所有key计算位数组时，将 `哈希函数的个数k` 硬编码到最后一个字符，用双层hash来计算key对应的位数组的索引：

    char* array = &(*dst)[init_size];
    for (int i = 0; i < n; i++) {
      // Use double-hashing to generate a sequence of hash values.
      // See analysis in [Kirsch,Mitzenmacher 2006].
      uint32_t h = BloomHash(keys[i]);
      const uint32_t delta = (h >> 17) | (h << 15);  // Rotate right 17 bits
      for (size_t j = 0; j < k_; j++) {
        const uint32_t bitpos = h % bits;
        array[bitpos/8] |= (1 << (bitpos % 8));
        h += delta;
      }
    }

## From rocksDB

