#include "state.h"

#include <string.h>
#include <stddef.h>

// ---FUNCTIONS---

// Function: initialize the game state with starting conditions
void init_game_state(GameState *state) {
    state->gold = 0;
    state->current_hp = 80;
    state->max_hp = 80;
    state->floor = 0;
    strcpy(state->current_room, "NONE");

    // deck
    state->deck = NULL;
    state->deck_count = 0;
    state->deck_capacity = 0;
    // relics
    state->relics = NULL;
    state->relic_count = 0;
    state->relic_capacity = 0;
    // potions
    state->potions = NULL;
    state->potion_count = 0;
    state->potion_capacity = 0;
    // codex
    state->codex = NULL;
    state->codex_count = 0;
    state->codex_capacity = 0;
    // defeated_log
    state->defeated_log = NULL;
    state->defeated_count = 0;
    state->defeated_capacity = 0;
    // exhaust_tags
    state->exhaust_tags = NULL;
    state->exhaust_count = 0;
    state->exhaust_capacity = 0;
}