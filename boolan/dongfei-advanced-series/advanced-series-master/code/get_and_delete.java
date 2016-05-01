// design a data structure that can support: get element in O(1), delete randomly in O(1)

public class ConstantDataStructure<T>
{
   HashSet<T> _elementsSet;
   HashMap<Integer, T> _indexMap;
   HashMap<T, Integer> _reverseIndexMap;
 
   boolean add(T element)
   {
      if(!_elementSet.contains(element))
      {
         _elementSet.add(element);
         _indexMap.put(_elementSet.size() - 1, element);
         _reverseIndexMap.put(element, _elementSet.size() - 1);
         return true;
      }
      return false;
   }
 
   boolean remove(T element)
   {
      if(_elementSet.contains(element))
      {
         int index = _reverseIndexMap.get(element);
         _indexMap.remove(index);
         _reverseIndexMap.remove(element);
         if (_elementSet.size() > 1) // if this is the last element, do NOT update
         {
           T lastElement = _indexMap.get(_elementSet.size() - 1);
           _indexMap.remove(_elementSet.size() - 1);
           _indexMap.put(index, lastElement);
           _reverseIndexMap.put(lastElement, index);    // no need to remove: this is a replacement
         }
         _elementSet.remove(element);
         return true;
      }
      return false;
   }
 
   T removeRandomElement()
   {
      if(_elementSet.size() == 0)
      {
          return null;
      }
      int randIndex = new Random().nextInt(_elementSet.size());
      T element = _indexMap.get(randIndex);
      remove(element);
      return element;
   }
}
