TreeNode *descend(TreeNode *node, int level) {
    while (node && level > 0) {
        if (node->left) {
            node = node->left;
        } else if (node->right) {
            node = node->right;
        } else {
            node = NULL;
        }
        level--;
    }
    return node;
}

TreeNode *findRightNeighbor(TreeNode *node) {
    int level = 0;
    TreeNode *parent = node->parent;

    if (!node) {
        return NULL;
    }

    while (parent != NULL) {
        if (isLeftChild(node, parent) && parent->right) {
            break;
        }
        level++;
        node = parent;
        parent = node->parent;
    }

    if (parent == NULL) {
        return NULL;
    } else {
        return descend(parent->right, level);
    }
}
