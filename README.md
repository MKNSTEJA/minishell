# MiniShell

A robust, lightweight shell implementation written in C. This project replicates the core functionalities of `bash` (Bourne Again SHell), focusing on process management, file descriptor manipulation, and signal handling.

It features a custom-built parser, a modular execution engine, and a fully functional environment variable management system.

---

## 🚀 Features

### Core Functionality
- **Command History:** Persistent history navigation using the readline library.
- **System Executables:** Executes binaries found in the system `$PATH` or via absolute/relative paths.
- **Process Isolation:** Robust `fork` and `execve` implementation to run commands in separate child processes.
- **Quote Handling:** - Single quotes (`'`) prevent interpretation of meta-characters.
  - Double quotes (`"`) allow variable expansion (`$VAR`) while preserving spacing.

### I/O Redirections & Pipelines
- **Pipes (`|`):** Connects the standard output of one command to the standard input of the next.
- **Input (`<`):** Redirects input from a file.
- **Output (`>`):** Redirects output to a file (overwrites).
- **Append (`>>`):** Redirects output to a file (appends).
- **Here-Doc (`<<`):** Reads input until a delimiter is found, supporting variable expansion within the input stream.

### Signal Handling
- **`Ctrl-C`:** Interrupts the current process and displays a new prompt.
- **`Ctrl-D`:** Exits the shell (sends EOF).
- **`Ctrl-\`:** Quits the current process (does nothing in the prompt).

### Built-in Commands
Minishell includes its own implementation of the following built-ins to manipulate the shell state directly:
- `cd`: Change directory (supports relative/absolute paths and `~`).
- `echo`: Print text to standard output (supports `-n` flag).
- `pwd`: Print the current working directory.
- `export`: Set environment variables.
- `unset`: Remove environment variables.
- `env`: Display current environment variables.
- `exit`: Terminate the shell with a status code.

---

## 🛠️ Architecture

The project is structured into two main components: **Parsing** and **Execution**.

### 1. Lexer & Parser
The input string is processed through a custom lexical analyzer:
- **Tokenization:** The input is split into tokens (`t_split`), categorizing text as `WORD`, `PIPE`, `REDIR`, etc.
- **Segmentation:** Quotes are handled by creating "segments" (`t_segment`), allowing mixed-quote strings like `echo "Hello" world`.
- **Expansion:** Environment variables (`$VAR`) are expanded in-place before execution.

### 2. Execution Engine
The executor traverses the parsed command list (`t_op`):
- **Pipeline Management:** Uses a loop to spawn child processes, linking them via standard UNIX pipes.
- **File Descriptors:** Manages `dup2` calls to handle complex redirection chains (e.g., `cmd1 < infile | cmd2 >> outfile`).
- **Wait Handling:** The parent process waits for the entire pipeline to finish, collecting exit codes (`$?`) appropriately.

---

## 📦 Installation

### Prerequisites
- `gcc` or `clang`
- `make`
- `readline` library (usually installed by default on macOS, requires dev package on Linux).

**For Linux (Debian/Ubuntu):**
```bash
sudo apt-get install libreadline-dev
