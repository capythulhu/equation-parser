#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "constants.h"
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif

// Node for each equation bit
typedef struct _node {
    char            equation[MAX_EQUATION_LENGTH];
    struct _node    *left;
    struct _node    *right;
} node;

// Create a new node
node *new_node(const char equation[]) {
    node *output = malloc(sizeof(node));
    strcpy(output->equation, equation);
    output->left = NULL;
    output->right = NULL;
    return output;
}

// Try to add a child to a tree node
bool add_child(node *parent, node *child) {
    if(!parent->left) {
        parent->left = child;
        return true;
    }
    if(!parent->right) {
        parent->right = child;
        return true;
    }
    return false;
}

// Try to clear the children from a tree node
bool clear_children(node *parent) {
    if(!parent->left) {
        free(parent->left);
        parent->left = NULL;
        return true;
    }
    if(!parent->right) {
        free(parent->right);
        parent->right = NULL;
        return true;
    }
    return false;
}

// Iterate along the tree, from the root or from the leaves
void for_each_node(node *root, void (*function)(node*), bool topToBottom) {
    // Execute the function from the top
    if(topToBottom) function(root);
    if(root->left) for_each_node(root->left, function, topToBottom);
    if(root->right) for_each_node(root->right, function, topToBottom);
    // Execute the function from the bottom
    if(!topToBottom) function(root);
}