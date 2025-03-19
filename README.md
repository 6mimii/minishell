# Minishell

A simple shell implementation inspired by bash.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Components](#components)
  - [Parser](#parser)
  - [Executor](#executor)
  - [Builtins](#builtins)
  - [Signal Handling](#signal-handling)
  - [Environment Variables](#environment-variables)
- [Project Structure](#project-structure)
- [Challenges and Solutions](#challenges-and-solutions)
- [Contributors](#contributors)
- [License](#license)

## Overview

Minishell is a simplified shell implementation that mimics the basic functionality of bash. This project is designed to understand the inner workings of a shell, including command parsing, execution, process creation, and environment management.

## Features

- Command execution with arguments
- Input/output redirections (`<`, `>`, `>>`)
- Pipes (`|`) for command chaining
- Environment variable expansion
- Signal handling (Ctrl+C, Ctrl+D, Ctrl+\\)
- Builtin commands implementation
- Error handling and proper exit status

## Installation

```bash
# Clone the repository
git clone https://github.com/yourusername/minishell.git

# Navigate to the project directory
cd minishell

# Compile the project
make

# Run minishell
./minishell
```

## Usage

Once you start minishell, you'll see a prompt where you can enter commands:

```
minishell> ls -la
minishell> echo $HOME
minishell> cat file.txt | grep "pattern" > output.txt
```

## Components

### Parser

The parser is responsible for analyzing and breaking down user input into meaningful components that the shell can understand and execute. It handles:

- Lexical analysis (tokenization) of the input string
- Syntax validation to ensure proper command structure
- Command and argument separation
- Recognition of special operators (pipes, redirections)
- Expansion of environment variables
- Handling of quotes and escape characters

The parser transforms raw input text into a structured representation (often an abstract syntax tree or command table) that can be processed by the executor.

### Executor

The executor takes the parsed command structure and handles the actual execution of commands. Key responsibilities include:

- Creating and managing child processes
- Setting up communication between processes (pipes)
- Handling input/output redirections
- Managing file descriptors
- Executing external commands by searching in PATH
- Invoking builtin commands
- Collecting and propagating exit statuses
- Handling command sequences and logical operators

The executor ensures that commands are run in the correct environment with proper input/output channels.

### Builtins

Minishell implements several builtin commands that are executed directly by the shell without creating a new process:

- `echo`: Display a line of text
- `cd`: Change the current directory
- `pwd`: Print the current working directory
- `export`: Set environment variables
- `unset`: Remove environment variables
- `env`: Display the environment variables
- `exit`: Exit the shell with a specified status code

### Signal Handling

The shell properly handles terminal signals:

- `Ctrl+C` (SIGINT): Interrupt the current foreground process
- `Ctrl+D` (EOF): Exit the shell when no processes are running
- `Ctrl+\` (SIGQUIT): Quit and core dump the current process

### Environment Variables

Minishell maintains its own environment, allowing users to:

- Display existing environment variables
- Create new environment variables
- Update existing environment variables
- Remove environment variables
- Expand variables in commands using the `$` symbol

## Project Structure

```
minishell/
├── Makefile
├── includes/
│   ├── minishell.h
├── srcs/
│   ├── main.c
│   ├── parser/
│   │   ├── lexer.c
│   │   ├── tokenizer.c
│   │   └── expander.c
│   ├── executor/
│   │   ├── executor.c
│   │   ├── redirections.c
│   │   └── pipes.c
│   ├── builtins/
│   │   ├── echo.c
│   │   ├── cd.c
│   │   └── ... (other builtins)
│   ├── utils/
│   │   ├── environment.c
│   │   ├── signals.c
│   │   └── error_handling.c
│   └── ... (other source files)
└── README.md
```

## Challenges and Solutions

During the development of Minishell, several challenges were encountered:

1. **Complex Command Parsing**: Handling nested quotes, escapes, and variable expansions required careful state tracking and recursive parsing.

2. **Process Management**: Proper handling of child processes, especially with pipes, demanded careful file descriptor management.

3. **Signal Handling**: Ensuring signals like Ctrl+C behave differently based on whether a command is running or the shell is waiting for input.

4. **Memory Management**: Preventing memory leaks when handling dynamic structures like the command table and environment variables.

## Contributors

- [Saffirio Filippo](https://github.com/JhoinZ)
- [Mohamed Doudi Baltit](https://github.com/6mimii)

## License

This project is licensed under the MIT License - see the LICENSE file for details.
