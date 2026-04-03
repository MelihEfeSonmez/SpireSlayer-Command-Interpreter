#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

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

// ---FUNCTIONS---

// Function: initialize the game state with starting conditions
static void init_game_state(GameState *state) {
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

// Function: remove new line character
static void chomp_newline(char *line) {
    size_t len = strlen(line);

    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
}

// Function: parse and execute the command line
static int execute_line(GameState *state, const char *line) {
    (void)state;
    (void)line;

    /*
     * Implement the Spring 2026 "spireslayer" grammar here.
     *
     * A typical approach is:
     * 1. Parse the whole line.
     * 2. Reject malformed input by returning 0.
     * 3. Apply semantic checks and print the required response.
     * 4. Return 1 after printing any non-INVALID response.
     */
    return 0;
}

// Function: main 
int main(void) {
    char line[MAX_LINE_LENGTH + 2];
    GameState state;

    init_game_state(&state);

    // loop which continues until reaching "Exit" command line
    while (1) {
        printf(">> ");
        fflush(stdout);

        // check whether there is any command line
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        chomp_newline(line); // remove new line character

        // check "Exit" command line
        if (strcmp(line, "Exit") == 0) {
            break;
        }

        // try to parse and execute the command line 
        // if fails, print "INVALID"
        if (!execute_line(&state, line)) {
            printf("INVALID\n");
        }
    }

    return 0;
}
