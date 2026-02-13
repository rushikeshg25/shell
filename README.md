# Jarvis

Jarvis is a custom Unix-like shell implemented in C. It provides a robust command-line interface with support for process execution, pipelines, input/output redirection, and signal management.

## Features

### Command Execution
The shell supports the execution of external binary commands found in the system path using the standard fork-exec-wait pattern.

### Built-in Commands
Several commands are handled directly by the shell process for efficiency and to manage the shell's state:
- **cd**: Changes the current working directory. Supports absolute paths, relative paths, and defaults to the HOME directory if no arguments are provided.
- **pwd**: Prints the absolute path of the current working directory.
- **exit**: Terminates the shell session gracefully.

### Input/Output Redirection
The shell implements file descriptor manipulation to support redirection:
- **Output Redirection (`>`)**: Overwrites a file with the output of a command.
- **Append Redirection (`>>`)**: Appends command output to the end of a file.
- **Input Redirection (`<`)**: Reads input for a command from a specified file.

### Pipelines
Users can chain multiple commands together using the pipe operator (`|`). The shell creates pipes and orchestrates concurrent process execution, ensuring the output of one command is correctly directed as the input to the next.

### Signal Handling
The shell is designed to be resilient:
- The parent shell process ignores interrupt (`SIGINT`) and stop (`SIGTSTP`) signals, preventing users from accidentally killing the shell with Ctrl+C or Ctrl+Z.
- Child processes are configured to restore default signal behaviors so they can be terminated or stopped as expected.

### Background Processes
Commands followed by an ampersand (`&`) are executed in the background. The shell returns the prompt immediately and reaps finished background processes automatically using a SIGCHLD handler to prevent zombie processes.

## Architecture and Design

The shell is built with a modular architecture to ensure clean separation of concerns and maintainability. It follows a classic Read-Eval-Print Loop (REPL) structure but uses a sophisticated execution engine to handle complex command patterns.

### The REPL Loop
The entry point in `main.c` initializes the signal handlers and enters a loop that uses the GNU Readline library. Readline provides advanced features like persistent command history and line editing shortcuts.

### Tokenization and Parsing
Input strings are processed in two stages:
1. **Pipeline Level**: The command line is split into individual command components based on the pipe (`|`) character.
2. **Argument Level**: Each individual command is tokenized into an argument vector (`argv`) based on whitespace. The parser also detects special tokens for redirection and background execution.

### Execution Engine
The execution engine in `execute.c` is the core of the shell. It manages process creation and pipe orchestration:
- For single commands that are built-ins like `cd` or `exit`, it executes them directly in the parent process to modify the shell's environment.
- For all other commands and pipelines, it creates the necessary pipes and forks child processes.
- It uses `dup2` to handle file descriptor redirection before executing the target binary.

## File Structure

The project is organized into the following modules:

- **main.c**: Contains the main REPL loop and shell initialization.
- **execute.c**: Implements the command execution logic, pipeline management, and process forking.
- **execute.h**: Exports the top-level execution interface.
- **parser.c**: Handles string tokenization and command/pipeline parsing.
- **parser.h**: Defines the tokenization and cleanup interfaces.
- **builtins.c**: Contains implementation for internal shell commands like `cd` and `pwd`.
- **builtins.h**: Defines the interface for identifying and executing built-ins.
- **redirect.c**: Manages file descriptor manipulation for `>`, `>>`, and `<`.
- **redirect.h**: Exports the redirection application logic.
- **signals.c**: Configures signal handlers for the parent shell and background job reaping.
- **signals.h**: Defines the signal setup interface.
- **makefile**: The build configuration for the project.

## Implementation Details

### Concurrent Pipeline Execution
Unlike simple shell implementations that execute piped commands sequentially, this shell launches all processes in a pipeline concurrently. It creates $N-1$ pipes for $N$ commands and ensures all processes are running before the parent begins waiting for the foreground ones to complete.

### Memory Management
The shell uses a centralized `free_tokens` system to ensure that all dynamically allocated memory for command strings and argument vectors is properly released after each execution cycle, preventing memory leaks during long-running sessions.

### Safe Build System
The makefile is configured with strict compiler flags (`-Wall -Wextra`) to catch potential issues during development. It links against `libreadline` for the user interface components.

### Execution
To start the shell, use:
```bash
make run
```
Alternatively, you can run the binary directly:
```bash
./shell
```
