import java.util.*;
/**
 * Implement a simple database that support:
 * Basic
 *  * GET <KEY>: retrieve a key, if not exist, return NULL.
 *  * SET <KEY> <VALUE>: set a key with value, if key exist, override. VALUE is integer
 *  * COUNT <VALUE>: return how many times that equal to a value, if not found, return 0.
 *  * DELETE <KEY>: delete a key
 *
 * Transaction
 *  * BEGIN: start a transaction, it could be nested
 *  * ROLLBACK: if in a transaction, it remains status before that transaction; otherwise, return NO TRANSACTION
 *  * COMMIT: commit all of transaction and make it persistent
 */
public class SimpleDB {
  Map<Integer, Integer> countMap;
  Map<String, Integer> cache;
  Stack<String> tranStack;
  boolean isTransactionStart;

  public SimpleDB() {
    countMap = new HashMap<Integer, Integer>();
    cache = new HashMap<String, Integer>();
    tranStack = new Stack<String>();
  }

  public String process(String input) {
    String[] segs = input.split(" ");
    String command = segs[0];
    Integer val;
    switch(command) {
      case "SET": {
        doSet(segs[1], Integer.parseInt(segs[2]));
        break;
      }
      case "GET": {
        val = doGet(segs[1]);
        return val == null? "NULL": val.toString();
      }
      case "DELETE": {
        doDelete(segs[1]);
        break;
      }
      case "COUNT": {
        val = doCount(Integer.parseInt(segs[1]));
        return val.toString();
      }
      case "BEGIN": {
        doBegin();
        break;
      }
      case "ROLLBACK": {
        boolean isTranscation = doRollback();
        return isTranscation? null : "NO TRANSACTION";
      }
      case "COMMIT": {
        doCommit();
        break;
      }
    }
    return null;
  }

  public void doBegin() {
    tranStack.push("#");
    isTransactionStart = true;
  }

  public boolean doRollback() {
    if (tranStack.isEmpty())
      return false;

    while (!tranStack.isEmpty() && !tranStack.peek().equals("#")) {
      String operations = tranStack.pop();
      String[] segs = operations.split(" ");
      String key = segs[0];
      String origVal = segs[1].equals("#")? "": segs[1];
      String newVal = segs[2].equals("#")? "": segs[2];
      int val;
      if (origVal.isEmpty()) {
        cache.remove(key);
      } else {
        cache.put(key, Integer.parseInt(origVal));
        val = Integer.parseInt(origVal);
        countMap.put(val, countMap.get(val)+1);
      }

      if (!newVal.isEmpty()) {
        val = Integer.parseInt(newVal);
        countMap.put(val, countMap.get(val)-1);
      }
    }

    if (!tranStack.isEmpty() && tranStack.peek().equals("#"))
      tranStack.pop();

    return true;
  }

  public void doCommit() {
    tranStack.clear();
    isTransactionStart = false;
  }

  public Integer doGet(String key) {
    return cache.containsKey(key)? cache.get(key): null;
  }

  public Integer doCount(int val) {
    return countMap.containsKey(val)? countMap.get(val): 0;
  }

  public void doSet(String key, int val) {
    if (isTransactionStart)
      tranStack.add(key + " " + (cache.containsKey(key)? cache.get(key): "#") + " " + val);

    if (cache.containsKey(key)) {
      int origVal = cache.get(key);
      if (countMap.containsKey(origVal))
        countMap.put(origVal, countMap.get(origVal)-1);
    }

    countMap.put(val, countMap.containsKey(val)? countMap.get(val) +1:1);
    cache.put(key, val);
  }

  public void doDelete(String key) {
    if (cache.containsKey(key)) {
      int origVal = cache.get(key);
      if (isTransactionStart)
        tranStack.add(key + " " + origVal + " #");
      countMap.put(origVal, countMap.get(origVal)-1);
      cache.remove(key);
    }
  }

  public static void main(String[] args) {
    SimpleDB db = new SimpleDB();
    Scanner scanner = new Scanner(System.in);
    while (true) {
      String input = scanner.nextLine();
      if (input.equals("END"))
        break;
      else {
        String res = db.process(input);
        if (res != null)
          System.out.println(res);
      }
    }
  }
}
