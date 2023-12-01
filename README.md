# Data Manipulation with Ptrace in C

## Overview

This project demonstrates the use of ptrace to attach to another program and manipulate data. It focuses on manipulating data in a program (`child1.c`) that contains variables such as password, shift, and shifted password stored in registers. The program runs in an endless loop until a flag in the register is changed.

The main program (`ptrace.c`) prompts the user for the PID (Process ID) of the target process, attaches to it using ptrace, retrieves the registers' content, performs data manipulation (converting and shifting), puts the modified data back into registers, and utilizes PTRACE_CONT to observe if the program leaks any data.

## Use Case

1. Compile both `ptrace.c` and `child1.c`.
2. Run the compiled `child1.c` with arguments `./a.out PASSWORD SHIFT` and note the PID of this process.
3. Run the compiled `ptrace.c` in another terminal.
4. Enter the PID when prompted.

## Installation

**Note: This project works only on Linux.**

## Technologies

- C
- Ptrace
- Assembly (asm)

## Implementation Details

- **child1.c:**
  - Contains variables like password, shift, and shifted password.
  - Runs in an endless loop until a flag in the register is changed.
  - Password, shift, and shifted password are stored in registers.

- **ptrace.c:**
  - Prompts the user for the PID of the target process.
  - Attaches to the target process using ptrace.
  - Retrieves the content of registers.
  - Performs data manipulation (conversion and shifting).
  - Puts the modified data back into registers.
  - Uses PTRACE_CONT to observe if the target program leaks any data.

This project showcases the powerful capabilities of ptrace for analyzing and manipulating data in a running program. Users can explore the implementation to understand the intricacies of low-level interactions between processes.

