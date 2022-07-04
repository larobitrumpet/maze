#include "tree.h"

TREE* construct_TREE(POINT value)
{
    TREE* tree = (TREE*)malloc(sizeof(TREE));
    tree->value = value;
    tree->parent = NULL;
    tree->left_child = NULL;
    tree->right_child = NULL;
}

void deconstruct_TREE(TREE* tree)
{
    if (tree == NULL)
        return;
    deconstruct_TREE(tree->left_child);
    deconstruct_TREE(tree->right_child);
    free(tree);
}

TREE* get_root(TREE* tree)
{
    TREE* node = tree;
    while (node->parent != NULL)
        node = node->parent;
    return node;
}

void tree_insert(TREE* tree_root1, TREE* tree_root2)
{
    TREE* node = tree_root1;
    while (1)
    {
        if (node->left_child == NULL)
        {
            node->left_child = tree_root2;
            tree_root2->parent = node;
            return;
        }
        if (node->right_child == NULL)
        {
            node->right_child = tree_root2;
            tree_root2->parent = node;
            return;
        }
        if (between(0, 2))
            node = node->right_child;
        else
            node = node->left_child;
    }
}

void tree_union(TREE* tree1, TREE* tree2)
{
    TREE* root1 = get_root(tree1);
    TREE* root2 = get_root(tree2);
    tree_insert(root1, root2);
}

unsigned char in_same_tree(TREE* tree1, TREE* tree2)
{
    TREE* root1 = get_root(tree1);
    TREE* root2 = get_root(tree2);
    return root1 == root2;
}
