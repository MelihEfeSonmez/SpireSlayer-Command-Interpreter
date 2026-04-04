#include "queries.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>

// ---FUNCTIONS--- (II) question—a read-only query)

// Function: Total gold
void execute_gold_query(GameState *state) {
    printf("%d\n", state->gold);
}

// Function: Floor
void execute_floor_query(GameState *state) {
    printf("%d\n", state->floor);
}

// Function: Where
void execute_where_query(GameState *state) {
    printf("%s\n", state->current_room);
}

// Function: Total card
void execute_total_card_query(GameState *state, const char *card_name) {
    int total = 0;
    
    // Iterate through deck
    for (int i = 0; i < state->deck_count; i++) {
        
        // Increment the counter
        if (strcmp(state->deck[i].name, card_name) == 0) {
            total++; 
        }
    }
    
    // Print the total amount
    printf("%d\n", total);
}

// Function: 
void execute_total_upgraded_card_query(GameState *state, const char *card_name) {
    int total_upgraded = 0;
    
    // Iterate through deck
    for (int i = 0; i < state->deck_count; i++) {
        
        // If the name matches and its 'is_upgraded' flag is active
        if (strcmp(state->deck[i].name, card_name) == 0 && state->deck[i].is_upgraded > 0) {
            total_upgraded++;
        }
    }
    
    // Print the upgraded amount
    printf("%d\n", total_upgraded);
}

// Function: 
void execute_deck_query(GameState *state) {

}

// Function: Relics ?
void execute_relics_query(GameState *state) {
    
    // If no relics are owned print "None"
    if (state->relic_count == 0) {
        printf("None\n");
        return;
    }

    // Create a temporary array to hold the names for sorting
    char sorted_names[MAX_TOKENS][MAX_LINE_LENGTH];
    
    // Copy the original names from the state into temporary array
    for (int i = 0; i < state->relic_count; i++) {
        strcpy(sorted_names[i], state->relics[i].name); 
    }

    // Sort the array with simple Bubble sort
    for (int i = 0; i < state->relic_count - 1; i++) {
        for (int j = i + 1; j < state->relic_count; j++) {
            
            // If the first string is alphabetically greater than the second string
            if (strcmp(sorted_names[i], sorted_names[j]) > 0) {
                // Swap them using a temporary string variable
                char temp[MAX_LINE_LENGTH];
                strcpy(temp, sorted_names[i]);
                strcpy(sorted_names[i], sorted_names[j]);
                strcpy(sorted_names[j], temp);
            }
        }
    }

    // Print names separated by ", "
    for (int i = 0; i < state->relic_count; i++) {
        printf("%s", sorted_names[i]);
        
        if (i < state->relic_count - 1) {
            printf(", ");
        }
    }
    
    printf("\n");
}

// Function: Potions ?
void execute_potions_query(GameState *state) {
    
    if (state->potion_count == 0) {
        printf("None\n");
        return;
    }

    // Create a temporary structure to hold quantities
    typedef struct {
        char name[MAX_LINE_LENGTH];
        int count;
    } UniquePotion;

    UniquePotion unique_potions[MAX_TOKENS];
    int unique_count = 0;

    // Group the potions from the state
    for (int i = 0; i < state->potion_count; i++) {
        int found = 0;
        
        // Check if we already added this potion to our unique list
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(unique_potions[j].name, state->potions[i].name) == 0) {
                unique_potions[j].count++; // Increase the counter
                found = 1;
                break;
            }
        }
        
        // New potion type add it to the unique list with count 1
        if (!found) {
            strcpy(unique_potions[unique_count].name, state->potions[i].name);
            unique_potions[unique_count].count = 1;
            unique_count++;
        }
    }

    // Sort the list with Bubble sort
    for (int i = 0; i < unique_count - 1; i++) {
        for (int j = i + 1; j < unique_count; j++) {
            
            // If the first string is alphabetically greater than the second
            if (strcmp(unique_potions[i].name, unique_potions[j].name) > 0) {
                // Swap the entire struct (both name and count)
                UniquePotion temp = unique_potions[i];
                unique_potions[i] = unique_potions[j];
                unique_potions[j] = temp;
            }
        }
    }

    // SPrint the sorted output in "<count> <name>" format
    for (int i = 0; i < unique_count; i++) {
        printf("%d %s", unique_potions[i].count, unique_potions[i].name);
        
        if (i < unique_count - 1) {
            printf(", ");
        }
    }
    
    printf("\n");
}

