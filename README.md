[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/WU4pxz6C)
# Assignment 1 | Spring 2026 | spireslayer

The starter repo for the Spring 2026 `spireslayer`.

Build the starter:

```sh
make
```

Run it:

```sh
./spireslayer
```

The interpreter reads from `stdin`, prints the `>> ` prompt before every command, and terminates only on the exact command `Exit`.

Run the included local grader:

```sh
make grade
```

Check a single test case manually:

```sh
mkdir -p my-outputs
python3 test/checker.py ./spireslayer test-cases/input1.txt my-outputs/output1.txt test-cases/output1.txt
```

Run the grader with explicit paths:

```sh
python3 test/grader.py ./spireslayer test-cases
```

The bundled test cases are only smoke tests for the starter scaffold. They do not cover the full Spring 2026 assignment grammar or semantics; hidden tests should be assumed to follow the official handout exactly.
