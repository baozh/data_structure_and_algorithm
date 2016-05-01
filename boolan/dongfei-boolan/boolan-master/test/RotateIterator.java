
package com.google;


import java.util.*;

/**
 * 实现一个rotateIterator(), 构造函数传入List<Iterator<T>>, 实现next(), hasNext(). 例如:
 * 传入的三个iterator里面的值分别是[[1,2,3],[4,5,6], [7,8]], 那rotateIterator的next()应该输出[1,4,7,2,5,8,3,6].
 * 就是竖着遍历每个iterator输出, 如果当前的iterator没有了, 就跳到下一个.
 *
 */
public class RotateIterator implements Iterator<Integer>{
    List<Iterator<Integer>> iterators;
    Queue<Iterator<Integer>> queue;

    public RotateIterator(List<Iterator<Integer>> iterators){
        this.iterators = iterators;
        queue = new LinkedList<Iterator<Integer>>();
        for(Iterator<Integer> it : iterators){
            if(it.hasNext()) queue.offer(it);
        }
    }

    @Override
    public boolean hasNext() {
        return !queue.isEmpty();
    }

    @Override
    public Integer next() {
        if(hasNext()){
            Iterator<Integer> it = queue.poll();
            int val = it.next();
            if(it.hasNext()) queue.offer(it);
            return val;
        }
        return -1;
    }

    @Override
    public void remove() {

    }

    public static void main(String[] args){
        List<Integer> list1 = Arrays.asList(1,2,3);
        List<Integer> list2 = Arrays.asList(4,5,6);
        List<Integer> list3 = Arrays.asList(7,8);

        List<Iterator<Integer>> iterators = new ArrayList<Iterator<Integer>>();
        iterators.add(list1.iterator());
        iterators.add(list2.iterator());
        iterators.add(list3.iterator());

        RotateIterator it = new RotateIterator(iterators);
        while(it.hasNext()){
            System.out.print(it.next() + " ");
        }
    }
}