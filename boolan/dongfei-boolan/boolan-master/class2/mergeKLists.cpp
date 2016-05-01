class Solution {
public:
    // comparison structure, required by priority_queue template
    struct greaterListNode{
        bool operator() (ListNode* x, ListNode* y) {return x->val > y->val;}
    };

    ListNode *mergeKLists(vector<ListNode *> &lists) {
        if(lists.size() == 0)
            return NULL;

        if(lists.size() == 1)
            return lists[0];

        ListNode* dummy = new ListNode(-1);
        ListNode* pre = dummy;
        std::priority_queue<ListNode*, vector<ListNode*>, greaterListNode> pq;

        // Insert entries of every list into heap
        for(auto& lst:lists)
            if(lst != NULL)
                pq.push(lst);

        // Recursively parse every element in lists
        while (!pq.empty()) {
            pre->next = pq.top();
            pre = pre->next;
            pq.pop();
            if (pre != NULL && pre->next != NULL)
                pq.push(pre->next);
        }

        ListNode* head = dummy->next;
        delete dummy;
        return head;
    }
};
