import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.Random;

/**
 * Given an array and a number k where k is smaller than size of array, we need to find the k’th smallest element in the given array.
 * It is given that ll array elements are distinct.
 *
 * Examples:
 *
 * Input: arr[] = {7, 10, 4, 3, 20, 15}
 * k = 3
 * Output: 7
 *
 * Input: arr[] = {7, 10, 4, 3, 20, 15}
 * k = 4
 * Output: 10
 *
 * Reference:
 * http://www.geeksforgeeks.org/kth-smallestlargest-element-unsorted-array/
 * http://www.geeksforgeeks.org/kth-smallestlargest-element-unsorted-array-set-2-expected-linear-time/
 * http://www.geeksforgeeks.org/k-largestor-smallest-elements-in-an-array/
 *
 */
public class FindKthSmallest {
    /**
     * Randomized quick select
     * Average/Best case: O(n)
     * Worst case: O(n^2)
     *
     */
    public static int findKthSmallest(int[] arr, int k) {
        if(k < 1 || k > arr.length) return -1;
        return findKthSmallest(arr, k - 1, 0, arr.length - 1);
    }

    public static int findKthSmallest(int[] arr, int k, int start, int end){
        int partitionIndex = partition(arr, start, end);
        if(partitionIndex > k) {
            return findKthSmallest(arr, k, start, partitionIndex - 1);
        } else if(partitionIndex < k){
            return findKthSmallest(arr, k, partitionIndex + 1, end);
        } else{
            return arr[partitionIndex];
        }
    }

    public static int partition(int[] arr, int start, int end){
        Random random = new Random();
        int pivotIndex = start + random.nextInt(end - start + 1);
        swap(arr, pivotIndex, end);
        int pivot = arr[end];
        int i = start, j = start;
        while(j < end) {
            if (arr[j] < pivot) {
                swap(arr, j, i);
                i++;
            }
            j++;
        }
        swap(arr, i, end);
        return i;
    }

    public static void swap(int[] arr, int x, int y){
        int tmp = arr[x];
        arr[x] = arr[y];
        arr[y] = tmp;
    }

    /**
     * Min heap
     * Time: O(n + klogn)
     *
     */
    public static int findKthSmallest2(int[] arr, int k){
        if(k < 1 || k > arr.length) return -1;
        Queue<Integer> queue = new PriorityQueue<Integer>(arr.length);
        for (int num : arr){
            queue.add(num);
        }

        for (int i = 0; i < k - 1; i++){
            queue.poll();
        }
        return queue.poll();
    }

    /**
     * Max heap
     * Time: O(k + (n - k)logk)
     *
     */
    public static int findKthSmallest3(int[] arr, int k){
        if(k < 1 || k > arr.length) return -1;
        Comparator<Integer> comparator = new Comparator<Integer>() {
            @Override
            public int compare(Integer o1, Integer o2) {
                return o2 - o1;
            }
        };

        Queue<Integer> queue = new PriorityQueue<Integer>(k, comparator);
        for (int i = 0; i < k; i++) {
            queue.add(arr[i]);
        }

        for (int i = k; i < arr.length; i++) {
            if (arr[i] < queue.peek()){
                queue.poll();
                queue.add(arr[i]);
            }
        }
        return queue.poll();
    }

    /**
     * Find kth largest
     * Min Heap
     * Time: O(k + (n - k)logk)
     */
    public static int findKthLargest(int[] arr, int k) {
        if(k < 1 || k > arr.length) return -1;
        Queue<Integer> queue = new PriorityQueue<Integer>(k);
        for(int i = 0; i < k; i++){
            queue.add(arr[i]);
        }

        for(int i = k; i < arr.length; i++){
            if(arr[i] > queue.peek()){
                queue.poll();
                queue.add(arr[i]);
            }
        }
        return queue.poll();
    }

    public static void main(String[] args){
        int arr[] = {7, 10, 4, 3, 20, 15};
        System.out.println(findKthSmallest(arr, 5));
        System.out.println(findKthSmallest2(arr, 5));
        System.out.println(findKthSmallest3(arr, 5));
        System.out.println(findKthLargest(arr, 5));
    }
}