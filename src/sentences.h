#ifndef SENTENCES_H
#define SENTENCES_H

#include "state.h"

// ---FUNCTION PROTOTYPES--- (I) sentence—a state-mutating command)

void execute_gain_gold(GameState *state, int amount);
void execute_gain_card(GameState *state, const char *card_name);
void execute_gain_relic(GameState *state, const char *relic_name);
void execute_gain_potion(GameState *state, const char *potion_name);
void execute_buy_card(GameState *state, const char *card_name, int price);
void execute_buy_relic(GameState *state, const char *relic_name, int price);
void execute_buy_potion(GameState *state, const char *potion_name, int price);
void execute_remove_card(GameState *state, const char *card_name);
void execute_remove_upgraded_card(GameState *state, const char *card_name);
void execute_upgrade_card(GameState *state, const char *card_name);
void execute_enter_room(GameState *state, const char *room_type);
void execute_learn_card_effective(GameState *state, const char *card_name, const char *enemy_name);
void execute_learn_relic_effective(GameState *state, const char *relic_name, const char *enemy_name);
void execute_learn_potion_effective(GameState *state, const char *potion_name, const char *enemy_name);
void execute_fight_enemy(GameState *state, const char *enemy_name);
void execute_heal(GameState *state, int amount);
void execute_take_damage(GameState *state, int amount);
void execute_discard_potion(GameState *state, const char *potion_name);
void execute_sell_card(GameState *state, const char *card_name, int price);
void execute_sell_upgraded_card(GameState *state, const char *card_name, int price);
void execute_sell_potion(GameState *state, const char *potion_name, int price);
void execute_mark_exhaust(GameState *state, const char *card_name);
void execute_fight_enemy_bounty(GameState *state, const char *enemy_name, int reward);
void execute_gain_max_hp(GameState *state, int amount);

#endif