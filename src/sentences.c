#include "sentences.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---GENERAL HELPER FUNCTIONS---

// Helper Function: remove card by index
void remove_card(GameState *state, int index) {
    if (index < 0 || index >= state->deck_count) {
        return;
    }

    for (int i = index; i < state->deck_count - 1; i++) {
        state->deck[i] = state->deck[i + 1];
    }

    state->deck_count--;
}

// Helper Function: remove potion by index
void remove_potion(GameState *state, int index) {
    if (index < 0 || index >= state->potion_count) {
        return;
    }

    for (int i = index; i < state->potion_count - 1; i++) {
        state->potions[i] = state->potions[i + 1];
    }

    state->potion_count--;
}

// Helper Function: check global exhaust list
int check_if_exhaust(GameState *state, const char *card_name) {
    for (int i = 0; i < state->exhaust_count; i++) {
        
        if(strcmp(state->exhaust_tags[i], card_name) == 0) {
            return 1;
        }
    }

    return 0;
}

// ---FUNCTIONS--- (I) sentence—a state-mutating command)

// Function: Increase total gold by the specified amount. 
void execute_gain_gold(GameState *state, int amount) {
    state->gold += amount;
    printf("Gold obtained");
}

// Function: One base copy of the named card is added to the deck.
void execute_gain_card(GameState *state, const char *card_name) {
    strcpy(state->deck[state->deck_count].name, card_name);

    state->deck[state->deck_count].is_upgraded = 0;
    state->deck[state->deck_count].is_exhaust = check_if_exhaust(state, card_name);
    state->deck_count++;

    printf("Card added: %s\n", card_name);
}

// Function: If the relic is not yet owned, add it. If already owned, nothing changes.
void execute_gain_relic(GameState *state, const char *relic_name) {
    
    for (size_t i = 0; i < state->relic_count; i++) {

        if (strcmp(state->relics[i].name, relic_name) == 0) {
            printf("Already has relic: %s\n", relic_name);            
            return;
        }
    }
    
    strcpy(state->relics[state->relic_count].name, relic_name);
    state->relic_count++;

    printf("Relic obtained: %s\n", relic_name);
}

// Function: If the belt has room, add one copy. Otherwise, refuse.
void execute_gain_potion(GameState *state, const char *potion_name) {
    
    if (state->potion_count >= 3 ) {
        printf("Potion belt is full\n");
        return;
    }

    strcpy(state->potions[state->potion_count].name, potion_name);
    state->potion_count++;

    printf("Potion obtained: %s\n", potion_name);
}

// Function: Deduct gold and apply the gain by considering "evaluation order".
void execute_buy_card(GameState *state, const char *card_name, int price) {
    // 1) check gold
    if (state->gold < price) { 
        printf("Not enough gold\n");
    }

    // 3) deduct gold and apply gain
    state->gold -= price;

    strcpy(state->deck[state->deck_count].name, card_name);

    state->deck[state->deck_count].is_upgraded = 0; 
    state->deck[state->deck_count].is_exhaust = check_if_exhaust(state, card_name);
    state->deck_count++;

    printf("Card added: %s\n", card_name);
}

// Function: Deduct gold and apply the gain. Print the corresponding gain output.
void execute_buy_relic(GameState *state, const char *relic_name, int price) {
    // 1) check gold
    if (state->gold < price) {
        printf("Not enough gold\n");
    }

    // 2) check contraints
    for (int i = 0; i < state->relic_count; i++) {
        if (strcmp(state->relics[i].name, relic_name) == 0) {
            printf("Already has relic: %s\n", relic_name);
            return;
        }
    }

    // 3) deduct gold and apply gain
    state->gold -= price;

    strcpy(state->relics[state->relic_count].name, relic_name);
    state->relic_count++;

    printf("Relic obtained: %s\n", relic_name);
}

// Function: Deduct gold and apply the gain. Print the corresponding gain output.
void execute_buy_potion(GameState *state, const char *potion_name, int price) {
    // 1) check gold
    if (state->gold < price) {
        printf("Not enough gold\n");
    }

    // 2) check contraints
    if (state->potion_count >= 3) {
        printf("Potion belt is full\n");
        return;
    }

    // 3) deduct gold and apply gain
    state->gold -= price;

    strcpy(state->potions[state->potion_count].name, potion_name);
    state->potion_count++;

    printf("Potion obtained: %s\n", potion_name);
}

