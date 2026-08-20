# Triflare

Triflare is a small esoteric programming language using only three characters: `0`, `1`, and `2`.

It operates on an effectively unbounded tape of binary values. A pointer selects the current tape cell.

## Installation

The easiest installation will be 

| OS  | Command |
| --- | ----- |
| Linux | `git clone https://github.com/Fiaguhop137/triflare && sudo install triflare/tric /usr/local/bin/ && rm -r triflare` |
| Windows(powershell) | `git clone https://github.com/Fiaguhop137/triflare; Copy-Item triflare/tric.exe -Destination "C:\Windows\System32\"; Remove-Item -Recurse -Force triflare` |
| Windows(command prompt) | `git clone https://github.com/Fiaguhop137/triflare && copy triflare\tric.exe C:\Windows\System32\ && rmdir /s /q triflare` |
| MacOS | Not compatible |

This command automatically pulls the latest version from my repository, installs everything, and cleans up after itself. 

## Instructions

| Code | Operation                       |
| ---- | ------------------------------- |
| `0`  | Move the pointer one cell left  |
| `1`  | Move the pointer one cell right |
| `2`  | Toggle the current bit          |
| `22` | Output the current bit          |
| `10` | Begin a loop                    |
| `01` | End a loop                      |

The tape contains only `0` and `1`.

### Movement

These move the pointer:
```text
0 = pointer--
1 = pointer++
```

The tape itself is not changed. The tape begins at cell 0. Moving left of cell 0 results in undefined behavior.

### Toggle

`2` toggles the current bit:
```text
0 → 1
1 → 0
```

Equivalent C++:
```cpp
tape[pointer] = !tape[pointer];
```

### Output

`22` outputs the value of the current tape cell.

Because `22` is a two-character instruction, it takes priority over two consecutive `2` instructions.

For example:
```text
222
```

is interpreted as:
```text
22 2
```

so it outputs the current bit and then toggles it.

### Loops

Loops are written as:
```text
10
    instructions
01
```

The loop executes while the current tape cell is `1`.

For example:
```text
10
    2
01
```

is equivalent to:
```cpp
while (tape[pointer] != 0) {
    tape[pointer] = !tape[pointer];
}
```

Since the `2` toggles the current bit from `1` to `0`, this loop executes once when the bit is `1` and not at all when it is `0`.

Loops may be nested. Empty loops do not modify the current bit. Therefore, an empty loop terminates if the current bit is `0` and runs forever if the current bit is `1`. On a random bit, this gives it a 50% chance of causing an infinite loop.

## RS NOR

There are no dedicated "set to 0" or "set to 1" instructions. They can be constructed from the existing instructions.

Set the current bit to `0`:
```text
10201
```

Set the current bit to `1`:
```text
102012
```

These two work regardless of the bit's initial value:
For 10201:
```text
initial 0 → loop skipped → 0
initial 1 → toggle → 0 → loop ends → 0
```

For 102012, the final 2 toggles the resulting 0 back to 1:
```text
initial 0 → loop skipped → toggle → 1
initial 1 → toggle → 0 → loop ends → toggle → 1
```

## Variables

Triflare deliberately has no variables, types, named memory locations, arithmetic operators, or other conveniences commonly found in conventional programming languages.

The programmer is responsible for keeping track of where data is stored on the tape.

The following sets five consecutive tape cells to `10001` which can represent the binary number 17: `102012110201110201110201110201102012`

## Destructive Bit Copy

This sequence moves the value of the current cell (Cell A) to the next cell (Cell B). 
```text
1        - Moves to cell B
10201    - Sets cell B to 0
0        - Moves back to cell A
10       - Begin loop if Cell A is 1
 2       - Toggle Cell A to 0 (preparing to exit loop)
 1       - Move to Cell B
 2       - Toggle Cell B to 1
 0       - Move back to Cell A
01       - End loop
```
Combined: `110201010212001`

## Non-Destructive Bit Copy

To copy a bit from Cell A to Cell B without destroying the original value, an auxiliary cell (Cell C) is required. 
```text
1        - Moves to cell B
10201    - Set cell B to 0
0        - Moves back to cell A
10       - If Cell A is 1
  1      - Moves to cell B
  2      - Toggles cell B to 1
  1      - Moves to cell c
  10201  - Sets cell C to 0 to end loop
01       - End loop (Cell A and B are the same now)
```
Combined: `1102010101211020101`

## Compilation

Triflare programs use the `.tri` extension.

The current compiler translates Triflare into C++, then uses `g++` to compile the generated C++ into a native executable:
```text
program.tri
    ↓
Triflare compiler
    ↓
program.cpp
    ↓
g++
    ↓
program
```

The generated C++ file is removed after compilation.

In other words, Triflare is currently stealing C++'s backend instead of having its own. This is intentional, because writing an entire machine-code backend for a three-character language would be a somewhat unnecessary descent into madness.
