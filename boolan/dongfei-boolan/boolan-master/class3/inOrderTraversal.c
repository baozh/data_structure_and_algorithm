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