// Function: removes one base copy, if present.
void execute_remove_card(GameState *state, const char *card_name) {
    
    for (int i = 0; i < state->deck_count; i++) {
        if (strcmp(state->deck[i].name, card_name) == 0 && state->deck[i].is_upgraded == 0) {
            
            for (int j = i; j < state->deck_count - 1; j++) {
                state->deck[j] = state->deck[j+1];
            }
            state->deck_count--;
            
            printf("Card removed: %s\n", card_name);
            return;
        }
    }
    
    printf("Card not found: %s\n", card_name);
}

// Function: removes one upgraded copy (<card>+), if present.
void execute_remove_upgraded_card(GameState *state, const char *card_name) {
        
    for (int i = 0; i < state->deck_count; i++) {
        if (strcmp(state->deck[i].name, card_name) == 0 && state->deck[i].is_upgraded == 1) {
            
            for (int j = i; j < state->deck_count - 1; j++) {
                state->deck[j] = state->deck[j+1];
            }
            state->deck_count--;
            
            printf("upgraded card removed: %s\n", card_name);
            return;
        }
    }
    
    printf("Card not found: %s\n", card_name);
}

// Function: If at least one base copy is in the deck, convert one base copy into an upgraded copy. Otherwise, nothing changes.
void execute_upgrade_card(GameState *state, const char *card_name) {
    
    for (int i = 0; i < state->deck_count; i++) {

        if (strcmp(state->deck[i].name, card_name) == 0) {
            if (state->deck[i].is_upgraded == 0) {
                state->deck[i].is_upgraded++;

                printf("Card upgraded: %s\n", card_name);
                return;

            } else {
                continue;
            }
        }
    }

    printf("Card not found: %s\n", card_name);
}

// Function: Floor increments by one; current room updates to the given type.
void execute_enter_room(GameState *state, const char *room_type) {
    state->floor++;
    strcpy(state->current_room, room_type);

    printf("Entered %s room\n", room_type);
}

// Helper Funciton: execute record effectiveness
void learn_effective_helper(GameState *state, const char *type, const char *entity_name, const char *enemy_name) {
    int enemy_index = -1;

    for (int i = 0; i < state->codex_count; i++) {
        if (strcmp(state->codex[i].enemy, enemy_name) == 0){
            enemy_index = i;
            break;
        }
    }

    // If the enemy is new to the codex, create an entry first.
    if (enemy_index == -1) {
        enemy_index = state->codex_count;

        strcpy(state->codex[enemy_index].enemy, enemy_name);
        state->codex[enemy_index].counter_count = 0;

        strcpy(state->codex[enemy_index].counters[0].type, type);
        strcpy(state->codex[enemy_index].counters[0].name, entity_name);
        state-> codex[enemy_index].counter_count;
        state->codex_count++;

        printf("Codex entry created: %s\n", enemy_name);
    
    } else if (enemy_index != -1) {

        for (int i = 0; i < state->codex[enemy_index].counter_count; i++) {

            // Duplicate typed counters are silently ignored (no statechange).
            if (strcmp(state->codex[enemy_index].counters[i].type, type) == 0 && state->codex[enemy_index].counters[i].name, entity_name) {
                printf("Effectiveness already noted\n");
                break;
            }
        }

        // update codex entry
        int codex_index = state->codex[enemy_index].counter_count;
        strcpy(state->codex[enemy_index].counters[codex_index].type, type);
        strcpy(state->codex[enemy_index].counters[codex_index].name, entity_name);

        printf("Codex entry updated: %s\n", enemy_name);
    }
}

// Function: Register a typed counter (card) in the codex for the specified enemy
void execute_learn_card_effective(GameState *state, const char *card_name, const char *enemy_name) {
    learn_effective_helper(state, "card", card_name, enemy_name);
}

