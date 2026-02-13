# Shell From Scratch - Complete Implementation Plan

## Project Overview
Build a Unix shell in C that supports command execution, I/O redirection, pipes, signal handling, and background processes.

---

## Phase 1: Foundation & Basic REPL

### Objectives
- Set up project structure
- Implement read-eval-print loop
- Handle basic user input

### Tasks
1. Create project directory structure
   - `shell.c` - main source file
   - `Makefile` - build configuration
   - `README.md` - documentation

2. Implement main loop
   - Infinite loop using `while(1)`
   - Display shell prompt (`myshell> `)
   - Read user input with `getline()` or `fgets()`
   - Handle empty input gracefully
   - Implement basic `exit` command

3. Memory management
   - Free dynamically allocated memory
   - Check for memory leaks

### Key Functions
- `getline()` - read user input
- `malloc()` / `free()` - memory management
- `strcmp()` - string comparison

### Testing
- Shell displays prompt
- Can enter text and see it processed
- `exit` command terminates shell
- No memory leaks (test with `valgrind`)

### Deliverable
A shell that displays a prompt, reads input, and responds to the exit command.

---

## Phase 2: Command Parsing

### Objectives
- Tokenize user input
- Build argument arrays for command execution

### Tasks
1. Implement tokenization
   - Split input string by whitespace
   - Handle multiple spaces/tabs
   - Store tokens in dynamic array

2. Build argument vector
   - Create `char *argv[]` array
   - NULL-terminate the array
   - Handle quoted strings (optional enhancement)

3. Handle edge cases
   - Empty input
   - Leading/trailing whitespace
   - Very long commands

### Key Functions
- `strtok()` or `strsep()` - string tokenization
- `malloc()` / `realloc()` - dynamic arrays
- `strdup()` - string duplication

### Testing
- Parse simple commands: `ls`, `pwd`
- Parse commands with arguments: `ls -la /home`
- Parse commands with multiple arguments: `echo hello world`
- Handle malformed input gracefully

### Deliverable
Parser that converts input string into argument array suitable for `exec()`.

---

## Phase 3: Process Creation & Execution

### Objectives
- Execute external programs
- Implement fork-exec-wait pattern

### Tasks
1. Implement process creation
   - Use `fork()` to create child process
   - Handle fork errors
   - Understand return values (0 for child, PID for parent)

2. Execute commands in child
   - Use `execvp()` to run programs
   - Handle execution errors (command not found)
   - Understand that `exec()` doesn't return on success

3. Parent process waiting
   - Use `wait()` or `waitpid()` to wait for child
   - Capture exit status
   - Display error messages if command fails

4. Error handling
   - Check all system call return values
   - Print meaningful error messages with `perror()`

### Key System Calls
- `fork()` - create new process
- `execvp()` - execute program
- `wait()` / `waitpid()` - wait for child
- `perror()` - print system error

### Testing
- Run basic commands: `ls`, `pwd`, `date`, `whoami`
- Run commands with arguments: `ls -l`, `echo hello`
- Test non-existent commands (should show error)
- Test commands that fail (e.g., `cat nonexistent.txt`)

### Deliverable
Shell that can execute any external command available on the system.

---

## Phase 4: Built-in Commands

### Objectives
- Implement commands that must run in shell process
- Understand why certain commands can't be external

