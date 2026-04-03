#ifndef PARSER_H
#define PARSER_H

#define MAX_TOKENS 513
#define MAX_LINE_LENGTH 1026

// ---FUNCTION PROTOTYPES---

// Function: 
int is_valid_positive_integer(const char *str);

// Function: 
int tokenize_line(const char *line, char tokens[MAX_TOKENS][MAX_LINE_LENGTH], int spaces_before[MAX_TOKENS]);

// Function: 
void extract_name(char tokens[MAX_TOKENS][MAX_LINE_LENGTH], int start_index, int end_index, char *extracted_name);

#endif