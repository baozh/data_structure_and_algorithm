class Solution {
public:
    /**
     * @param l1: the first list
     * @param l2: the second list
     * @return: the sum list of l1 and l2
     */
    ListNode *addLists(ListNode *l1, ListNode *l2) {
        if (NULL == l1 && NULL == l2) {
            return NULL;
        }

        ListNode *sumlist = new ListNode(0);
        ListNode *templist = sumlist;

        int carry = 0;
        while ((NULL != l1) || (NULL != l2) || (0 != carry)) {
            // padding for NULL
            int l1_val = (NULL == l1) ? 0 : l1->val;
            int l2_val = (NULL == l2) ? 0 : l2->val;

            templist->val = (carry + l1_val + l2_val) % 10;
            carry = (carry + l1_val + l2_val) / 10;

            if (NULL != l1) l1 = l1->next;
            if (NULL != l2) l2 = l2->next;

            // return sumlist before generating new ListNode
            if ((NULL == l1) && (NULL == l2) && (0 == carry)) {
                return sumlist;
            }
            templist->next = new ListNode(0);
            templist = templist->next;
        }

        return sumlist;
    }
};
