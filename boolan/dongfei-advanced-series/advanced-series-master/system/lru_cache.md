
 如何设计一个LRU Cache？
 
Google和百度的面试题都出现了设计一个Cache的题目，什么是Cache，如何设计简单的Cache，通过搜集资料，本文给出个总结。
 通常的问题描述可以是这样：
Question：
[1] Design a layer in front of a system which cache the last n requests and the responses to them from the system.
在一个系统之上设计一个Cache，缓存最近的n个请求以及系统的响应。
what data structure would you use to implement the cache in the later to support following operations.
用什么样的数据结构设计这个Cache才能满足下面的操作呢？
[a] When a request comes look it up in the cache and if it hits then return the response from here and do not pass the request to the system
[b] If the request is not found in the cache then pass it on to the system
[c] Since cache can only store the last n requests, Insert the n+1th request in the cache and delete one of the older requests from the cache
因为Cache只缓存最新的n个请求，向Cache插入第n+1个请求时，从Cache中删除最旧的请求。
[d]Design one cache such that all operations can be done in O(1) – lookup, delete and insert.
 Cache简介：
Cache(高速缓存)， 一个在计算机中几乎随时接触的概念。CPU中Cache能极大提高存取数据和指令的时间，让整个存储器(Cache+内存)既有Cache的高速度，又能有内存的大容量；操作系统中的内存page中使用的Cache能使得频繁读取的内存磁盘文件较少的被置换出内存，从而提高访问速度；数据库中数据查询也用到Cache来提高效率；即便是Powerbuilder的DataWindow数据处理也用到了Cache的类似设计。Cache的算法设计常见的有FIFO(first in first out)和LRU(least recently used)。根据题目的要求，显然是要设计一个LRU的Cache。
 解题思路：
Cache中的存储空间往往是有限的，当Cache中的存储块被用完，而需要把新的数据Load进Cache的时候，我们就需要设计一种良好的算法来完成数据块的替换。LRU的思想是基于“最近用到的数据被重用的概率比较早用到的大的多”这个设计规则来实现的。
为了能够快速删除最久没有访问的数据项和插入最新的数据项，我们双向链表连接Cache中的数据项，并且保证链表维持数据项从最近访问到最旧访问的顺序。每次数据项被查询到时，都将此数据项移动到链表头部（O(1)的时间复杂度）。这样，在进行过多次查找操作后，最近被使用过的内容就向链表的头移动，而没有被使用的内容就向链表的后面移动。当需要替换时，链表最后的位置就是最近最少被使用的数据项，我们只需要将最新的数据项放在链表头部，当Cache满时，淘汰链表最后的位置就是了。

  注： 对于双向链表的使用，基于两个考虑。首先是Cache中块的命中可能是随机的，和Load进来的顺序无关。其次，双向链表插入、删除很快，可以灵活的调整相互间的次序，时间复杂度为O(1)。

    查找一个链表中元素的时间复杂度是O(n)，每次命中的时候，我们就需要花费O(n)的时间来进行查找，如果不添加其他的数据结构，这个就是我们能实现的最高效率了。目前看来，整个算法的瓶颈就是在查找这里了，怎么样才能提高查找的效率呢？Hash表，对，就是它，数据结构中之所以有它，就是因为它的查找时间复杂度是O(1)。
梳理一下思路：对于Cache的每个数据块，我们设计一个数据结构来储存Cache块的内容，并实现一个双向链表，其中属性next和prev时双向链表的两个指针，key用于存储对象的键值，value用户存储要cache块对象本身。
 Cache的接口：
查询：

   * 根据键值查询hashmap，若命中，则返回节点，否则返回null。
   * 从双向链表中删除命中的节点，将其重新插入到表头。
   * 所有操作的复杂度均为O(1)。

插入：

   * 将新的节点关联到Hashmap
   * 如果Cache满了，删除双向链表的尾节点，同时删除Hashmap对应的记录
   * 将新的节点插入到双向链表中头部

更新：

   * 和查询相似

删除：

   * 从双向链表和Hashmap中同时删除对应的记录。

