#ifndef STATE_H
#define STATE_H

#include <stdlib.h>

// ---STRUCTURES---

// Structure: card
typedef struct {
    char *name;
    int base_count;
    int upgraded_count;
    int is_exhaust;
} CardEntry;

// Structure: relic
typedef struct {
    char *name;
} RelicEntry;

// Structure: potion
typedef struct {
    char *name;
} PotionEntry;

// Structure: codex counter
typedef struct {
    char *type;
    char *name;
} CodexCounter;

// Structure: codex
typedef struct {
    char *enemy;
    CodexCounter *counters;
    int counter_count;
    int counter_capacity;
} CodexEntry;

// Structure: defeated log
typedef struct {
    char *enemy;
    int count;
} DefeatedEntry;

// Structure: game state
typedef struct {
    int gold;
    int current_hp;
    int max_hp;
    int floor;
    char current_room[16];

    // deck attributes
    CardEntry *deck;
    int deck_count;
    int deck_capacity;
    
    // relics attributes
    RelicEntry *relics;
    int relic_count;
    int relic_capacity;

    // potions attributes
    PotionEntry *potions;
    int potion_count;
    int potion_capacity;

    //codex attributes
    CodexEntry *codex;
    int codex_count;
    int codex_capacity;

    //defeated log attributes
    DefeatedEntry *defeated_log;
    int defeated_count;
    int defeated_capacity;
    
    //exhaust tags attributes
    char **exhaust_tags;
    int exhaust_count;
    int exhaust_capacity;
} GameState;

// ---FUNCTION PROTOTYPES---

void init_game_state(GameState *state);

#endif