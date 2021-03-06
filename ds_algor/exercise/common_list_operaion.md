#链表常见操作

------

推荐大家看下[《编程之美》][1]、[《程序员面试金典》][2]  
还有编程相关网站：[leetcode][3]
老师讲的很多题其实就是这些书和网站的题

##单链表是否存在环？环的入口是什么？

###是否存在环

1) 判断是否存在环：设置快慢指针fast和slow，fast步速为2，slow为1，若最终fast==slow，那么就证明单链表中一定有环。如果没有环的话，fast一定先到达尾节点

2) 简单证明：利用相对运动的概念，以slow为参考点（静止不动），那么fast的步速实际为1，当fast超过slow之后，fast以每步一个节点的速度追赶slow，如果链表有环的话，fast一定会追赶到slow，即fast==slow。

###如何找到环的入口

![第一次相遇][4]

字母代表的量：  
- a：链表头结点到环入口的距离  
- r：环长
- 蓝色线：fast指针所走的距离2s  
- 黑色线：slow指针所走的距离s

假设链表总长度为L，且fast与slow相遇时fast已经绕环走了n圈，则有如下关系：

2s = s + n*r  
将s移到左边得：  
s = n*r  
转换：  
s = (n-1)*r + r = (n-1)*r + L-a
a+x = (n-1)*r + L-a
得：
a = (n-1)*r + L-a-x

由图可知，（L-a-x）为相遇点到环入口点的距离。由上式可知：  
从链表头到环入口的距离 = （n-1）圈内环循环 + 相遇点到环入口点的距离  
将r视为周期的话，a与L-a-x在某种意义上是相等的（实际并不一定相等）。  
那么由此我们便找到了突破点，为了找到环的入口点，在fast与slow相遇时，将slow指针重新指向单链表的头节点，fast仍然留在相遇点，只不过步速降为与slow相同的1，每次循环只经过一个节点，如此，当fast与slow再次相遇时，那个新的相遇点便是我们苦苦寻找的入口点了。

###如何知道环的长度

纪录下相遇点，让slow与fast从该点开始，再次碰撞所走过的操作数就是环的长度r。  

###带环的链表的长度是多少？

通过以上分析我们已经知道了如何求环入口，环长，那么链表长度显然就是两者之和，即：  
L = a + r


##判断两个链表是否相交

分析问题之前我们要搞清楚链表相交的一些基本概念  

- 明确概念：两个单向链表相交，只能是y型相交，不可能是x型相交。  
- 分析：有两个链表，La,Lb,设他们的交点设为p，假设在La中，p的前驱为pre_a，后继为next_a,在Lb中，前驱为pre_b,后继为next_b，则
pre_a->next=p,pre_b->next=p,接下来看后继,p->next=next_a,p->next=next_b；那么问题就出来了，一个单链表的next指针只有一个，
怎么跑出两个来呢，所以必有next_a==next_b，于是我们得出两个链表相交只能是Y型相交。明确了这个概念，我们再来堆相交问题进行分析。

###情况一：两个链表都无环

1) 问题简化。将链表B接到链表A的后面，如果A、B有交点，则构成一个有环的单链表，而我们刚刚在上面已经讨论了如何判断一个
单链表是否有环。  
2) 若两个链表相交则必为Y型，由此可知两个链表从相交点到尾节点是相同的，我们并不知道他们的相交点位置，但是我们可以遍历得出A、B链表的
尾节点，如此，比较他们的尾节点是否相等便可以求证A、B是否相交了。

###情况二：链表有环

1) 其中一个链表有环，另外一个链表无环。则两个链表不可能相交。（啥？你不知道为啥？自己看看前面的“明确概念”反省吧）

2) 那么有环相交的情况只有当两个链表都有环时才会出现，如果两个有环链表相交，则他们拥有共通的环，即环上任意一个节点都存在于
两个链表上。因此，通过判断A链表上的快慢指针相遇点是否也在B链表上便可以得出两个链表是否相交了。


##求相交链表的相交点

题目描述：如果两个无环单向链表相交，怎么求出他们相交的第一个节点呢？

分析：采用对齐的思想。计算两个链表的长度 L1 , L2，分别用两个指针 p1 , p2 指向两个链表的头，然后将较长链表的 p1（假设为 p1）向后移动L2 - L1个节点，然后再同时向后移动p1 , p2，直到 p1 = p2。相遇的点就是相交的第一个节点。


------

[1]:http://book.douban.com/subject/3004255/
[2]:http://book.douban.com/subject/25753386/
[3]:https://leetcode.com/problemset/algorithms/
[4]:http://ww4.sinaimg.cn/mw690/89b29945gw1eurmxod4q0j20eq09tjs5.jpg
