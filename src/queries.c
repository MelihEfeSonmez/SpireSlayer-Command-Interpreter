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

// Function: 
void execute_potions_query(GameState *state) {

}

// Function: 
void execute_effective_query(GameState *state, const char *enemy_name) {

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
