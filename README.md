# Pipex

## Summary  
This repository implements the **Pipex** project—a minimal recreation of Unix shell piping in C. It takes an input file, executes a sequence of commands where each command’s output is piped to the next, and writes the final output to an output file. Under the hood, it uses `pipe()`, `fork()`, `dup2()`, and `execve()` to link processes, and relies on a custom Libft for utility functions. :contentReference[oaicite:0]{index=0}

## Repository Structure  
- **`.vscode/`**  
  - Configuration files for Visual Studio Code (e.g., `settings.json`, `launch.json`).  
- **`libft/`**  
  - A custom Libft library containing reimplemented C standard‐library routines (e.g., `ft_split`, `ft_strjoin`, `ft_strdup`, `ft_strlen`, `ft_strncmp`, `ft_substr`) that Pipex depends on for string manipulation and memory tasks.  
- **`Makefile`**  
  - Build instructions:  
    - **`make`** (default):  
      1. Compiles Libft (into `libft/libft.a`).  
      2. Compiles `pipex.c` and `pipex_helper.c` and links against `-lft` to produce the `pipex` executable.  
    - **`make clean`**: Removes object files in the root and in `libft/`.  
    - **`make fclean`**: Runs `clean` and also removes `libft.a` and the `pipex` binary.  
    - **`make re`**: Runs `fclean` then `make` to rebuild from scratch. :contentReference[oaicite:1]{index=1}  
- **`pipex.h`**  
  - Header file that declares all necessary function prototypes and includes for Pipex:  
    - Standard headers (`<unistd.h>`, `<stdlib.h>`, `<fcntl.h>`, `<sys/wait.h>`)  
    - Libft header (`libft/libft.h`)  
    - Macros for error messages and constants (e.g., `#define ERR_ARG "Error: invalid arguments\n"`).  
- **`pipex.c`**  
  - Contains the `main()` function:  
    1. Validates that exactly **5** arguments are provided:  
       ```bash
       ./pipex infile cmd1 cmd2 outfile
       ```  
       (Alternatively, a more advanced version may accept more than two commands in between.)  
    2. Opens `infile` for reading and `outfile` for writing (with `O_CREAT | O_WRONLY | O_TRUNC`, mode `0644`).  
    3. Creates a pipe (an array `int pipefd[2]`).  
    4. **First child process** (`fork()`):  
       - Uses `dup2(pipefd[1], STDOUT_FILENO)` to redirect its standard output into `pipefd[1]`.  
       - Uses `dup2(infile_fd, STDIN_FILENO)` to take input from `infile`.  
       - Closes unused file descriptors.  
       - Parses `cmd1` (splits it into `argv[]` using Libft’s `ft_split()`), finds its full path via `get_path()`, then calls `execve()`.  
    5. **Second child process** (`fork()`):  
       - Uses `dup2(pipefd[0], STDIN_FILENO)` to take input from `pipefd[0]`.  
       - Uses `dup2(outfile_fd, STDOUT_FILENO)` to write its standard output to `outfile`.  
       - Closes unused descriptors.  
       - Parses `cmd2`, resolves its path, then `execve()`.  
    6. Parent closes both ends of the pipe and waits for both children to finish.  
    7. On any `open()` or `pipe()` or `fork()` failure, prints an error message and exits. :contentReference[oaicite:2]{index=2}  
- **`pipex_helper.c`**  
  - Helper routines used by `pipex.c`:  
    - **`get_path()`**:  
      - Scans the `PATH` environment variable (using `getenv("PATH")`), splits it by colons, and appends `/cmd` to each directory to locate the executable (using `access(..., X_OK)`).  
      - Returns the fully qualified path to the executable or `NULL` if not found.  
    - **`exec_cmd()`**:  
      - Wraps `execve()` calls: takes a `cmd_str` and `envp`, calls `get_path()`, then `execve()`. On failure, prints `cmd: command not found\n` and exits.  
    - **`free_split(char **arr)`**:  
      - Frees a NULL-terminated array of strings (used to clean up after `ft_split()`).  
    - **`error_exit(char *msg)`**:  
      - Prints the given `msg` to `stderr` and calls `exit(EXIT_FAILURE)`. :contentReference[oaicite:3]{index=3}  
- **`pipex.sh`**  
  - A shell script demonstrating typical usage and testing:  
    ```bash
    #!/bin/bash
    # Example: ./pipex infile "grep something" "wc -l" outfile
    VALGRIND=$(command -v valgrind)
    if [ -z "$VALGRIND" ]; then
      echo "Valgrind not installed"
    else
      echo "Running under Valgrind:"
      valgrind --leak-check=full ./pipex infile "grep foo" "wc -l" outfile
    fi
    ```
  - Can be used to validate memory leaks and correct file descriptor usage. :contentReference[oaicite:4]{index=4}  
- **`text.txt`**  
  - Contains additional usage notes or examples (e.g., sample commands and expected output). Likely used as a quick reference.  

## Languages and Tools  
- **C (96.6%)**: Core implementation uses ISO C (C99/C11) system calls (`fork()`, `execve()`, `pipe()`, `dup2()`, `open()`, `close()`, `waitpid()`), and standard library functions from Libft (e.g., `ft_split()`, `ft_strjoin()`, `ft_substr()`, `ft_strlen()`). :contentReference[oaicite:5]{index=5}  
- **Makefile (2.7%)**: Automates compilation and linking of Pipex with a custom Libft.  
- **Shell (0.7%)**: `pipex.sh` for demonstration/testing and any CI hooks.  

## How to Build and Run  

1. **Clone the repository**  
   ```bash
   git clone https://github.com/yuhi-ootani/Pipex.git
   cd Pipex
   ``` :contentReference[oaicite:6]{index=6}  

2. **Compile**  
   ```bash
   make