// Function: Register a typed counter (relic) in the codex for the enemy
void execute_learn_relic_effective(GameState *state, const char *relic_name, const char *enemy_name) {
    learn_effective_helper(state, "relic", relic_name, enemy_name);
}

// Function: Register a typed counter (potion) in the codex for the specified enemy
void execute_learn_potion_effective(GameState *state, const char *potion_name, const char *enemy_name) {
    learn_effective_helper(state, "potion", potion_name, enemy_name);
}

// Function: fight enemy 
void execute_fight_enemy(GameState *state, const char *enemy_name) {
    int is_victor = 0;
    int codex_index = -1;

    for (int i = 0; i < state->codex_count; i++) {
        
        if (strcmp(state->codex[i].enemy, enemy_name) == 0) {
            codex_index = i;
            break;
        }
    }
    
    // victory conditions
    if (codex_index != -1) {
        for (int i = 0; i < state->codex[codex_index].counter_count; i++) {
            char *type = state->codex[codex_index].counters[i].type;
            char *name = state->codex[codex_index].counters[i].name;

            if (strcmp(type, "card") == 0) {
                
                for (int deck_index = 0; deck_index < state->deck_count; deck_index++) {
                    if (strcmp(state->deck[deck_index].name, name) == 0) {
                        is_victor = 1;
                        break;
                    }              
                }
            } else if (strcmp(type, "relic") == 0) {
                
                for (int relic_index = 0; relic_index < state->relic_count; relic_index++) {
                    if (strcmp(state->relics[relic_index].name, name) == 0) {
                        is_victor = 1;
                        break;
                    }              
                }
            } else if (strcmp(type, "potion") == 0) {
                
                for (int potion_index = 0; potion_index < state->potion_count; potion_index++) {
                    if (strcmp(state->potions[potion_index].name, name) == 0) {
                        is_victor = 1;
                        break;
                    }              
                }
            }

            if (is_victor == 1) {
                break;
            }
        }
    }

    // post-victory effects
    if (is_victor == 1) {
        for (int i = 0; i < state->codex[codex_index].counter_count; i++) {
            char *type = state->codex[codex_index].counters[i].type;
            char *name = state->codex[codex_index].counters[i].name;
        
            // 1) For effective potion type, consume exactly one copy.
            if (strcmp(type, "potion") == 0) {
             
                for (int potion_index = 0; potion_index < state->potion_count; potion_index++) {
                    if(strcmp(state->potions[potion_index].name, name) == 0) {

                        for (int j = 0; j < state->potion_count; j++) {
                            state->potions[j] = state->potions[j + 1];
                        }
                        
                        state->potion_capacity--;
                        break;
                    }
                }

            // 2) For effective card type (tagged as exhaust and  at least one copy), remove exactly one copy. 
            // Only remove an upgraded copy if no base copy remains.
            } else if (strcmp(type, "card") == 0) {

                int target_base_index = -1;
                int target_upgraded_index = -1;

                for (int deck_index = 0; deck_index < state->deck_count; deck_index++) {
                    if(strcmp(state->deck[deck_index].name, name) == 0 && state->deck[deck_index].is_exhaust == 1) {

                        if (state->deck[deck_index].is_upgraded == 0) {
                            target_base_index = deck_index;
                            break;
                        } else if (state->deck[deck_index].is_upgraded == 1 && target_upgraded_index == -1) {
                            target_upgraded_index = deck_index;
                        }
                    }
                }

                int index_to_remove = -1;

                if (target_base_index != -1) {
                    index_to_remove = target_base_index;
                } else if (target_upgraded_index != -1) {
                    index_to_remove = target_upgraded_index;
                }
 
                if(index_to_remove != -1) {
                    remove_card(state, index_to_remove);
                }
            }
        }
        int is_enemy_defeated_before = 0;

        for (int i = 0; i < state->defeated_count; i++) {
                
            if (strcmp(state->defeated_log[i].enemy, enemy_name) == 0) {
                state->defeated_log[i].count++;
                is_enemy_defeated_before = 1;
            }
        }

        if ( !(is_enemy_defeated_before == 1) ) {
            strcpy(state->defeated_log[state->defeated_count].enemy, enemy_name);
            state->defeated_log[state->defeated_count].count = 1;
            state->defeated_count++;
        }

        printf("Ironclad defeats %s\n", enemy_name);

    // defeated effects    
    } else {
        state->current_hp -= 15;
        if (state->current_hp < 0) {
            state->current_hp = 0;
        }
        
        printf("Ironclad is outmatched and flees with %d hp remaining\n", state->current_hp);
    }
}

