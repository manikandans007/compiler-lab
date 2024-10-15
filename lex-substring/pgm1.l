%{
#include <stdio.h>
#include <string.h>

// Define a buffer to store the name and its first part
char name[100];
char prefix[100]; // This will store the first part of the name

// Function to recognize a string that does not contain the prefix
void recognize_string(const char *input_string) {
    if (strstr(input_string, prefix) == NULL) {
        printf("%s is recognized\n", input_string);
    } else {
        printf("%s is not recognized\n", input_string);
    }
}

%}

%%
[A-Za-z]+  { recognize_string(yytext); }  // Match alphabetic strings

\n         { /* Ignore newlines */ }

.          { /* Ignore any other character */ }

%%

int main() {
    printf("Enter the name: ");
    fgets(name, 100, stdin);  // Input the name
    name[strcspn(name, "\n")] = 0; // Remove the newline character at the end

    int prefix_len;
    prefix_len=4;

    // Extract the first 'prefix_len' characters from the name
    strncpy(prefix, name, prefix_len);
    prefix[prefix_len] = '\0'; // Add null terminator to the prefix

    printf("Enter input strings (Ctrl+D to stop):\n");
    yylex();  // Start lexical analysis
    return 0;
}

int yywrap() {
    return 1;
}

