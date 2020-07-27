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
    // Allocate the space for the node
    node *output = malloc(sizeof(node));
    // Copy the equation to the new node
    strcpy(output->equation, equation);
    // Clear the children
    output->left = NULL;
    output->right = NULL;
    return output;
}

// Try to add a child to a tree node
bool add_child(node *parent, node *child) {
    // If it hasn't the left child,
    // assign it
    if(!parent->left) {
        parent->left = child;
        return true;
    }
    // If it hasn't the right child,
    // assign it
    if(!parent->right) {
        parent->right = child;
        return true;
    }
    return false;
}

// Try to clear the children from a tree node
bool clear_children(node *parent) {
    bool hasChildren = false;
    // If it has the left child, free it
    if(!parent->left) {
        free(parent->left);
        parent->left = NULL;
        hasChildren = true;
    }
    // If it has the right child, free it
    if(!parent->right) {
        free(parent->right);
        parent->right = NULL;
        hasChildren = true;
    }
    return hasChildren;
}

// Iterate along the tree, from the root or from the leaves
void for_each_node(node *root, void (*function)(node*), bool topToBottom) {
    // Execute the function from the top
    if(topToBottom) function(root);
    // Continue the loop first to the left,
    // and then to the right
    if(root->left) for_each_node(root->left, function, topToBottom);
    if(root->right) for_each_node(root->right, function, topToBottom);
    // Execute the function from the bottom
    if(!topToBottom) function(root);
}