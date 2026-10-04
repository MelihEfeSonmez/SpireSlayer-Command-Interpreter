# SpireSlayer: Command Interpreter in C

A command-line interpreter written in **C11** that tracks the state of an *Ironclad* run in the style of *Slay the Spire*. It runs a **read → parse → respond** loop over a small English-like command language. Commands change or query the player's gold, HP, deck, relics, potions, map progress and a per-enemy "codex" of what works against each enemy.


```text
Ironclad gains card Bash
» Card added: Bash
Ironclad upgrades card Bash
» Card upgraded: Bash
Ironclad learns card Bash is effective against Cultist
» Codex entry created: Cultist
Ironclad fights Cultist for 25 gold
» Ironclad defeats Cultist and gains 25 gold
Deck ?
» 1 Bash+
Total gold ?
» 25
Exit
```

---

## Highlights

- **Hand-written tokenizer and parser.** No parser generators or regex libraries. Every line is tokenized and matched exactly against the grammar. Multi-word names (e.g. `Gremlin Nob`, `Burning Blood`) are supported, and malformed input gets `INVALID` instead of crashing.
- **Strict input validation.** Positive-integer checks, name validation (spacing rules between tokens) and exact keyword matching, so the program never trusts its input.
- **Dynamic memory management.** The deck, relics, potions, codex, defeat log and exhaust tags are all growable arrays (`malloc`/`realloc` with capacity doubling). Everything is freed on exit.
- **Modular design.** Parsing, state, state-changing commands (*sentences*) and read-only commands (*queries*) live in separate modules with clear headers.
- **Automated testing and CI.** A Python grader runs the binary against expected-output test cases, and a GitHub Actions workflow builds and grades every push inside a Docker image.

## Command Language

Each input line is one of three kinds:

| Kind | Form | Effect |
|------|------|--------|
| **Sentence** | `Ironclad ...` | Changes the game state |
| **Question** | `... ?` | Reads the state without changing it |
| **Exit** | `Exit` | Ends the interpreter |

Lines that don't match the grammar get `» INVALID`.

<details>
<summary><b>Sentences (state-changing)</b></summary>

```text
Ironclad gains <n> gold
Ironclad gains <n> max hp
Ironclad gains card|relic|potion <name>
Ironclad buys card|relic|potion <name> for <n> gold
Ironclad sells card <name> for <n> gold
Ironclad sells upgraded card <name> for <n> gold
Ironclad sells potion <name> for <n> gold
Ironclad removes card <name>
Ironclad removes upgraded card <name>
Ironclad upgrades card <name>
Ironclad marks card <name> as exhaust
Ironclad discards potion <name>
Ironclad enters Monster|Elite|Rest|Shop|Treasure|Event|Boss room
Ironclad learns card|relic|potion <name> is effective against <enemy>
Ironclad fights <enemy>
Ironclad fights <enemy> for <n> gold
Ironclad heals <n> hp
Ironclad takes <n> damage
```
</details>

<details>
<summary><b>Questions (read-only)</b></summary>

```text
Total gold ?
Health ?
Floor ?
Where ?
Deck ?
Deck size ?
Total card <name> ?
Total upgraded card <name> ?
Relics ?
Potions ?
Exhausts ?
What is effective against <enemy> ?
Defeated <enemy> ?
```
</details>

### Game rules the interpreter enforces

- Ironclad starts with **80/80 HP**, 0 gold, floor 0, and no room.
- HP never goes below 0 or above max HP. The potion belt holds at most **3** potions, and a relic can only be owned once.
- Buying fails with `Not enough gold` if the player can't afford it.
- **Fights** are decided by the codex. If Ironclad owns anything recorded as effective against the enemy, it wins. On a win, effective potions are used up and one copy of each effective *exhaust* card is removed (a base copy before an upgraded one). On a loss, Ironclad takes 15 damage and flees.

## Project Structure

```text
spireslayer/
├── src/
│   ├── main.c        # REPL loop and grammar dispatch
│   ├── parser.c/.h   # Tokenizer, name extraction, input validation
│   ├── sentences.c/.h# State-changing commands (gain, buy, fight, ...)
│   ├── queries.c/.h  # Read-only questions (deck, codex, health, ...)
│   └── state.c/.h    # GameState struct, init and cleanup
├── test/
│   ├── grader.py     # Runs every test case and reports a score
│   └── checker.py    # Compares one run against its expected output
├── test-cases/       # input_XX.txt / output_XX.txt pairs
├── .github/workflows/grade.yml  # CI: build and grade on every push
└── Makefile
```

## Getting Started

**Requirements:** `gcc` (C11), `make`, and `python3` for the tests.

```sh
# Build
make

# Run interactively
./spireslayer

# Run from a script file
./spireslayer < test-cases/input_01.txt
```

The binary is compiled with `-std=c11 -Wall -Wextra -pedantic`.

## Testing

Run the whole test suite:

```sh
make grade
```

Check one test case:

```sh
mkdir -p my-outputs
python3 test/checker.py ./spireslayer test-cases/input_01.txt my-outputs/output_01.txt test-cases/output_01.txt
```

## Authors

- **Melih Efe Sönmez**
- **İlhan Altınay**
