# Minishell

[![en](https://img.shields.io/badge/lang-en-blue.svg)](README.md)
[![fr](https://img.shields.io/badge/lang-fr-red.svg)](README.fr.md)

A lightweight UNIX shell implementation in C, recreating core functionalities of bash. This project is part of the 42 school curriculum and demonstrates system programming concepts including process management, file descriptors, and signal handling.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Architecture](#architecture)
- [Core Components](#core-components)
  - [1. Lexer (Tokenization)](#1-lexer-tokenization)
  - [2. Parser](#2-parser)
  - [3. Here-Document Handling](#3-here-document-handling)
  - [4. Execution](#4-execution)
  - [5. Built-in Commands](#5-built-in-commands)
  - [6. Signal Handling](#6-signal-handling)
- [Data Structures](#data-structures)
- [Quote and Variable Expansion](#quote-and-variable-expansion)
- [Pipes and Redirections](#pipes-and-redirections)
- [Compilation](#compilation)
- [Usage](#usage)

---

## Overview

Minishell reads user input, tokenizes it, parses tokens into commands, and executes them. The shell supports:
- Command execution with arguments
- Pipes (`|`) for chaining commands
- Input/Output redirections (`<`, `>`, `>>`)
- Here-documents (`<<`)
- Environment variable expansion (`$VAR`, `$?`)
- Quote handling (single and double quotes)
- Built-in commands (cd, echo, env, exit, export, pwd, unset)

---

## Features

| Feature | Description |
|---------|-------------|
| **Prompt** | Interactive prompt using GNU readline |
| **History** | Command history navigation with arrow keys |
| **Pipes** | Chain multiple commands: `cmd1 \| cmd2 \| cmd3` |
| **Redirections** | `<` input, `>` output, `>>` append |
| **Here-doc** | `<< DELIMITER` multi-line input |
| **Variables** | `$VAR` expansion, `$?` for exit code |
| **Quotes** | `'single'` (literal) and `"double"` (with expansion) |
| **Signals** | Ctrl+C, Ctrl+D, Ctrl+\ handling |
| **Built-ins** | cd, echo, env, exit, export, pwd, unset |

---

## Project Structure

```
minishell/
├── include/
│   ├── minishell.h      # Main header - all function declarations
│   ├── struct.h         # Data structure definitions
│   ├── macros.h         # Constants and signal type enum
│   ├── library.h        # System library includes
│   └── my_lib/          # Custom libft library
│
├── src/
│   ├── main.c           # Entry point and main loop
│   ├── header.c         # ASCII art startup animation
│   ├── signals.c        # Signal setup functions
│   ├── signals_execution.c  # Signal handlers
│   │
│   ├── lexer/           # Tokenization module
│   │   ├── lexer.c              # Main lexer entry point
│   │   ├── identify_tokens.c    # Token classification
│   │   ├── calc_token_len.c     # Calculate expanded length
│   │   ├── copy_token.c         # Copy with expansion
│   │   ├── token_parser.c       # Quote removal
│   │   └── utils.c              # Lexer utilities
│   │
│   ├── parser/          # Command structure creation
│   │   ├── parser.c             # Main parser entry point
│   │   ├── check_token_syntax.c # Syntax validation
│   │   ├── counting.c           # Count commands/args
│   │   ├── prepar_cmd.c         # Build command list
│   │   └── handle_redirection.c # Redirection parsing
│   │
│   ├── execution/       # Command execution
│   │   ├── execute.c            # Main execution logic
│   │   ├── execute_build_in.c   # Built-in dispatcher
│   │   ├── execute_external.c   # External command execution
│   │   ├── get_path.c           # PATH resolution
│   │   ├── check_the_acces.c    # File access validation
│   │   └── utilities.c          # Pipe/wait helpers
│   │
│   ├── here_doc/        # Here-document handling
│   │   ├── here_doc.c           # Main here-doc logic
│   │   ├── here_doc_utils.c     # Temp file management
│   │   └── expand.c             # Variable expansion
│   │
│   ├── build_in/        # Built-in commands
│   │   ├── cd.c, echo.c, env.c, pwd.c, exit.c, unset.c
│   │   ├── export.c             # Export command
│   │   ├── export_parser.c      # Export argument parsing
│   │   └── export_utils.c       # Export helpers
│   │
│   ├── init/            # Initialization
│   │   ├── init_data.c          # Main data init
│   │   ├── init_envp.c          # Environment setup
│   │   └── init_cmd.c           # Command struct init
│   │
│   └── utils/           # Utilities
│       ├── cleanup.c            # Post-command cleanup
│       ├── free_all.c           # Memory deallocation
│       ├── error.c              # Error messages
│       ├── exit_all.c           # Exit with cleanup
│       ├── get_char.c           # String utilities
│       ├── get_int.c            # Integer utilities
│       └── utilities_.c         # Execution helpers
│
└── Makefile
```

---

## Architecture

The shell follows a classic interpreter pipeline:

```
┌─────────────────────────────────────────────────────────────────────────┐
│                              MAIN LOOP                                   │
│                           (src/main.c)                                   │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  1. INPUT                                                                │
│     readline("minishell$ ") → raw input string                          │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  2. LEXER (Tokenization)                        src/lexer/              │
│     ┌──────────────────────────────────────────────────────────────┐    │
│     │ "echo hello | cat -e" → [echo][hello][|][cat][-e]           │    │
│     │                                                               │    │
│     │ • Split on whitespace and operators (|, <, >, <<, >>)        │    │
│     │ • Preserve quoted sections                                    │    │
│     │ • Expand $VARIABLES (except in single quotes)                 │    │
│     │ • Assign token types (COMMAND, STRING, PIPE, REDIRECT, etc.) │    │
│     └──────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  3. PARSER                                      src/parser/             │
│     ┌──────────────────────────────────────────────────────────────┐    │
│     │ Tokens → Command Structures (t_cmd linked list)              │    │
│     │                                                               │    │
│     │ • Validate syntax (no empty pipes, proper redirections)      │    │
│     │ • Group tokens into commands (split by pipes)                │    │
│     │ • Extract redirections and filenames                         │    │
│     │ • Build argument arrays for each command                     │    │
│     └──────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  4. HERE-DOC PROCESSING                         src/here_doc/           │
│     ┌──────────────────────────────────────────────────────────────┐    │
│     │ Before execution, read all here-doc content                  │    │
│     │                                                               │    │
│     │ • Fork child to read input until delimiter                   │    │
│     │ • Store in temp file (/tmp/here_doc_N)                       │    │
│     │ • Expand variables if delimiter wasn't quoted                │    │
│     └──────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  5. EXECUTION                                   src/execution/          │
│     ┌──────────────────────────────────────────────────────────────┐    │
│     │ Execute command pipeline                                      │    │
│     │                                                               │    │
│     │ Single built-in (no pipe):                                   │    │
│     │   → Execute directly in parent process                       │    │
│     │                                                               │    │
│     │ Pipeline or external command:                                │    │
│     │   → Create pipes between commands                            │    │
│     │   → Fork child for each command                              │    │
│     │   → Setup redirections (dup2)                                │    │
│     │   → Execute (built-in or execve)                             │    │
│     │   → Wait for all children                                    │    │
│     └──────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  6. CLEANUP                                     src/utils/              │
│     • Free tokens and command structures                                │
│     • Delete temp files                                                 │
│     • Reset for next command                                            │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## Core Components

### 1. Lexer (Tokenization)

**Location:** [src/lexer/](src/lexer/)

The lexer transforms raw input into a linked list of tokens. This is the first processing stage.

#### Entry Point: `lexer_main()` in [lexer.c](src/lexer/lexer.c)

```c
void lexer_main(t_data *data)
{
    cut_lexeur(data);        // Step 1: Split into raw tokens
    identify_tokens(data);   // Step 2: Assign token types
    token_parser(data);      // Step 3: Process quotes and expansions
}
```

#### Step 1: Token Splitting (`cut_lexeur`)

**File:** [identify_tokens.c](src/lexer/identify_tokens.c)

Splits the input string into tokens based on:
- **Whitespace** (spaces, tabs) - token separators
- **Operators** (`|`, `<`, `>`, `<<`, `>>`) - become their own tokens
- **Quotes** - content inside quotes stays together, even with spaces

```
Input:  echo "hello world" | cat
Tokens: [echo] ["hello world"] [|] [cat]
```

Key functions:
- `handle_specials()` - Identifies operator tokens (handles `<<` and `>>` as single tokens)
- `handle_token()` - Extracts regular tokens, skipping over quoted sections

#### Step 2: Token Classification (`identify_tokens`)

**File:** [identify_tokens.c](src/lexer/identify_tokens.c)

Assigns a type to each token:

| Type | Description | Example |
|------|-------------|---------|
| `COMMAND` | First token in a command | `echo`, `cat`, `ls` |
| `STRING` | Arguments | `hello`, `-la`, `file.txt` |
| `PIPE` | Pipe operator | `\|` |
| `REDIRECT_IN` | Input redirection | `<` |
| `REDIRECT_OUT` | Output redirection | `>` |
| `APPEND` | Append redirection | `>>` |
| `HEREDOC` | Here-document | `<<` |
| `STDIN` | Filename after `<` or delimiter after `<<` | |
| `STDOUT` | Filename after `>` or `>>` | |

#### Step 3: Quote and Variable Processing

**Files:** [calc_token_len.c](src/lexer/calc_token_len.c), [copy_token.c](src/lexer/copy_token.c), [token_parser.c](src/lexer/token_parser.c)

This step:
1. **Calculates expanded length** - Pre-computes how big the token will be after variable expansion
2. **Copies with expansion** - Creates new token value with `$VAR` replaced by actual values
3. **Removes quotes** - Strips quote characters while preserving content

```
Input token:  "Hello $USER"
After expansion: "Hello john"
After quote removal: Hello john
```

---

### 2. Parser

**Location:** [src/parser/](src/parser/)

The parser converts the token list into executable command structures.

#### Entry Point: `parsing()` in [parser.c](src/parser/parser.c)

```c
int parsing(t_data *data)
{
    if (!check_tokens_syntax(data))   // Validate syntax
        return (0);
    cut_to_cmds(data);                // Build command list
    if (!process_here_docs(data))     // Handle here-docs
        return (0);
    return (1);
}
```

#### Syntax Validation

**File:** [check_token_syntax.c](src/parser/check_token_syntax.c)

Catches errors like:
- Empty pipes: `| cmd` or `cmd |`
- Consecutive operators: `> >`, `| |`
- Missing redirection targets: `cmd >` (nothing after)
- Invalid operator sequences: `> |`

#### Command Building

**File:** [prepar_cmd.c](src/parser/prepar_cmd.c)

Creates a linked list of `t_cmd` structures, one per command in the pipeline:

```
Input: echo hello | cat -e | wc -l

Creates:
cmd1 → cmd2 → cmd3
 │      │      │
 ├─args: [echo, hello]
 │      ├─args: [cat, -e]
 │      │      └─args: [wc, -l]
```

#### Redirection Extraction

**File:** [handle_redirection.c](src/parser/handle_redirection.c)

Parses redirection tokens and stores them in the command structure:

```c
// For: cat < input.txt > output.txt
cmd->input_file = "input.txt"
cmd->output_file = "output.txt"
cmd->append = 0  // (would be 1 for >>)
```

For here-docs:
```c
// For: cat << EOF
cmd->here_doc = 1
cmd->delimiter = "EOF"
cmd->heredoc_expand = 1  // (0 if delimiter was quoted: << 'EOF')
```

---

### 3. Here-Document Handling

**Location:** [src/here_doc/](src/here_doc/)

Here-documents allow multi-line input until a delimiter is encountered.

#### How It Works

**File:** [here_doc.c](src/here_doc/here_doc.c)

```
$ cat << END
> Hello
> World
> END
Hello
World
```

Implementation:
1. **Fork a child process** - Isolated environment for reading input
2. **Create temp file** - `/tmp/here_doc_0`, `/tmp/here_doc_1`, etc.
3. **Read lines** - Using `readline("> ")` until delimiter matched
4. **Write to temp file** - With optional variable expansion
5. **Parent opens temp file** - As the command's input
6. **Unlink temp file** - File deleted but remains accessible via FD

#### Variable Expansion in Here-Docs

**File:** [expand.c](src/here_doc/expand.c)

```bash
# Expansion enabled (unquoted delimiter)
$ cat << EOF
Hello $USER
EOF
# Output: Hello john

# Expansion disabled (quoted delimiter)
$ cat << 'EOF'
Hello $USER
EOF
# Output: Hello $USER
```

The `heredoc_expand` flag controls this behavior:
- Unquoted delimiter (`<< EOF`) → `heredoc_expand = 1`
- Quoted delimiter (`<< 'EOF'` or `<< "EOF"`) → `heredoc_expand = 0`

---

### 4. Execution

**Location:** [src/execution/](src/execution/)

The execution module runs the parsed commands.

#### Entry Point: `execution()` in [execute.c](src/execution/execute.c)

```c
void execution(t_data *data, t_envp **envp)
{
    // Special case: single built-in without pipes
    // Execute in parent to affect shell state (cd, export, etc.)
    if (data->num_cmds == 1 && cmd->is_built_in)
    {
        solo_build_in_execution_and_handling_of_fd(data, cmd, envp);
        return;
    }

    // General case: fork for each command
    process(data, envp);
}
```

#### Pipeline Execution

**File:** [execute.c](src/execution/execute.c)

For `cmd1 | cmd2 | cmd3`:

```
          pipe[0]      pipe[1]
             │            │
    ┌────────┼────────────┼────────┐
    │        ▼            ▼        │
    │   ┌─────────┐  ┌─────────┐   │
    │   │ write   │  │ write   │   │
cmd1 ──►│ end [1] │  │ end [1] │◄── cmd2
    │   └────┬────┘  └────┬────┘   │
    │        │            │        │
    │   ┌────▼────┐  ┌────▼────┐   │
    │   │ read    │  │ read    │   │
    │   │ end [0] │──►  end [0]│──► cmd3
    │   └─────────┘  └─────────┘   │
    └──────────────────────────────┘
```

Process:
1. Create `n-1` pipes for `n` commands
2. Fork child for each command
3. Child: setup stdin/stdout with `dup2()`, close unused pipes, execute
4. Parent: close all pipes, wait for all children

#### External Command Execution

**File:** [execute_external.c](src/execution/execute_external.c)

```c
void execute_external_function(t_cmd *cmd, t_envp **envp)
{
    char *cmd_path = find_command_path(cmd, *envp);
    char **envp_strs = convert_envp_to_strs(*envp);

    execve(cmd_path, cmd->args, envp_strs);
    // If we reach here, execve failed
}
```

#### Path Resolution

**File:** [get_path.c](src/execution/get_path.c)

1. **Absolute path** (`/bin/ls`) - Check existence and permissions directly
2. **Relative path** (`./script.sh`) - Resolve from current directory
3. **Command name** (`ls`) - Search in PATH directories

```c
// PATH=/usr/bin:/bin:/usr/local/bin
// Command: ls
// Search: /usr/bin/ls, /bin/ls, /usr/local/bin/ls
// Return first that exists and is executable
```

---

### 5. Built-in Commands

**Location:** [src/build_in/](src/build_in/)

Built-ins are executed directly by the shell, not via `execve()`.

| Command | File | Description |
|---------|------|-------------|
| `cd` | [cd.c](src/build_in/cd.c) | Change directory, updates PWD/OLDPWD |
| `echo` | [echo.c](src/build_in/echo.c) | Print arguments, supports `-n` flag |
| `env` | [env.c](src/build_in/env.c) | Print environment variables |
| `exit` | [exit.c](src/build_in/exit.c) | Exit shell with status code |
| `export` | [export.c](src/build_in/export.c) | Set environment variables |
| `pwd` | [pwd.c](src/build_in/pwd.c) | Print working directory |
| `unset` | [unset.c](src/build_in/unset.c) | Remove environment variables |

#### Why Built-ins Run in Parent

Built-ins like `cd` and `export` must modify the shell's state. If they ran in a forked child, changes would be lost when the child exits.

```c
// In execution():
if (data->num_cmds == 1 && cmd->is_built_in)
{
    // Run in parent - changes persist!
    solo_build_in_execution_and_handling_of_fd(data, cmd, envp);
}
```

However, in a pipeline (`export VAR=x | cat`), each command runs in a child, so `export` won't affect the parent shell.

---

### 6. Signal Handling

**Location:** [signals.c](src/signals.c), [signals_execution.c](src/signals_execution.c)

The shell handles signals differently depending on context:

#### Signal Modes

| Mode | Context | Ctrl+C (SIGINT) | Ctrl+\ (SIGQUIT) |
|------|---------|-----------------|-------------------|
| `MAIN_SIGNAL` | Waiting for input | Redisplay prompt | Ignored |
| `CHILD_SIGNAL` | During execution | Newline + exit 130 | "Quit" + exit 131 |
| `HEREDOC_CHILD` | Reading here-doc | Exit 130 | Ignored |
| `HEREDOC_PARENT` | Waiting for here-doc | Set exit code 130 | Ignored |

#### Implementation

```c
void set_signal(int signal_type)
{
    if (signal_type == MAIN_SIGNAL)
    {
        signal(SIGINT, reset_prompt);   // Ctrl+C: new prompt
        signal(SIGQUIT, SIG_IGN);       // Ctrl+\: ignore
    }
    else if (signal_type == CHILD_SIGNAL)
    {
        signal(SIGINT, ctrl_c);         // Ctrl+C: exit 130
        signal(SIGQUIT, ctrl_backslash); // Ctrl+\: exit 131
    }
    // ... other modes
}
```

---

## Data Structures

**Location:** [include/struct.h](include/struct.h)

### Main Data Structure: `t_data`

```c
typedef struct s_data
{
    char    *input;              // Raw input from readline
    int     num_cmds;            // Number of commands in pipeline
    int     exit_code;           // Last command's exit code

    t_tokens    **tokens;        // Array of token lists
    t_envp      *env_var;        // Environment as linked list
    t_cmd       *cmds;           // Command linked list

    char    **here_doc_temp_files;  // Temp file tracking
    // ... redirection fields
} t_data;
```

### Token Structure: `t_tokens`

```c
typedef struct s_tokens
{
    int         position;        // Token index
    char        *value;          // Token string
    t_tok_type  type;            // COMMAND, STRING, PIPE, etc.
    bool        simple_quote;    // Was in single quotes
    bool        was_quoted;      // Was quoted at all

    struct s_tokens *next;
    struct s_tokens *prev;
} t_tokens;
```

### Command Structure: `t_cmd`

```c
typedef struct s_cmd
{
    char    **args;              // Argument array for execve
    int     args_count;
    int     is_built_in;         // 1 if built-in command

    // Redirections
    char    *input_file;         // < filename
    char    *output_file;        // > or >> filename
    int     input_fd;
    int     output_fd;
    int     append;              // 1 for >>, 0 for >

    // Here-doc
    int     here_doc;            // 1 if has here-doc
    char    *delimiter;
    int     heredoc_expand;      // 1 for variable expansion

    struct s_cmd *next;
    struct s_cmd *prev;
} t_cmd;
```

### Environment Structure: `t_envp`

```c
typedef struct s_envp
{
    char    *name;               // Variable name
    char    *value;              // Variable value
    struct s_envp *next;
    struct s_envp *prev;
} t_envp;
```

---

## Quote and Variable Expansion

### Quote Rules

| Quote Type | Variable Expansion | Special Characters |
|------------|-------------------|-------------------|
| No quotes | Yes (`$VAR` → value) | Spaces split tokens |
| Single `'` | No (`$VAR` → literal) | Everything literal |
| Double `"` | Yes (`$VAR` → value) | Spaces preserved |

### Expansion Process

**Location:** [src/lexer/calc_token_len.c](src/lexer/calc_token_len.c), [src/lexer/copy_token.c](src/lexer/copy_token.c)

```
Token: "Hello $USER"

1. Calculate length:
   - "Hello " = 6
   - $USER → lookup "john" = 4
   Total: 10 + null terminator

2. Copy with expansion:
   Result: "Hello john"

3. Remove quotes:
   Final: Hello john
```

### Special Variables

| Variable | Meaning |
|----------|---------|
| `$VAR` | Environment variable value |
| `$?` | Exit code of last command |

---

## Pipes and Redirections

### Pipe Implementation

For `cmd1 | cmd2`:

1. Create pipe: `pipe(pipefd)` → `pipefd[0]` (read), `pipefd[1]` (write)
2. Fork cmd1: stdout → `pipefd[1]`
3. Fork cmd2: stdin → `pipefd[0]`
4. Close pipe ends in parent
5. Wait for both children

### Redirection Implementation

**File:** [src/execution/check_the_acces.c](src/execution/check_the_acces.c)

```c
// Input redirection: < file
open(file, O_RDONLY);
dup2(fd, STDIN_FILENO);

// Output redirection: > file
open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
dup2(fd, STDOUT_FILENO);

// Append redirection: >> file
open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
dup2(fd, STDOUT_FILENO);
```

### Redirection Priority

- Later redirections override earlier ones
- Only the last redirection of each type takes effect
- Example: `< a < b` → reads from `b` only

---

## Compilation

```bash
make        # Build minishell
make clean  # Remove object files
make fclean # Remove objects and executable
make re     # Rebuild from scratch
```

Requirements:
- GCC compiler
- GNU Readline library (`-lreadline`)
- Make

---

## Usage

```bash
./minishell
```

### Examples

```bash
# Simple command
minishell$ ls -la

# Pipeline
minishell$ cat file.txt | grep "pattern" | wc -l

# Redirections
minishell$ echo "Hello" > output.txt
minishell$ cat < input.txt >> output.txt

# Here-document
minishell$ cat << EOF
> Line 1
> Line 2
> EOF

# Variable expansion
minishell$ echo "User: $USER, Status: $?"

# Built-in commands
minishell$ cd /tmp
minishell$ export MY_VAR="hello"
minishell$ echo $MY_VAR
minishell$ unset MY_VAR
minishell$ exit 0
```

---

## License

This project is part of the 42 school curriculum.
