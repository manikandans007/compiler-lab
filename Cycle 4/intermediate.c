#include <stdio.h>
#include <string.h>

// Function to generate intermediate code for a specific operator
void gen_code_for_operator(char *inp, char operator, char *reg) {
    int i = 0, j = 0;
    char temp[100]; // Temporary storage for modified expression

    while (inp[i] != '\0') {
        if (inp[i] == operator) {
            // Print intermediate code for the current operation
            printf("%c\t%c\t%c\t%c\n", operator, *reg, inp[i - 1], inp[i + 1]);

            // Replace the left operand in temp with the current register
            temp[j - 1] = *reg;

            // Skip the operator and the next operand
            i += 2;

            // Move to the next register
            (*reg)--;
            continue;
        }
        // Copy the current character into temp
        temp[j++] = inp[i++];
    }

    // Null-terminate the temporary string
    temp[j] = '\0';

    // Update the input expression with the modified version
    strcpy(inp, temp);
}

// Function to process the expression based on operator precedence
void gen_code(char *inp) {
    char reg = 'Z'; // Start with register Z
    // Process operators in order of precedence
    gen_code_for_operator(inp, '/', &reg);
    gen_code_for_operator(inp, '*', &reg);
    gen_code_for_operator(inp, '+', &reg);
    gen_code_for_operator(inp, '-', &reg);
    gen_code_for_operator(inp, '=', &reg);
}

// Main function
int main() {
    char inp[100];
    printf("Enter expression:\n");
    scanf("%s", inp); // Input the expression
    printf("Op  \tDestn\tArg1\tArg2\n"); // Print column headers
    gen_code(inp); // Generate intermediate code
    return 0;
}


/*
gcc intermediate.c
    ./a.out
    Enter the expression :
p=a*b+c/d=z
    */
