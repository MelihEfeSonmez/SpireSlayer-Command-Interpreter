#include "parser.h"

#include <ctype.h>
#include <string.h>
#include <stddef.h>

// ---FUNCTIONS---

// Function: check whether the integer is valid
int is_valid_positive_integer(const char *str) {

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

    return 1;
}

// Function: tokenize the line and return number of token
int tokenize_line(const char *line, char tokens[MAX_TOKENS][MAX_LINE_LENGTH], int spaces_before[MAX_TOKENS]) {
    int count = 0;
    const char *p = line; // Pointer

    while (*p != '\0') {
        int space_count = 0;
        
        // Count and skip any spaces before the next word
        while (*p == ' ') { 
            space_count++; 
            p++; 
        }
        
        // If we hit the end of the string after skipping spaces, exit the loop
        if (*p == '\0') break;
        
        // Record how many spaces preceded this specific token
        if (count < MAX_TOKENS) { 
            spaces_before[count] = space_count; 
        }
        
        // Determine the length of the current
        int len = 0;
        while (p[len] != '\0' && p[len] != ' ') { 
            len++; 
        }
        
        // Copy the token into our array and add the null-terminator
        if (count < MAX_TOKENS) { 
            strncpy(tokens[count], p, len); 
            tokens[count][len] = '\0'; 
            count++; 
        } 
        
        // Advance the pointer past the word that copied
        p += len; 
    }
    
    return count; 
}

// Function: concatenate the tokens as a name
void extract_name(char tokens[MAX_TOKENS][MAX_LINE_LENGTH], int start_index, int end_index, char *extracted_name) {
    extracted_name[0] = '\0'; // clean for strcat

    for (int i = start_index; i <= end_index; i++) {
        strcat(extracted_name, tokens[i]);

        if (i< end_index) {
            strcat(extracted_name, " ");
        }
    }
}

// Function: check whether the name satisfies all formatting and keyword rules
int is_valid_name(char tokens[MAX_TOKENS][MAX_LINE_LENGTH], int spaces_before[MAX_TOKENS], int start_index, int end_index) {
    
    // Must consist of one or more words
    if (start_index > end_index) {
        return 0; 
    }

    // List of reserved keywords that cannot be used in a name
    const char *reserved[] = {
        "Ironclad", "gains", "gold", "max", "hp", "card", "relic", "potion", "buys", "for", "removes", "upgraded", "upgrades", "enters", "room", 
        "learns", "is", "effective", "against", "fights", "heals", "takes", "damage", "discards", "sells", "marks", "as", "exhaust", "Total", 
        "Floor", "Where", "Deck", "size", "Relics", "Potions", "What", "Defeated", "Health", "Exhausts", "Exit"
    };

    for (int i = start_index; i <= end_index; i++) {
        
        // Consecutive words must be separated by exactly one space
        if (i > start_index && spaces_before[i] != 1) {
            return 0;
        }

        // Each word must consist solely of English
        for (int j = 0; tokens[i][j] != '\0'; j++) {
            if (!isalpha(tokens[i][j])) {
                return 0;
            }
        }

        // No word in the name may be a reserved keyword
        for (int k = 0; k < 40; k++) {
            if (strcmp(tokens[i], reserved[k]) == 0) {
                return 0;
            }
        }
    }

    return 1; // Passed all checks
}
