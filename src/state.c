#include "state.h"

#include <string.h>
#include <stddef.h>

// Function: initialize the game state with starting conditions
void init_game_state(GameState *state) {
    state->gold = 0;
    state->current_hp = 80;
    state->max_hp = 80;
    state->floor = 0;
    strcpy(state->current_room, "NONE");

    // deck
    state->deck_capacity = 10;
    state->deck = malloc(state->deck_capacity * sizeof(CardEntry));
    state->deck_count = 0;

    // relics
    state->relic_capacity = 10;
    state->relics = malloc(state->relic_capacity * sizeof(RelicEntry));
    state->relic_count = 0;

    // potions
    state->potion_capacity = 3;
    state->potions = malloc(state->potion_capacity * sizeof(PotionEntry));
    state->potion_count = 0;

    // codex
    state->codex_capacity = 10;
    state->codex = malloc(state->codex_capacity * sizeof(CodexEntry));
    state->codex_count = 0;

    // defeated_log
    state->defeated_capacity = 10;
    state->defeated_log = malloc(state->defeated_capacity * sizeof(DefeatedEntry));
    state->defeated_count = 0;

    // exhaust_tags
    state->exhaust_capacity = 10;
    state->exhaust_tags = malloc(state->exhaust_capacity * sizeof(char*));
    state->exhaust_count = 0;
}

// Function: free all dynamically allocated memory in the state
void free_game_state(GameState *state) {
    
    // free deck
    for (int i = 0; i < state->deck_count; i++) {
        free(state->deck[i].name);
    }
    free(state->deck);

    // free relics
    for (int i = 0; i < state->relic_count; i++) {
        free(state->relics[i].name);
    }
    free(state->relics);

    // free potions
    for (int i = 0; i < state->potion_count; i++) {
        free(state->potions[i].name);
    }
    free(state->potions);

    // free codex
    for (int i = 0; i < state->codex_count; i++) {
        free(state->codex[i].enemy);
        
        // free counters
        for (int j = 0; j < state->codex[i].counter_count; j++) {
            free(state->codex[i].counters[j].type);
            free(state->codex[i].counters[j].name);
        }
        free(state->codex[i].counters);
    }
    free(state->codex);

    // free defeated log
    for (int i = 0; i < state->defeated_count; i++) {
        free(state->defeated_log[i].enemy);
    }
    free(state->defeated_log);

    // free exhaust tags
    for (int i = 0; i < state->exhaust_count; i++) {
        free(state->exhaust_tags[i]);
    }
    free(state->exhaust_tags);
}