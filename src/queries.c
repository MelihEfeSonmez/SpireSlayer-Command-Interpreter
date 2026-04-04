#include "queries.h"

#include <stdio.h>
#include <string.h>

// ---FUNCTIONS--- (II) question—a read-only query)

// Function: Total gold
void execute_gold_query(GameState *state) {
    printf("%d\n", state->gold);
}

// Funciton: Floor
void execute_floor_query(GameState *state) {
    printf("%d\n", state->floor);
}

// Funciton: Where
void execute_where_query(GameState *state) {
    printf("%s\n", state->current_room);
}

// Funciton: 
void execute_total_card_query(GameState *state, const char *card_name) {

}

// Funciton: 
void execute_total_upgraded_card_query(GameState *state, const char *card_name) {

}

// Funciton: 
void execute_deck_query(GameState *state) {

}

// Funciton: 
void execute_relics_query(GameState *state) {

}

// Funciton: 
void execute_potions_query(GameState *state) {

}

// Funciton: 
void execute_effective_query(GameState *state, const char *enemy_name) {

}

// Funciton: 
void execute_defeated_query(GameState *state, const char *enemy_name) {
    int defeat_count = 0; // 1. Başlangıç durumu
    
    // 2. Log dizisinde gezinme
    for (int i = 0; i < state->defeated_count; i++) {
        // 3. İsimleri karşılaştırma
        if (strcmp(state->defeated_log[i].enemy, enemy_name) == 0) {
            defeat_count = state->defeated_log[i].count; // Eşleşme bulundu
            break; // 4. Döngüyü kırma
        }
    }
    
    // 5. Sonucu yazdırma
    printf("%d\n", defeat_count);
}

// Funciton: Health
void execute_health_query(GameState *state) {
    printf("%d/%d\n", state->current_hp, state->max_hp);
}

// Funciton: Deck size
void execute_deck_size_query(GameState *state) {
    int total_cards = 0;
    
    // Desteyi dön ve her bir kartın base ve upgraded sayılarını toplama ekle
    for (int i = 0; i < state->deck_count; i++) {
        total_cards += state->deck[i].base_count + state->deck[i].upgraded_count;
    }
    
    printf("%d\n", total_cards);
}

// Funciton: 
void execute_exhausts_query(GameState *state) {

}
