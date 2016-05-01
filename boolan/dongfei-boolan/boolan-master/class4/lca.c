TreeNode *commonAncestor(TreeNode *root, TreeNode *p, TreeNode *q) {
    if (covers(root->left, p) && covers(root->left, q)) {
        return commonAncestor(root->left, q, p);
    }

    if (covers(root->right, p) && covers(root->right, q)) {
        return commonAncestor(root->right, q, p);
    }
    return root;
}

bool covers(TreeNode *root, TreeNode *p) {
    if (root == NULL) {
        return false;
    }

    if (root == p) {
        return true;
    }
    
    return covers(root->left, p) || covers(root->right, p);
}

// bottom to top

TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *A, TreeNode *B) {
    // return either A or B or NULL
    if (NULL == root || root == A || root == B) return root;

    TreeNode *left = lowestCommonAncestor(root->left, A, B);
    TreeNode *right = lowestCommonAncestor(root->right, A, B);

    // A and B are on both sides
    if ((NULL != left) && (NULL != right)) 
    	return root;

    // either left or right or NULL
    return (NULL != left) ? left : right;
}
