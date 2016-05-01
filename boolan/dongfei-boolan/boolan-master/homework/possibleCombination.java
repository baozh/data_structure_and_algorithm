import java.util.ArrayList;
import java.util.List;

/**
 * 给string, 只包含{0,1,?}, ?可以代表0或者1, 输出所有的组合. 例如"10?", 输出"100", "101"
 */
public class PossibleCombination {
    public static List<String> getCombination(String s){
        List<String> result = new ArrayList<String>();
        StringBuilder single = new StringBuilder();
        dfs(s, 0, single, result);
        return result;
    }

    private static void dfs(String s, int index, StringBuilder single, List<String> result){
        if(index == s.length()){
            result.add(single.toString());
            return;
        }

        if(s.charAt(index) == '?'){
            single.append('0');
            dfs(s, index + 1, single, result);
            single.deleteCharAt(single.length() - 1);

            single.append('1');
            dfs(s, index + 1, single, result);
            single.deleteCharAt(single.length() - 1);

        }else{
            single.append(s.charAt(index));
            dfs(s, index + 1, single, result);
            single.deleteCharAt(single.length() - 1);
        }
    }

    public static void main(String[] args){
        System.out.println(getCombination("10?"));
    }
}