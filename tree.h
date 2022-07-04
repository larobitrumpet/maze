#ifndef TREE_H
#define TREE_H

typedef struct TREE
{
    POINT value;
    struct TREE* parent;
    struct TREE* left_child;
    struct TREE* right_child;

} TREE;

TREE* construct_TREE(POINT value);
void deconstruct_TREE(TREE* tree);
TREE* get_root(TREE* tree);
void tree_insert(TREE* tree_root1, TREE* tree_root2);
void tree_union(TREE* tree1, TREE* tree2);
unsigned char in_same_tree(TREE* tree1, TREE* tree2);

#endif