// Function: Increase current HP by the given amount. If the result would exceed Max HP, current HP is clamped to Max HP.
void execute_heal(GameState *state, int amount) {
    state->current_hp += amount;

    if (state->current_hp > state->max_hp) {
        state->current_hp = state->max_hp;
    }

    printf("Ironclad heals to %d\n", state->current_hp);
}

// Function: Decrease current HP by the given amount. If the result would drop below 0, current HP is clamped to 0.
void execute_take_damage(GameState *state, int amount) {
    state->current_hp -= amount;

    if (state->current_hp < 0) {
        state->current_hp = 0;
    }

    printf("Ironclad health drops to %d\n", state->current_hp);  
}

// Function: If the named potion exists in the belt, remove one copy; otherwise, do nothing.
void execute_discard_potion(GameState *state, const char *potion_name) {
    for (int i = 0; i < state->potion_count; i++) {
        
        if (strcmp(state->potions[i].name, potion_name)) {
            remove_potion(state, i);
            state->potions--;

            printf("Potion discarded: %s\n", potion_name);
            return;
        }
    }

    printf("Potion not found: %s\n", potion_name);
}

// Function: If the item (card) exists, remove it and add the given gold amount. Otherwise, nothing changes.
void execute_sell_card(GameState *state, const char *card_name, int price) {
    for (int i = 0; i < state->deck_count; i++) {
        
        if (strcmp(state->deck[i].name, card_name) == 0 && state->deck[i].is_upgraded == 0) {
            state->gold += price;

            remove_card(state, i);

            printf("Card sold: %s\n", card_name);
            return;
        }
    }
    
    printf("Card not found: %s\n", card_name);
}

// Function: If the item (card) exists, remove it and add the given gold amount. Otherwise, nothing changes.
void execute_sell_upgraded_card(GameState *state, const char *card_name, int price) {
    for (int i = 0; i < state->deck_count; i++) {

        if (strcmp(state->deck[i].name, card_name) == 0 && state->deck[i].is_upgraded == 1) {
            state->gold += price;

            remove_card(state, i);

            printf("Upgraded card sold: %s\n", card_name);
            return;
        }
    }
    
    printf("Upgraded card not found: %s\n", card_name);
}

// Function: If the item (card) exists, remove it and add the given gold amount. Otherwise, nothing changes.
void execute_sell_potion(GameState *state, const char *potion_name, int price) {
    for (int i = 0; i < state->potion_count; i++) {

        if (strcmp(state->potions[i].name, potion_name) == 0) {
            state->gold += price;

            remove_potion(state, i);

            printf("Potion sold: %s\n", potion_name);
            return;
        }
    }
    
    printf("Potion not found: %s\n", potion_name);
}

// Function: Tag the given card name as exhaust. The tag is global—regardless of whether any copies currently exist in the deck.
void execute_mark_exhaust(GameState *state, const char *card_name) {
    for (int i = 0; i < state->exhaust_count; i++) {

        if (strcmp(state->exhaust_tags[i], card_name) == 0) {

            printf("Card already exhausts: %s\n", card_name);
            return;
        }
    }

    strcpy(state->exhaust_tags[state->exhaust_count], card_name);
    state->exhaust_count++;

    for (int i = 0; i < state->deck_count; i++) {
        if (strcmp(state->deck[i].name, card_name) == 0) {
            state->deck[i].is_exhaust = 1;
        }
    }
    
    printf("Card marked as exhaust: %s\n", card_name);
}

