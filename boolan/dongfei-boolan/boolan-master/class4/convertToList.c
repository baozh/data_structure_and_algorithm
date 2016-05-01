TreeNode *convertToList(TreeNode *root){
    if (!root)   return NULL;

    TreeNode *leftTail = convertToList(root->left);
    TreeNode *rightTail = convertToList(root->right);

    if (leftTail) {
        leftTail->right = root->right;
    }
    	
    if (root->left) {
        root->right = root->left;
    }
    
    root->left = NULL;

    if (rightTail) {
       return rightTail; 
    } else if(leftTail) {
       return leftTail;  
    } else {
        return root;
    }	
}
