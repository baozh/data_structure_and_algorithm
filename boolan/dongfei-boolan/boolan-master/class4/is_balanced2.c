int isBalancedHelper(TreeNode *root) {
    if (root == NULL) {
       return 0;
    }
    int leftHeight = isBalancedHelper(root->left);
    if (leftHeight == -1) {
        return -1;
    }
    int rightHeight = isBalancedHelper(root->right);
    if (rightHeight == -1) {
        return -1;
    }

    if (abs(leftHeight - rightHeight) > 1) {
        return -1;
    }

    return max(leftHeight, rightHeight) + 1;    // return height
}

bool isBalancedTree(TreeNode *root) {
    return (isBalancedHelper(root) != -1)
}
