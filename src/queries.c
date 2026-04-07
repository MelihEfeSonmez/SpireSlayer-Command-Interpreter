#include "queries.h"
#include "parser.h"

#include <stdio.h>
#include <string.h>

// --- FUNCTIONS (Read-only Queries) ---

// Function: Print total gold
void execute_gold_query(GameState *state) {
    printf("» %d\n", state->gold);
}

// Function: Print current floor
void execute_floor_query(GameState *state) {
    printf("» %d\n", state->floor);
}

// Function: Print current room
void execute_where_query(GameState *state) {
    printf("» %s\n", state->current_room);
}

// Function: Print total count of a specific card
void execute_total_card_query(GameState *state, const char *card_name) {
    int total = 0;
    
    // Count occurrences of the specified card in the deck
    for (int i = 0; i < state->deck_count; i++) {
        if (strcmp(state->deck[i].name, card_name) == 0) {
            total++; 
        }
    }
    
    printf("» %d\n", total);
}

// Function: Print total count of a specific upgraded card
void execute_total_upgraded_card_query(GameState *state, const char *card_name) {
    int total_upgraded = 0;
    
    // Count occurrences where the name matches and the card is upgraded
    for (int i = 0; i < state->deck_count; i++) {
        if (strcmp(state->deck[i].name, card_name) == 0 && state->deck[i].is_upgraded > 0) {
            total_upgraded++;
        }
    }
    
    printf("» %d\n", total_upgraded);
}

