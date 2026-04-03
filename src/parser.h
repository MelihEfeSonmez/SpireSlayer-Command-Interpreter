#ifndef PARSER_H
#define PARSER_H

#define MAX_TOKENS 300
#define MAX_TOKEN_LEN 100

// ---FUNCTION PROTOTYPES---

// Function: 
int is_valid_integer(const char *str);

// Function: 
int tokenize_line(const char *line, char tokens[MAX_TOKENS][MAX_TOKEN_LEN]);

// Function: 
void join_tokens(char tokens[MAX_TOKENS][MAX_TOKEN_LEN], int start_index, int end_index, char *joined_tokens);

#endif