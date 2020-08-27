#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef STBOOL_H
#define STBOOL_H
#include <stdbool.h>
#endif

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "constants.h"
#endif

#ifndef TREE_H
#define TREE_H
#include "tree.h"
#endif

// Operators ordered by their precedence level
char operators[][2] = {{'*', '/'}, {'+', '-'}};

// Check if the whole equation is inside a
// pair of brackets and remove them
bool normalize_precedence_offset(char equation[]) {
    // Get the size of the equation
    const unsigned int size = strlen(equation);
    int i = 0;
    // And skip the whitespaces
    while(equation[i] == ' ') i++;
    // If it finds a parentheses before
    // any other character...
    if(equation[i] == '(') {
        // ...store the index and begin to
        // iterate over the equation again,
        // this time backwards
        int j = i;
        i = size - 1;
        // Skip the whitespaces
        while(equation[i] == ' ') i--;
        // And if it finds a closing parentheses
        // as the last character...
        if(equation[i] == ')') {
            // Remove both parentheses and return
            equation[j] = ' ';
            equation[i] = ' ';
            return true;
        }
    }
    // Or return false if the base offset
    // of this equation isn't greater
    // than zero
    return false;
}

// Get least precedent operation. This function
// is necessary to split the main equation into
// smaller bits so we can dispose these parts
// along the tree data structure and calculate
// most precedent parts first.
int get_last_operation(const char equation[]) {
    // How much precedence levels there are...
    const unsigned int size = sizeof operators / sizeof operators[0];
    // ...and the array to keep track of the char positions
    int positions[size];
    
    // Initialize the positions to 'invalid'
    int i;
    for(i = 0; i < size; i++) positions[i] = -1;
    
    // Initialize the precedence offset
    // (used for precedencies forced by
    // parenthesis)
    int p = 0;

    // Loop backwards through the equation
    // (we want to get the least precedent
    // operation).
    for(i = strlen(equation) - 1; i >= 0; i--) {
        // If it's a parentheses, increase 
        // or decrease the precedence offset
        if(equation[i] == ')') {
            p--;
            continue;
        }
        if(equation[i] == '(') {
            p++;
            continue;
        }
        // If there is any precedence offset...
        if(p != 0) continue;
        // ...or if the char is a whitespace...
        if(equation[i] == ' ') continue;
        // ...or a number, skip to the next iteration
        if(equation[i] >= '0'
            && equation[i] <= '9') continue;
        // But if it's an EOL, break the loop
        if(equation[i] == '\0') break;

        // If it's possibly an operator, iterate through all
        // of them, checking which precedence level it has
        int j, k, l = false;
        for(j = 0; j < size && !l; j++) {
            // If that operator has been seen previously,
            // skip to the next iteration
            if(positions[j] >= 0) continue;
            // Loop through all the operators in that
            // precedence level
            for(k = 0; k < sizeof operators[0]; k++) {
                // If it's one of these operators...
                if(equation[i] == operators[j][k]) {
                    // ...and it's already the least precedent
                    // operator, just return the current index
                    if(j + 1 == size) return i;
                    else {
                        // If it isn't the least precedent, just
                        // fill the array at that index
                        positions[j] = i;
                        // And tell the outer loop that the job
                        // is done
                        l = true;
                        break;
                    }
                }
            }
        }
    }
    // After it's all done, get char index
    // that contains the least precedent
    // operation sign, and return it
    for(i = size - 1; i >= 0; i--)
        if(positions[i] >= 0) return positions[i];

    // If it has none, then it isn't an
    // equation, so return -1
    return -1;
}

// Split a node if it has unsolved math
void split_node(node *n) {
    // Try to normalize the precedence, based
    // on parentheses
    while(normalize_precedence_offset(n->equation));
    // Check if there is still unsolved math
    int operation = get_last_operation(n->equation);
    // If there is...
    if(operation >= 0) {
        // ...start splitting the equation
        // The left node equation first
        char leftEquation[MAX_EQUATION_LENGTH];
        int i = 0;
        // While before the separator, send the
        // characters to the left-side string
        while(i < operation) {
            leftEquation[i] = n->equation[i];
            i++;
        }
        // After it's done, finalize the string...
        leftEquation[i] = '\0';
        // ...and skip the separator index
        i++;
        // Now do the same for the right node
        char rightEquation[MAX_EQUATION_LENGTH];
        int j = i;
        do {
            rightEquation[i - j] = n->equation[i];
        } while(n->equation[i++] != '\0');
        // Create the children and assign the math
        node *left = new_node(leftEquation);
        node *right = new_node(rightEquation);
        // Make the current node equation the
        // current operator and finalize it
        n->equation[0] = n->equation[operation];
        n->equation[1] = '\0';
        // Then assign the children to the node
        add_child(n, left);
        add_child(n, right);
    }
}

// Solve the math contained on a node
void solve_node(node *n) {
    // If the node has both children, solve it
    if(n->left && n->right) {
        // Switch on the operation and properly
        // calculate the operation
        switch(n->equation[0]) {
            // Detect addition
            case '+':
                sprintf(n->equation, "%i",
                atoi(n->left->equation) 
                    + atoi(n->right->equation));     
            break;
            // Detect subtraction
            case '-':
                sprintf(n->equation, "%i",
                atoi(n->left->equation) 
                    - atoi(n->right->equation));
            break;
            // Detect multiplication
            case '*':
                sprintf(n->equation, "%i",
                atoi(n->left->equation) 
                    * atoi(n->right->equation));
            break;
            // Detect division
            case '/':
                sprintf(n->equation, "%i",
                atoi(n->left->equation) 
                    / atoi(n->right->equation));
            break;
        }
        // Clear the children of the current node
        clear_children(n);
    }
}

// Print a node
void print_node(node *n) {
    printf("%s\n", n->equation);
}

// Solve the equation
int solve(const char equation[]) {
    // Create the tree
    node *root = new_node(equation);
    // Split the equation from the root
    for_each_node(root, split_node, true);
    // Start solving it from the leaves
    for_each_node(root, solve_node, false);
    // And print the result
    printf("%s", root->equation);
}