// Function: Group, sort, and print the entire deck
void execute_deck_query(GameState *state) {
    
    if (state->deck_count == 0) {
        printf("» None\n");
        return;
    }

    // Structure to track unique card variants and their quantities
    typedef struct {
        char name[MAX_LINE_LENGTH];
        int count;
        int is_upgraded;
        int is_exhaust;
    } UniqueCard;

    UniqueCard unique_cards[MAX_TOKENS];
    int unique_count = 0;

    // Group identical cards together
    for (int i = 0; i < state->deck_count; i++) {
        int found = 0;
        int current_upgrade_status = (state->deck[i].is_upgraded > 0) ? 1 : 0;
        
        // Check if this exact card variant already exists in the unique list
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(unique_cards[j].name, state->deck[i].name) == 0 && unique_cards[j].is_upgraded == current_upgrade_status) {
                unique_cards[j].count++;
                found = 1;
                break;
            }
        }
        
        // If it is a new variant, add it to the unique list
        if (!found) {
            strcpy(unique_cards[unique_count].name, state->deck[i].name);
            unique_cards[unique_count].count = 1;
            unique_cards[unique_count].is_upgraded = current_upgrade_status;
            unique_cards[unique_count].is_exhaust = state->deck[i].is_exhaust;
            unique_count++;
        }
    }

    // Sort cards alphabetically, prioritizing base cards over upgraded ones if names match
    for (int i = 0; i < unique_count - 1; i++) {
        for (int j = i + 1; j < unique_count; j++) {
            
            int should_swap = 0;
            int name_comparison = strcmp(unique_cards[i].name, unique_cards[j].name);
            
            if (name_comparison > 0) {
                should_swap = 1; // Swap for alphabetical order
            } else if (name_comparison == 0) {
                if (unique_cards[i].is_upgraded > unique_cards[j].is_upgraded) {
                    should_swap = 1; // Swap so base card comes before upgraded card
                }
            }

            if (should_swap) {
                UniqueCard temp = unique_cards[i];
                unique_cards[i] = unique_cards[j];
                unique_cards[j] = temp;
            }
        }
    }

    // Print sorted deck with quantity and suffixes ('+' for upgraded, '*' for exhaust)
    printf("» ");
    for (int i = 0; i < unique_count; i++) {
        
        char suffix[3] = "";
        if (unique_cards[i].is_upgraded == 1) strcat(suffix, "+");
        if (unique_cards[i].is_exhaust == 1) strcat(suffix, "*");

        printf("%d %s%s", unique_cards[i].count, unique_cards[i].name, suffix);
        
        if (i < unique_count - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

// Function: Sort and print all owned relics
void execute_relics_query(GameState *state) {
    
    if (state->relic_count == 0) {
        printf("» None\n");
        return;
    }

    char sorted_names[MAX_TOKENS][MAX_LINE_LENGTH];
    
    // Copy relic names to a temporary array
    for (int i = 0; i < state->relic_count; i++) {
        strcpy(sorted_names[i], state->relics[i].name); 
    }

    // Sort the relics alphabetically
    for (int i = 0; i < state->relic_count - 1; i++) {
        for (int j = i + 1; j < state->relic_count; j++) {
            if (strcmp(sorted_names[i], sorted_names[j]) > 0) {
                char temp[MAX_LINE_LENGTH];
                strcpy(temp, sorted_names[i]);
                strcpy(sorted_names[i], sorted_names[j]);
                strcpy(sorted_names[j], temp);
            }
        }
    }

    // Print the sorted relics
    printf("» ");
    for (int i = 0; i < state->relic_count; i++) {
        printf("%s", sorted_names[i]);
        if (i < state->relic_count - 1) printf(", ");
    }
    printf("\n");
}

// Function: Group, sort, and print potions
void execute_potions_query(GameState *state) {
    
    if (state->potion_count == 0) {
        printf("» None\n");
        return;
    }

    // Structure to track potion types and their quantities
    typedef struct {
        char name[MAX_LINE_LENGTH];
        int count;
    } UniquePotion;

    UniquePotion unique_potions[MAX_TOKENS];
    int unique_count = 0;

    // Group identical potions together
    for (int i = 0; i < state->potion_count; i++) {
        int found = 0;
        
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(unique_potions[j].name, state->potions[i].name) == 0) {
                unique_potions[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            strcpy(unique_potions[unique_count].name, state->potions[i].name);
            unique_potions[unique_count].count = 1;
            unique_count++;
        }
    }

    // Sort unique potions alphabetically
    for (int i = 0; i < unique_count - 1; i++) {
        for (int j = i + 1; j < unique_count; j++) {
            if (strcmp(unique_potions[i].name, unique_potions[j].name) > 0) {
                UniquePotion temp = unique_potions[i];
                unique_potions[i] = unique_potions[j];
                unique_potions[j] = temp;
            }
        }
    }

    // Print sorted potions with quantities
    printf("» ");
    for (int i = 0; i < unique_count; i++) {
        printf("%d %s", unique_potions[i].count, unique_potions[i].name);
        if (i < unique_count - 1) printf(", ");
    }
    printf("\n");
}

// Function: Find and print effective strategies against a specific enemy
void execute_effective_query(GameState *state, const char *enemy_name) {
    int codex_index = -1;
    
    // Search for the enemy in the codex
    for (int i = 0; i < state->codex_count; i++) {
        if (strcmp(state->codex[i].enemy, enemy_name) == 0) {
            codex_index = i;
            break; 
        }
    }
    
    if (codex_index == -1) {
        printf("» No codex data for %s\n", enemy_name);
        return;
    }
    
    int num_counters = state->codex[codex_index].counter_count;
    char sorted_strings[MAX_TOKENS][MAX_LINE_LENGTH];
    
    // Format the counters into "<type> <name>" strings
    for (int i = 0; i < num_counters; i++) {
        sprintf(sorted_strings[i], "%s %s", state->codex[codex_index].counters[i].type, state->codex[codex_index].counters[i].name);
    }
    
    // Sort the formatted strings alphabetically
    for (int i = 0; i < num_counters - 1; i++) {
        for (int j = i + 1; j < num_counters; j++) {
            if (strcmp(sorted_strings[i], sorted_strings[j]) > 0) {
                char temp[MAX_LINE_LENGTH];
                strcpy(temp, sorted_strings[i]);
                strcpy(sorted_strings[i], sorted_strings[j]);
                strcpy(sorted_strings[j], temp);
            }
        }
    }
    
    // Print the sorted counters
    printf("» ");
    for (int i = 0; i < num_counters; i++) {
        printf("%s", sorted_strings[i]);
        if (i < num_counters - 1) printf(", ");
    }
    printf("\n");
}

// Function: Print defeat count for a specific enemy
void execute_defeated_query(GameState *state, const char *enemy_name) {
    int defeat_count = 0; 
    
    // Search the defeated log for the specific enemy
    for (int i = 0; i < state->defeated_count; i++) {
        if (strcmp(state->defeated_log[i].enemy, enemy_name) == 0) {
            defeat_count = state->defeated_log[i].count;
            break;
        }
    }
    
    printf("» %d\n", defeat_count);
}

// Function: Print current and maximum health
void execute_health_query(GameState *state) {
    printf("» %d/%d\n", state->current_hp, state->max_hp);
}

// Function: Print total deck size
void execute_deck_size_query(GameState *state) {
    printf("» %d\n", state->deck_count);
}

// Function: Sort and print exhaust-tagged elements
void execute_exhausts_query(GameState *state) {
    
    if (state->exhaust_count == 0) {
        printf("» None\n");
        return;
    }

    char sorted_names[MAX_TOKENS][MAX_LINE_LENGTH];
    
    // Copy exhaust tags to a temporary array
    for (int i = 0; i < state->exhaust_count; i++) {
        strcpy(sorted_names[i], state->exhaust_tags[i]); 
    }

    // Sort the exhaust tags alphabetically
    for (int i = 0; i < state->exhaust_count - 1; i++) {
        for (int j = i + 1; j < state->exhaust_count; j++) {
            if (strcmp(sorted_names[i], sorted_names[j]) > 0) {
                char temp[MAX_LINE_LENGTH];
                strcpy(temp, sorted_names[i]);
                strcpy(sorted_names[i], sorted_names[j]);
                strcpy(sorted_names[j], temp);
            }
        }
    }

    // Print the sorted exhaust tags
    printf("» ");
    for (int i = 0; i < state->exhaust_count; i++) {
        printf("%s", sorted_names[i]);
        if (i < state->exhaust_count - 1) printf(", ");
    }
    printf("\n");
}