class Solution {
public:
    /**
     * @param head: The first node of linked list.
     * @return: void
     */
    void reorderList(ListNode *head) {
        if (NULL == head || NULL == head->next || NULL == head->next->next) {
            return;
        }

        ListNode *middle = findMiddle(head);
        ListNode *right = reverse(middle->next);
        middle->next = NULL;

        merge(head, right);
    }

private:
    void merge(ListNode *left, ListNode *right) {
        ListNode *dummy = new ListNode(0);
        while (NULL != left && NULL != right) {
            dummy->next = left;
            left = left->next;
            dummy = dummy->next;
            dummy->next = right;
            right = right->next;
            dummy = dummy->next;
        }

        dummy->next = (NULL != left) ? left : right;
        //delete dummy; /* bug, delete the tail node */
    }

    ListNode *reverse(ListNode *head) {
        ListNode *newHead = NULL;
        while (NULL != head) {
            ListNode *temp = head->next;
            head->next = newHead;
            newHead = head;
            head = temp;
        }

        return newHead;
    }

    ListNode *findMiddle(ListNode *head) {
        if (NULL == head || NULL == head->next) {
            return head;
        }

        ListNode *slow = head, *fast = head->next;
        while (NULL != fast && NULL != fast->next) {
            fast = fast->next->next;
            slow = slow->next;
        }

        return slow;
    }
};