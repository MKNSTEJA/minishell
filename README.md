# minishell

A robust, lightweight shell implementation written in C. This project re-implements the **core interactive behavior of `bash`** (Bourne Again SHell) with a focus on **process management**, **file descriptor manipulation**, **signal handling**, and **environment management**.

> **Scope note (42 minishell):** This is an educational re-implementation of a subset of a POSIX-like shell. It is **not** intended to be a drop-in replacement for `bash`.

---

## Table of Contents

* [Overview](#overview)
* [Features](#features)
* [Supported Syntax](#supported-syntax)
* [Built-ins](#built-ins)
* [Architecture](#architecture)
* [Installation](#installation)
* [Usage](#usage)
* [Project Structure](#project-structure)
* [Testing & Debugging](#testing--debugging)
* [Limitations](#limitations)
* [Contributing](#contributing)
* [License](#license)

---

## Overview

`minishell` is a small interactive shell that:

* reads and parses user input (including quotes and expansions),
* builds an execution plan (commands, pipes, and redirections),
* runs external programs via `fork()`/`execve()` with correct I/O wiring,
* implements common shell built-ins that must run **inside** the shell process,
* tracks exit status and exposes it via `$?`.

The codebase is split into **parsing** and **execution** layers to keep responsibilities clear and to make complex behaviors (pipelines, redirections, heredocs) maintainable.

---

## Features

### Interactive UX

* **Readline-based prompt** (line editing + history navigation)
* **Graceful signal behavior** for an interactive shell

### Execution

* Execute binaries from:

  * `$PATH` resolution
  * absolute paths (`/bin/ls`)
  * relative paths (`./a.out`)
* Correct process lifecycle using **`fork()` + `execve()`**
* Proper exit-code propagation (including pipeline exit rules)

### Quotes & Expansions

* **Single quotes** (`'...'`) prevent interpretation/expansion
* **Double quotes** (`"..."`) allow variable expansion while preserving spaces
* Environment variable expansion: `$VAR`, `$?`

### Redirections & Pipelines

* Pipe: `|`
* Input: `<`
* Output truncate: `>`
* Output append: `>>`
* Heredoc: `<<` (reads until a delimiter)

---

## Supported Syntax

This minishell supports a practical subset of common shell syntax:

### Pipelines

```bash
ls -la | grep "\.c" | wc -l
```

### Redirections

```bash
cat < infile
cat < infile > outfile
cat < infile >> outfile
```

### Heredoc

```bash
cat << EOF
hello $USER
EOF
```

### Quotes

```bash
echo "Hello $USER"   # expands

echo 'Hello $USER'   # no expansion
```

### Exit status

```bash
false
echo $?
```

---

## Built-ins

Built-ins are implemented internally because they must affect the shell state (environment, current directory, exit code):

* `cd` — change directory (supports relative/absolute paths and `~` when applicable)
* `echo` — print text (supports `-n`)
* `pwd` — print current working directory
* `export` — set environment variables
* `unset` — remove environment variables
* `env` — display environment variables
* `exit` — terminate the shell with an optional status code

---

## Architecture

The project is organized into two main components: **Parsing** and **Execution**.

### 1) Lexer & Parser

A custom lexical analyzer converts the input line into structured tokens and command nodes:

* **Tokenization**: categorizes input as `WORD`, `PIPE`, `REDIR`, etc.
* **Segmentation**: quote-aware segmentation allows mixed strings (e.g., `echo "Hello"world`)
* **Expansion**: expands `$VAR` and `$?` before execution (with quote rules)

### 2) Execution Engine

The execution layer traverses the parsed command list and performs:

* **Pipeline creation** using UNIX pipes
* **Redirection chaining** using `open()` + `dup2()`
* **Process spawning** using `fork()` and `execve()`
* **Wait / exit status collection** for correct `$?` behavior

---

## Installation

### Prerequisites

* A POSIX-like environment (Linux or macOS)
* `make`
* `gcc` or `clang`
* `readline` development headers

### Build

```bash
git clone https://github.com/Neko-Bytes/minishell
cd minishell
make
```

> If your system does not ship `readline` headers by default, install the development package (e.g., `libreadline-dev` on Debian/Ubuntu, `readline` on macOS via package manager).

---

## Usage

Run the shell:

```bash
./minishell
```

Typical usage:

```bash
minishell$ echo "hello" | cat -e
minishell$ export NAME=world
minishell$ echo "$NAME"
minishell$ cat << EOF
> line 1
> line 2
> EOF
```

Exit:

```bash
minishell$ exit
```

---

## Project Structure

High-level layout (names may vary slightly by branch):

```text
.
├── include/        # Public headers
├── libft/          # 42 libft dependency (vendored)
├── parsing/        # Lexer/parser/expansion logic
├── src/            # Execution engine + built-ins + runtime
├── testdir/        # Local testing assets/scripts (if present)
├── outfiles/       # Output artifacts for tests (if present)
├── Makefile
└── README.md
```

---

## Testing & Debugging

### Manual comparison against bash

A reliable way to validate behavior is to compare outputs and exit statuses side-by-side:

```bash
# bash
bash$ echo "a  b" | wc -w
bash$ echo $?

# minishell
minishell$ echo "a  b" | wc -w
minishell$ echo $?
```

### Debugging tips

* Use **Valgrind** to verify memory and FD hygiene:

  ```bash
  valgrind --leak-check=full --track-fds=yes ./minishell
  ```
* Validate signals interactively:

  * `Ctrl-C` should interrupt and show a fresh prompt
  * `Ctrl-D` should exit the shell (EOF)
  * `Ctrl-\` should not kill the shell at the prompt

---

## Limitations

This project intentionally focuses on the 42 minishell scope. Commonly out-of-scope features include:

* Job control (`fg`, `bg`, `jobs`)
* Shell scripting (`if`, `for`, `while`, functions)
* Wildcard/globbing (`*`) and advanced pattern expansion
* Subshells and command substitution (`$(...)`, backticks)

---