### Tasks
1. Implement `cd` (change directory)
   - Parse `cd` command before forking
   - Use `chdir()` to change directory
   - Handle no arguments (cd to home)
   - Handle errors (directory doesn't exist)

2. Implement `pwd` (print working directory)
   - Use `getcwd()` to get current directory
   - Print to stdout

3. Enhance `exit` command
   - Clean up resources
   - Exit with proper status code

4. Implement `help` (optional)
   - Display available built-in commands
   - Show usage information

### Key Functions
- `chdir()` - change working directory
- `getcwd()` - get current directory
- `getenv()` - get environment variables (for HOME)

### Testing
- `cd /tmp` then `pwd` shows `/tmp`
- `cd` with no args goes to home directory
- `cd nonexistent` shows error but shell continues
- `exit` terminates shell cleanly

### Deliverable
Shell with functional built-in commands: cd, pwd, exit.

---

## Phase 5: Output Redirection

### Objectives
- Redirect command output to files
- Support `>` and `>>` operators

### Tasks
1. Parse redirection operators
   - Detect `>` and `>>` in command line
   - Separate command from filename
   - Remove redirection from argv

2. Implement `>` (truncate mode)
   - Open file with `open()` using flags: `O_WRONLY | O_CREAT | O_TRUNC`
   - Set permissions: `0644`
   - Use `dup2()` to redirect stdout (fd 1) to file
   - Close original file descriptor

3. Implement `>>` (append mode)
   - Open file with flags: `O_WRONLY | O_CREAT | O_APPEND`
   - Use `dup2()` same as truncate mode

4. Error handling
   - Handle file open errors (permissions, path)
   - Restore stdout if needed

### Key System Calls
- `open()` - open file
- `close()` - close file descriptor
- `dup2()` - duplicate file descriptor
- File flags: `O_WRONLY`, `O_CREAT`, `O_TRUNC`, `O_APPEND`

### Testing
- `ls > output.txt` - file contains ls output
- `echo hello > test.txt` then verify contents
- `echo world >> test.txt` - appends to file
- `ls nonexistent > error.txt` - error still shown or redirected to file
- Redirect to file in non-existent directory (should fail gracefully)

### Deliverable
Shell supporting output redirection with `>` and `>>`.

---

## Phase 6: Input Redirection

### Objectives
- Redirect file content as command input
- Support `<` operator

### Tasks
1. Parse input redirection
   - Detect `<` in command line
   - Extract filename
   - Remove redirection from argv

2. Implement `<` redirection
   - Open file with `open()` using `O_RDONLY`
   - Use `dup2()` to redirect stdin (fd 0) to file
   - Close original file descriptor

3. Error handling
   - Handle file not found
   - Handle permission errors

### Key System Calls
- `open()` with `O_RDONLY` flag
- `dup2()` for stdin redirection

### Testing
- `cat < input.txt` - displays file contents
- `wc -l < file.txt` - counts lines from file
- `cat < nonexistent.txt` - shows error
- `sort < unsorted.txt` - sorts file contents

### Deliverable
Shell supporting input redirection with `<`.

---

## Phase 7: Combining Redirections

### Objectives
- Support multiple redirections in one command
- Handle complex redirection scenarios

### Tasks
1. Parse multiple redirections
   - Support `< input.txt > output.txt`
   - Support `> out.txt < in.txt` (order doesn't matter)

2. Apply redirections correctly
   - Redirect both stdin and stdout
   - Ensure proper file descriptor management

3. Error handling
   - Handle conflicts gracefully
   - Close all opened file descriptors

### Testing
- `cat < input.txt > output.txt`
- `sort < unsorted.txt > sorted.txt`
- `wc < file.txt > count.txt`

### Deliverable
Shell supporting simultaneous input and output redirection.

---

## Phase 8: Pipes (Single Pipe)

### Objectives
- Connect two commands via pipe
- Implement inter-process communication

### Tasks
1. Parse pipe operator
   - Detect `|` in command line
   - Split into left and right commands
   - Parse each command separately

2. Create pipe
   - Use `pipe()` to create pipe
   - Understand pipe file descriptors: `pipefd[0]` (read), `pipefd[1]` (write)

3. Fork twice
   - First child runs left command
   - Second child runs right command
   - Parent waits for both

4. Connect processes
   - Left child: redirect stdout to `pipefd[1]`
   - Right child: redirect stdin from `pipefd[0]`
   - Close unused pipe ends in all processes

5. Resource cleanup
   - Close all pipe file descriptors
   - Wait for both children

### Key System Calls
- `pipe()` - create pipe
- `dup2()` - redirect file descriptors
- `fork()` - create multiple processes
- `close()` - close file descriptors

### Testing
- `ls | grep txt` - filters ls output
- `cat file.txt | wc -l` - counts lines
- `ps aux | grep bash` - filters process list
- `echo hello | cat` - simple pipe test

### Deliverable
Shell supporting single pipe between two commands.

---

## Phase 9: Multiple Pipes

### Objectives
- Chain multiple commands
- Support arbitrary number of pipes

### Tasks
1. Parse multiple pipes
   - Split command line by `|`
   - Create array of commands
   - Count number of pipes

2. Create pipe chain
   - Create N-1 pipes for N commands
   - Fork N children
   - Connect each child's stdin to previous pipe's read end
   - Connect each child's stdout to next pipe's write end

3. Handle first and last commands
   - First command: only redirect stdout
   - Last command: only redirect stdin
   - Middle commands: redirect both

4. Close all file descriptors
   - Parent closes all pipe ends
   - Each child closes unused pipe ends

### Testing
- `ls | grep txt | wc -l` - three commands
- `cat file.txt | sort | uniq | wc -l` - four commands
- `ps aux | grep bash | grep -v grep` - filter chains

### Deliverable
Shell supporting multiple pipes in a single command.

---

## Phase 10: Signal Handling

### Objectives
- Handle keyboard interrupts properly
- Prevent Ctrl+C from killing shell

### Tasks
1. Understand signals
   - SIGINT: Ctrl+C (interrupt)
   - SIGTSTP: Ctrl+Z (suspend)
   - SIGCHLD: child process state change

2. Set up signal handlers
   - Use `signal()` or `sigaction()`
   - Ignore SIGINT in parent shell
   - Let child processes receive SIGINT normally

3. Implement handlers
   - SIGINT handler: do nothing in parent
   - SIGTSTP handler: optional job control
   - SIGCHLD handler: reap zombie processes

4. Signal safety
   - Use async-signal-safe functions only
   - Understand race conditions

### Key Functions
- `signal()` - simple signal handling
- `sigaction()` - advanced signal handling
- Signal constants: `SIGINT`, `SIGTSTP`, `SIGCHLD`

### Testing
- Run long command (e.g., `sleep 100`)
- Press Ctrl+C - command stops, shell continues
- Run shell, press Ctrl+C - shell stays alive
- Verify no zombie processes with `ps aux | grep defunct`

### Deliverable
Shell that handles signals correctly and doesn't get killed by Ctrl+C.

---

## Phase 11: Background Processes

### Objectives
- Run commands in background
- Support `&` operator

### Tasks
1. Parse background operator
   - Detect `&` at end of command
   - Remove `&` from arguments
   - Set background flag

2. Don't wait for background jobs
   - Skip `wait()` if background
   - Return prompt immediately
   - Store background job info (optional)

3. Reap zombie processes
   - Set up SIGCHLD handler
   - Use `waitpid()` with `WNOHANG`
   - Clean up finished background jobs

4. Job tracking (optional)
   - Maintain list of background jobs
   - Assign job numbers
   - Display when jobs complete

### Key System Calls
- `waitpid()` with `WNOHANG` flag
- SIGCHLD signal handling

### Testing
- `sleep 10 &` - returns prompt immediately
- `sleep 10 &` then `ps` - sleep process visible
- Run multiple background jobs
- Verify no zombie processes accumulate

### Deliverable
Shell supporting background job execution with `&`.

---

## Phase 12: Advanced Features (Optional)

### Feature A: Command History
- Use GNU readline library
- Store command history
- Support up/down arrow keys
- Implement history search

### Feature B: Tab Completion
- Complete command names
- Complete file paths
- Complete directory names

### Feature C: Environment Variables
- Parse and expand `$VAR` syntax
- Implement `export` built-in
- Implement `env` built-in
- Support variable substitution

### Feature D: Globbing
- Expand wildcards (`*`, `?`)
- Pattern matching
- Use `glob()` function

### Feature E: Scripting
- Read commands from file
- Support shebang (`#!/path/to/shell`)
- Batch command execution

### Feature F: Job Control
- Implement `jobs` built-in
- Implement `fg` (foreground) built-in
- Implement `bg` (background) built-in
- Track job states (running, stopped)

### Feature G: Prompt Customization
- Support PS1-style prompts
- Display current directory
- Display username/hostname
- Color support

---

## Testing Strategy

### Unit Testing
- Test each function individually
- Test edge cases and error conditions
- Use assertions for internal validation

### Integration Testing
- Test complete command workflows
- Test combinations of features (pipes + redirection)
- Test error handling across features

### Memory Testing
- Run with `valgrind` to check for leaks
- Check for memory corruption
- Verify proper cleanup on exit

### Stress Testing
- Very long commands
- Many pipes in sequence
- Many background jobs
- Large files for redirection

---

## Common Pitfalls & Solutions

### Pitfall 1: Forgetting to Close File Descriptors
**Problem:** File descriptor leak, process runs out of FDs
**Solution:** Close FDs immediately after `dup2()`, close all pipe ends

### Pitfall 2: Not Checking Return Values
**Problem:** Errors go undetected, undefined behavior
**Solution:** Check every system call, use `perror()` for errors

### Pitfall 3: Zombie Processes
**Problem:** Background jobs become zombies
**Solution:** Set up SIGCHLD handler, use `waitpid()` with `WNOHANG`

### Pitfall 4: Memory Leaks
**Problem:** Allocated memory never freed
**Solution:** Free after use, use `valgrind` to detect leaks

### Pitfall 5: Buffer Overflows
**Problem:** Writing beyond buffer boundaries
**Solution:** Use safe functions (`strncpy`, `snprintf`), bounds checking

### Pitfall 6: Race Conditions with Signals
**Problem:** Signal handler conflicts with main code
**Solution:** Use `sigaction()`, only async-signal-safe functions in handlers

### Pitfall 7: Improper Fork Usage
**Problem:** Creating too many processes or processes that don't exit
**Solution:** Understand fork return values, ensure children exit properly

---

## Project Milestones & Timeline

### Week 1: Foundation
- **Milestone 1:** Basic REPL working
- **Milestone 2:** Command parsing complete
- **Milestone 3:** Simple command execution

### Week 2: Core Features
- **Milestone 4:** Built-in commands working
- **Milestone 5:** Output redirection implemented
- **Milestone 6:** Input redirection implemented
- **Milestone 7:** Combined redirections working

### Week 3: Advanced Features
- **Milestone 8:** Single pipe working
- **Milestone 9:** Multiple pipes working
- **Milestone 10:** Signal handling complete

### Week 4: Polish & Extras
- **Milestone 11:** Background jobs working
- **Milestone 12:** Testing complete, bugs fixed
- **Milestone 13:** Optional features (choose 1-2)
- **Milestone 14:** Documentation complete

---

## Resources & References

### Essential Man Pages
- `man 2 fork` - process creation
- `man 2 execvp` - program execution
- `man 2 wait` - waiting for child processes
- `man 2 pipe` - creating pipes
- `man 2 dup2` - duplicating file descriptors
- `man 2 open` - opening files
- `man 2 signal` - signal handling
- `man 3 getline` - reading input

### Recommended Reading
- "Advanced Programming in the UNIX Environment" by Stevens & Rago
- "The Linux Programming Interface" by Michael Kerrisk
- GNU Bash source code (for reference)

### Debugging Tools
- `gdb` - debugger
- `valgrind` - memory leak detector
- `strace` - system call tracer
- `ltrace` - library call tracer

### Online Resources
- Linux man pages: https://man7.org/linux/man-pages/
- GNU C Library documentation
- POSIX standards documentation

---

## Code Structure Recommendations

### Suggested File Organization
```
project/
├── shell.c              # Main shell loop
├── parser.c/parser.h    # Command parsing
├── executor.c/executor.h # Command execution
├── builtins.c/builtins.h # Built-in commands
├── redirect.c/redirect.h # I/O redirection
├── signals.c/signals.h   # Signal handling
├── utils.c/utils.h       # Utility functions
├── Makefile
└── README.md
```

### Coding Standards
- Use meaningful variable names
- Comment complex logic
- Consistent indentation (4 spaces or tabs)
- Error checking on all system calls
- Free all allocated memory
- No global variables (except where necessary for signals)

---

## Success Criteria

### Minimum Viable Product
- ✅ Displays prompt and reads commands
- ✅ Executes external programs
- ✅ Supports `cd`, `pwd`, `exit` built-ins
- ✅ Handles Ctrl+C without crashing
- ✅ Basic error handling

### Full Feature Set
- ✅ All MVP features
- ✅ Input/output redirection (`<`, `>`, `>>`)
- ✅ Pipes (single and multiple)
- ✅ Background processes (`&`)
- ✅ Proper signal handling
- ✅ No memory leaks
- ✅ Comprehensive error handling

### Excellence
- ✅ All full feature set items
- ✅ 2+ optional advanced features
- ✅ Clean, well-documented code
- ✅ Comprehensive test suite
- ✅ Professional README
- ✅ Handles edge cases gracefully

---

## Final Deliverables

1. **Source Code**
   - Well-organized, commented C files
   - Makefile for easy compilation
   - No compiler warnings

2. **Documentation**
   - README with usage instructions
   - List of implemented features
   - Known limitations
   - Build instructions

3. **Testing**
   - Test script demonstrating features
   - Evidence of memory leak testing
   - Bug tracking log

4. **Presentation** (if applicable)
   - Demo of shell capabilities
   - Code walkthrough
   - Challenges and solutions discussion

---

## Next Steps

1. **Set up development environment**
   - Install GCC compiler
   - Install valgrind
   - Install gdb
   - Set up version control (git)

2. **Create project structure**
   - Initialize git repository
   - Create initial files
   - Write basic Makefile

3. **Start with Phase 1**
   - Implement basic REPL
   - Test thoroughly
   - Commit working code

4. **Iterate through phases**
   - Complete one phase at a time
   - Test after each phase
   - Don't move forward until current phase works

5. **Regular testing**
   - Test after each feature
   - Use valgrind frequently
   - Keep a bug log

---

## Conclusion

Building a shell from scratch is an excellent way to understand:
- Process management in Unix/Linux
- File descriptors and I/O
- Inter-process communication
- Signal handling
- Systems programming in C

Take your time with each phase, understand the concepts deeply, and don't hesitate to consult man pages and documentation. Good luck!