LRU Cache的Java 实现：

     public interface Cache<K extends Comparable, V> {

       V get(K obj);  //查询
       void put(K key, V obj); //插入和更新
       void put(K key, V obj, long validTime);
       void remove(K key); //删除
       Pair[] getAll();
       int size();
    }
      public class Pair<K extends Comparable, V> implements Comparable<Pair> {

       public Pair(K key1, V value1) {
          this.key = key1;
          this.value = value1;
       }
       public K key;
       public V value;
       public boolean equals(Object obj) {
          if(obj instanceof Pair) {
             Pair p = (Pair)obj;
             return key.equals(p.key)&&value.equals(p.value);
          }
          return false;
       }
       @SuppressWarnings("unchecked")
       public int compareTo(Pair p) {
          int v = key.compareTo(p.key);
          if(v==0) {
             if(p.value instanceof Comparable) {
                return ((Comparable)value).compareTo(p.value);
             }
          }
          return v;
       }
       @Override
       public int hashCode() {
          return key.hashCode()^value.hashCode();
       }
       @Override
       public String toString() {
          return key+": "+value;
       }
    }
     public class LRUCache<K extends Comparable, V> implements Cache<K, V>,

          Serializable {
       private static final long serialVersionUID = 3674312987828041877L;
       Map<K, Item> m_map = Collections.synchronizedMap(new HashMap<K, Item>());
       Item m_start = new Item();      //表头
       Item m_end = new Item();        //表尾
       int m_maxSize;
       Object m_listLock = new Object();        //用于并发的锁
       static class Item {
          public Item(Comparable k, Object v, long e) {
             key = k;
             value = v;
             expires = e;
          }
          public Item() {}
          public Comparable key;        //键值
          public Object value;          //对象
           public long expires;          //有效期
          public Item previous;
          public Item next;
       }
       void removeItem(Item item) {
          synchronized(m_listLock) {
             item.previous.next = item.next;
             item.next.previous = item.previous;
          }
       }
       void insertHead(Item item) {
          synchronized(m_listLock) {
             item.previous = m_start;
             item.next = m_start.next;
             m_start.next.previous = item;
             m_start.next = item;
          }
       }
       void moveToHead(Item item) {
          synchronized(m_listLock) {
             item.previous.next = item.next;
             item.next.previous = item.previous;
             item.previous = m_start;
             item.next = m_start.next;
             m_start.next.previous = item;
             m_start.next = item;
          }
       }
       public LRUCache(int maxObjects) {
          m_maxSize = maxObjects;
          m_start.next = m_end;
          m_end.previous = m_start;
       }
       @SuppressWarnings("unchecked")
       public Pair[] getAll() {
          Pair p[] = new Pair[m_maxSize];
          int count = 0;
          synchronized(m_listLock) {
             Item cur = m_start.next;
             while(cur!=m_end) {
                p[count] = new Pair(cur.key, cur.value);
                ++count;
                cur = cur.next;
             }
          }
          Pair np[] = new Pair[count];
          System.arraycopy(p, 0, np, 0, count);
          return np;
       }
       @SuppressWarnings("unchecked")
       public V get(K key) {
          Item cur = m_map.get(key);
          if(cur==null) {
             return null;
          }
         //过期则删除对象
          if(System.currentTimeMillis()>cur.expires) {
             m_map.remove(cur.key);
             removeItem(cur);
             return null;
          }
          if(cur!=m_start.next) {
             moveToHead(cur);
          }
          return (V)cur.value;
       }
       public void put(K key, V obj) {
          put(key, obj, -1);
       }
       public void put(K key, V value, long validTime) {
          Item cur = m_map.get(key);
          if(cur!=null) {
             cur.value = value;
             if(validTime>0) {
                cur.expires = System.currentTimeMillis()+validTime;
             }
             else {
                cur.expires = Long.MAX_VALUE;
             }
             moveToHead(cur);  //成为最新的对象，移动到头部
             return;
          }
          if(m_map.size()>=m_maxSize) {
             cur = m_end.previous;
             m_map.remove(cur.key);
             removeItem(cur);
          }
          long expires=0;
          if(validTime>0) {
             expires = System.currentTimeMillis()+validTime;
          }
          else {
             expires = Long.MAX_VALUE;
          }
          Item item = new Item(key, value, expires);
          insertHead(item);
          m_map.put(key, item);
       }
       public void remove(K key) {
          Item cur = m_map.get(key);
          if(cur==null) {
             return;
          }
          m_map.remove(key);
          removeItem(cur);
       }
       public int size() {
          return m_map.size();
       }

    }

