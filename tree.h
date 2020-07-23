#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

// Node for each equation bit
typedef struct _node {
    char            equation[1<<7];
    struct _node    *left;
    struct _node    *right;
} node;

// Creates a new node
node *new_node(char equation[]) {
    node *output = malloc(sizeof(node));
    output->equation = equation;
    output->left = NULL;
    output->right = NULL;
    return output;
}