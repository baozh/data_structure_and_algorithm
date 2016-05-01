interface ResultStream {
  Integer Next();
}

class UnionOperator {
  /** 
   * Minimizing time to first tuple, returns a stream of tuples applying the union operator
   * @one: First input stream (unsorted)
   * @two: second input stream (unsorted)
   * @returns: a stream of the (distinct) union of elements in both inputs 
   */
  UnionOperator (ResultStream<Integer> one, ResultStream<Integer> two) {
    countMap = new HashSet();
    this.one = one;
    this.two = two;
  }

  HashSet<Integer> countMap;
  Integer Next() {
      while (this.one.hasNext()) {
          int ele = this.one.next();
          if (!countMap.containsKey(ele)) {
              countMap.add(ele);
              return ele;
          }
      }
      // no more input from one (one is EOF)
      while (this.two.hasNext()) {
          int ele = this.two.next();
          if (!countMap.containsKey(ele)) {
              countMap.add(ele);
              return ele;
          }
      }
      return NULL;
  }
  // other fields / methods are fine
}



class Pipeline {
   void main() {
      Stream one = asyncProcessOne();   
      Stream two = asyncProcessTwo();
      
      for(Integer serial : union(one, two).iterator()) {
         println(serial);
      }
   }
}
