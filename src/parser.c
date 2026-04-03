#include "parser.h"

#include <ctype.h>
#include <string.h>
#include <stddef.h>

// ---FUNCTIONS---

// Function: check whether the integer is valid
int is_valid_integer(const char *str) {

    if (str == NULL || str[0] == '\0') { // null or empty
        return 0;
    }
    if (str[0] == '0') { // no leading zero
        return 0;
    }
    if (str[0] == '+' || str[0] == '-') { // no sign prefix
        return 0;
    }

    for (int i = 0; str[i] != '\0'; i++) { // numeric value
        if (!isdigit(str[i])) {
            return 0;
        }    
    }
    
}

// Function: tokenize the line and return number of token
int tokenize_line(const char *line, char tokens[MAX_TOKENS][MAX_TOKEN_LEN]) {
    int count = 0;
    char temp_line[1026];
    strcopy(temp_line, line);

    char *token = strtok(temp_line, " ");

    while (token != NULL && count < MAX_TOKENS) {
        strcpy(tokens[count], token);
        count++;
        token = strtok(NULL, " ");
    }
    
    return count;
}

// Function: concatenate the tokens as a string
void join_tokens(char tokens[MAX_TOKENS][MAX_TOKEN_LEN], int start_index, int end_index, char *joined_tokens) {
    joined_tokens[0] = '\0'; // clean for strcat

    for (int i = start_index; i <= end_index; i++) {
        strcat(joined_tokens, tokens[i]);

        if (i< end_index) {
            strcat(joined_tokens, " ");
        }
    }
}
