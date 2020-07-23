#ifndef STRING_H
#define STRING_H
#include "string.h"
#endif

char operators[][2] = {{'*', '/'}, {'+', '-'}};

// Get least precedent operation. This function
// is necessary to split the main equation into
// smaller bits so we can dispose these parts
// along the tree data structure and calculate
// most precedent parts first.
int get_last_operation(char equation[]) {
    // How much precedence levels there are...
    const int size = sizeof operators / sizeof operators[0];
    // ...and the array to keep track of the char positions
    int positions[size];
    
    // Initialize the positions to 'invalid'
    int i;
    for(i = 0; i < size; i++) positions[i] = -1;
    
    // Loop backwards through the equation
    // (we want to get the least precedent,
    // so that makes sense).
    for(i = strlen(equation) - 1; i >= 0; i--) {
        // If the char is a whitespace...
        if(equation[i] == ' '
            || equation[i] == '\0') continue;
        // ...or a number, jump to next iteration
        if(equation[i] >= '0'
            && equation[i] <= '9') continue;
        // If it's possibly an operator, iterate through all
        // of them, checking which precedence level it has
        int j, k, l = 0;
        for(j = 0; j < size && !l; j++) {
            // If that operator has been seen previously,
            // jump to the next iteration
            if(positions[j] != -1) continue;
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
                        l = 1;
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
        if(positions[i] != -1) return positions[i];

    // If it has none, then it isn't an
    // equation, so return -1
    return -1;
}