// Function: Same function with execute_fight_enemy (only difference: the specified gold amount is added)
void execute_fight_enemy_bounty(GameState *state, const char *enemy_name, int reward) {
    int is_victor = 0;
    int codex_index = -1;

    for (int i = 0; i < state->codex_count; i++) {
        
        if (strcmp(state->codex[i].enemy, enemy_name) == 0) {
            codex_index = i;
            break;
        }
    }
    
    // victory conditions
    if (codex_index != -1) {
        for (int i = 0; i < state->codex[codex_index].counter_count; i++) {
            char *type = state->codex[codex_index].counters[i].type;
            char *name = state->codex[codex_index].counters[i].name;

            if (strcmp(type, "card") == 0) {
                
                for (int deck_index = 0; deck_index < state->deck_count; deck_index++) {
                    if (strcmp(state->deck[deck_index].name, name) == 0) {
                        is_victor = 1;
                        break;
                    }              
                }
            } else if (strcmp(type, "relic") == 0) {
                
                for (int relic_index = 0; relic_index < state->relic_count; relic_index++) {
                    if (strcmp(state->relics[relic_index].name, name) == 0) {
                        is_victor = 1;
                        break;
                    }              
                }
            } else if (strcmp(type, "potion") == 0) {
                
                for (int potion_index = 0; potion_index < state->potion_count; potion_index++) {
                    if (strcmp(state->potions[potion_index].name, name) == 0) {
                        is_victor = 1;
                        break;
                    }              
                }
            }

            if (is_victor == 1) {
                break;
            }
        }
    }

    // post-victory effects
    if (is_victor == 1) {
        for (int i = 0; i < state->codex[codex_index].counter_count; i++) {
            char *type = state->codex[codex_index].counters[i].type;
            char *name = state->codex[codex_index].counters[i].name;
        
            // 1) For effective potion type, consume exactly one copy.
            if (strcmp(type, "potion") == 0) {
             
                for (int potion_index = 0; potion_index < state->potion_count; potion_index++) {
                    if(strcmp(state->potions[potion_index].name, name) == 0) {

                        for (int j = 0; j < state->potion_count; j++) {
                            state->potions[j] = state->potions[j + 1];
                        }
                        
                        state->potion_capacity--;
                        break;
                    }
                }

            // 2) For effective card type (tagged as exhaust and  at least one copy), remove exactly one copy. 
            // Only remove an upgraded copy if no base copy remains.
            } else if (strcmp(type, "card") == 0) {

                int target_base_index = -1;
                int target_upgraded_index = -1;

                for (int deck_index = 0; deck_index < state->deck_count; deck_index++) {
                    if(strcmp(state->deck[deck_index].name, name) == 0 && state->deck[deck_index].is_exhaust == 1) {

                        if (state->deck[deck_index].is_upgraded == 0) {
                            target_base_index = deck_index;
                            break;
                        } else if (state->deck[deck_index].is_upgraded == 1 && target_upgraded_index == -1) {
                            target_upgraded_index = deck_index;
                        }
                    }
                }

                int index_to_remove = -1;

                if (target_base_index != -1) {
                    index_to_remove = target_base_index;
                } else if (target_upgraded_index != -1) {
                    index_to_remove = target_upgraded_index;
                }
 
                if(index_to_remove != -1) {
                    remove_card(state, index_to_remove);
                }
            }
        }
        int is_enemy_defeated_before = 0;

        for (int i = 0; i < state->defeated_count; i++) {
                
            if (strcmp(state->defeated_log[i].enemy, enemy_name) == 0) {
                state->defeated_log[i].count++;
                is_enemy_defeated_before = 1;
            }
        }

        if ( !(is_enemy_defeated_before == 1) ) {
            strcpy(state->defeated_log[state->defeated_count].enemy, enemy_name);
            state->defeated_log[state->defeated_count].count = 1;
            state->defeated_count++;
        }

        state->gold += reward; // bounty gain

        printf("Ironclad defeats %s and gains %d gold\n", enemy_name, reward);

    // defeated effects    
    } else {
        state->current_hp -= 15;
        if (state->current_hp < 0) {
            state->current_hp = 0;
        }
        
        printf("Ironclad is outmatched and flees with %d hp remaining\n", state->current_hp);
    }    
}

// Function: Increase Max HP by the given amount.
void execute_gain_max_hp(GameState *state, int amount) {
    state->max_hp += amount;
    printf("Max health increased to %d\n", state->max_hp);
}
