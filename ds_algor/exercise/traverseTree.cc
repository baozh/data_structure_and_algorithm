//前序遍历
void preOrderTraversal(TreeNode *root) {
    if (!root) {
        return;
    }

    visit(root);

    preOrderTraversal(root->left);
    preOrderTraversal(root->right);
}


// An iterative process to print preorder traversal of Binary tree
void iterativePreorder(node *root) 
{
    // Base Case
    if (root == NULL)
       return;
 
    // Create an empty stack and push root to it
    stack<node *> nodeStack;
    nodeStack.push(root);
 
    /* Pop all items one by one. Do following for every popped item
       a) print it
       b) push its right child
       c) push its left child
    Note that right child is pushed first so that left is processed first */
    while (nodeStack.empty() == false) 
    {
        // Pop the top item from stack and print it
        struct node *node = nodeStack.top();
        printf ("%d ", node->data);
        nodeStack.pop();
 
        // Push right and left children of the popped node to stack
        if (node->right)
            nodeStack.push(node->right);
        if (node->left)
            nodeStack.push(node->left);
    }
}


//中序遍历
void inOrderTraversal(TreeNode *root) {
    if (!root) {
        return;
    }

    inOrderTraversal(root->right);
    visit(root);
    inOrderTraversal(root->left);
}

void inorderTraversal(TreeNode *root) {
    if(root == NULL)
        return;
    stack<TreeNode *> st;
    while (!st.empty() || root )
    {
        /* The 3 Nodes: root, left and right can actually be regarded as 3 subtrees( same type ), though we did little optimization here so we only push 1 subtree instead of 3 at one time( always the root ).*/
        if (root) 
        {
            st.push(root);
            root = root->left;
        } 
        else 
        {
            root = st.top();
            st.pop();
            visit( root );
            root = root->right;
        }
    }
}

//后序遍历
void postOrderTraversal(TreeNode *root) {
    if (!root) {
        return;
    }

    postOrderTraversal(root->left);
    postOrderTraversal(root->right);
    visit(root);
}

//层次遍历
void levelTraversal(TreeNode *root) 
{
    queue<TreeNode *> nodeQueue;
    TreeNode *currentNode;
    if (!root) 
    {
        return;
    }
    nodeQueue.push(root);

    while (!nodeQueue.empty()) 
    {
        currentNode = nodeQueue.front();
        processNode(currentNode);
        if (currentNode->left) 
        {
            nodeQueue.push(currentNode->left);
        }
        
        if (currentNode->right) 
        {
            nodeQueue.push(currentNode->right);
        }
        nodeQueue.pop();
    }
}
















