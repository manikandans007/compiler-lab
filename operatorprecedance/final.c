#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *input;
int i = 0;
char lasthandle[6], stack[50], handles[][5] = {")E(", "E*E", "E+E", "i", "E^E"};
int top = 0, l;

char prec[9][9] = {
    /*input*/
    /*stack    +    -   *   /   ^   i   (   )   $  */
    /*  + */  '>', '>', '<', '<', '<', '<', '<', '>', '>',
    /*  - */  '>', '>', '<', '<', '<', '<', '<', '>', '>',
    /*  * */  '>', '>', '>', '>', '<', '<', '<', '>', '>',
    /*  / */  '>', '>', '>', '>', '<', '<', '<', '>', '>',
    /*  ^ */  '>', '>', '>', '>', '<', '<', '<', '>', '>',
    /*  i */  '>', '>', '>', '>', '>', 'e', 'e', '>', '>',
    /*  ( */  '<', '<', '<', '<', '<', '<', '<', '>', 'e',
    /*  ) */  '>', '>', '>', '>', '>', 'e', 'e', '>', '>',
    /*  $ */  '<', '<', '<', '<', '<', '<', '<', '<', '>',
};

int getindex(char c) {
    switch (c) {
        case '+': return 0;
        case '-': return 1;
        case '*': return 2;
        case '/': return 3;
        case '^': return 4;
        case 'i': return 5;
        case '(': return 6;
        case ')': return 7;
        case '$': return 8;
        default: return -1;  // Return -1 for undefined characters
    }
}

int shift() {
    stack[++top] = *(input + i++);
    stack[top + 1] = '\0';
    return 1;  // Indicate success
}

int reduce() {
    int len, found, t;
    for (int i = 0; i < 5; i++) {  // Selecting handles
        len = strlen(handles[i]);
        if (stack[top] == handles[i][0] && top + 1 >= len) {
            found = 1;
            for (t = 0; t < len; t++) {
                if (stack[top - t] != handles[i][t]) {
                    found = 0;
                    break;
                }
            }
            if (found) {
                stack[top - t + 1] = 'E';
                top = top - t + 1;
                strcpy(lasthandle, handles[i]);
                stack[top + 1] = '\0';
                return 1;  // Successful reduction
            }
        }
    }
    return 0;
}

void dispstack() {
    for (int j = 0; j <= top; j++)
        printf("%c", stack[j]);
}

void dispinput() {
    for (int j = i; j < l; j++)
        printf("%c", *(input + j));
}

int main() {
    input = (char *)malloc(51 * sizeof(char));  // Allocate 51 for null terminator
    printf("\nEnter the string\n");
    scanf("%s", input);
    strcat(input, "$");  // Concatenate "$" properly
    l = strlen(input);
    strcpy(stack, "$");  // Initialize stack with "$"

    printf("\nSTACK\tINPUT\tACTION");
    
    while (i <= l) {
        shift();
        printf("\n");
        dispstack();
        printf("\t");
        dispinput();
        printf("\tShift");
        
        while (prec[getindex(stack[top])][getindex(input[i])] == '>') {
            while (reduce()) {
                printf("\n");
                dispstack();
                printf("\t");
                dispinput();
                printf("\tReduced: E->%s", lasthandle);
            }
        }
    }

    if (strcmp(stack, "$E$") == 0)
        printf("\nAccepted;");
    else
        printf("\nNot Accepted;");

    free(input);  // Free allocated memory
    return 0;  // Return success
}

