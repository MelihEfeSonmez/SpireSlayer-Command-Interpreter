#ifndef QUERIES_H
#define QUERIES_H

#include "state.h"

// ---FUNCTION PROTOTYPES--- (II) question—a read-only query)

void execute_gold_query(GameState *state);
void execute_floor_query(GameState *state);
void execute_where_query(GameState *state);
void execute_total_card_query(GameState *state, const char *card_name);
void execute_total_upgraded_card_query(GameState *state, const char *card_name);
void execute_deck_query(GameState *state);
void execute_relics_query(GameState *state);
void execute_potions_query(GameState *state);
void execute_effective_query(GameState *state, const char *enemy_name);
void execute_defeated_query(GameState *state, const char *enemy_name);
void execute_health_query(GameState *state);
void execute_deck_size_query(GameState *state);
void execute_exhausts_query(GameState *state);

#endif