// Function: i) What is effective against <enemy> ?
void execute_effective_query(GameState *state, const char *enemy_name) {
    int codex_index = -1;
    
    // Find the enemy in the codex
    for (int i = 0; i < state->codex_count; i++) {
        if (strcmp(state->codex[i].enemy, enemy_name) == 0) {
            codex_index = i;
            break; // Enemy found save the index
        }
    }
    
    // If no codex entry exists for this enemy
    if (codex_index == -1) {
        printf("No codex data for %s\n", enemy_name);
        return;
    }
    
    int num_counters = state->codex[codex_index].counter_count;
    
    // Create a temporary array to hold "<type> <name>"
    char sorted_strings[MAX_TOKENS][MAX_LINE_LENGTH];
    
    // Build the full printed strings
    for (int i = 0; i < num_counters; i++) {
        sprintf(sorted_strings[i], "%s %s", state->codex[codex_index].counters[i].type, state->codex[codex_index].counters[i].name);
    }
    
    // Sort the strings using Bubble sort
    for (int i = 0; i < num_counters - 1; i++) {
        for (int j = i + 1; j < num_counters; j++) {
            
            // Compare the constructed strings
            if (strcmp(sorted_strings[i], sorted_strings[j]) > 0) {
                char temp[MAX_LINE_LENGTH];
                strcpy(temp, sorted_strings[i]);
                strcpy(sorted_strings[i], sorted_strings[j]);
                strcpy(sorted_strings[j], temp);
            }
        }
    }
    
    // Print the sorted counters separated by ", "
    for (int i = 0; i < num_counters; i++) {
        printf("%s", sorted_strings[i]);
        
        if (i < num_counters - 1) {
            printf(", ");
        }
    }
    
    printf("\n");
}

// Function: 
void execute_defeated_query(GameState *state, const char *enemy_name) {
    int defeat_count = 0; // Initial value
    
    for (int i = 0; i < state->defeated_count; i++) {
        // Comparing names
        if (strcmp(state->defeated_log[i].enemy, enemy_name) == 0) {
            defeat_count = state->defeated_log[i].count; // Find matches
            break;
        }
    }
    
    printf("%d\n", defeat_count);
}

// Function: Health
void execute_health_query(GameState *state) {
    printf("%d/%d\n", state->current_hp, state->max_hp);
}

// Function: Deck size
void execute_deck_size_query(GameState *state) {
    printf("%d\n", state->deck_count);
}

// Function: 
void execute_exhausts_query(GameState *state) {
    
    // If no cards are tagged as exhaust print "None"
    if (state->exhaust_count == 0) {
        printf("None\n");
        return;
    }

    // Create a temporary array to hold the names for sorting
    char sorted_names[MAX_TOKENS][MAX_LINE_LENGTH];
    
    // Copy the original names from exhaust_tags into temporary array
    for (int i = 0; i < state->exhaust_count; i++) {
        // exhaust_tags[i] is directly a string
        strcpy(sorted_names[i], state->exhaust_tags[i]); 
    }

    // Sort the array with Bubble sort 
    for (int i = 0; i < state->exhaust_count - 1; i++) {
        for (int j = i + 1; j < state->exhaust_count; j++) {
            
            // If the first string is alphabetically greater than the second string
            if (strcmp(sorted_names[i], sorted_names[j]) > 0) {
                // Swap them using a temporary string variable
                char temp[MAX_LINE_LENGTH];
                strcpy(temp, sorted_names[i]);
                strcpy(sorted_names[i], sorted_names[j]);
                strcpy(sorted_names[j], temp);
            }
        }
    }

    // Print the sorted names separated by ", "
    for (int i = 0; i < state->exhaust_count; i++) {
        printf("%s", sorted_names[i]);
        
        // Print a comma and a space if it is not the last element
        if (i < state->exhaust_count - 1) {
            printf(", ");
        }
    }
    
    printf("\n");
}
