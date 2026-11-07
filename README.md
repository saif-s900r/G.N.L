*This activity has been created as part of the 42 curriculum by sbadandi*

# get_next_line

## Description

**get_next_line** is a C function that returns one line at a time from a file descriptor. Each call to `get_next_line(fd)` returns the next line, including its terminating `\n` (if there is one), and returns `NULL` when there is nothing left to read or when an error occurs.

The goal of the project is to practice:
- reading from a file descriptor with `read()` in chunks of a configurable size (`BUFFER_SIZE`),
- **static variables**, which keep data between calls,
- dynamic memory management (`malloc` / `free`) without leaks,
- writing code that behaves correctly for any `BUFFER_SIZE`, for files and for standard input.

Files:
- `get_next_line.c`: `get_next_line`, `read_to_stash`, `extract_line`, `clean_stash`
- `get_next_line_utils.c`: `ft_str_len`, `ft_strjoin`, `ft_strchr`
- `get_next_line.h`: prototypes and the default `BUFFER_SIZE`

## Instructions

### Compilation

The function must be compiled with the `BUFFER_SIZE` macro (the project must also compile with the default value defined in the header):

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

`main.c` is your own test file, for example:

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int main(int argc, char **argv)
{
    int     fd;
    char    *line;

    if (argc != 2)
        return (1);
    fd = open(argv[1], O_RDONLY);
    while ((line = get_next_line(fd)))
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

### Execution

```bash
./gnl file.txt
cat file.txt | ./gnl /dev/stdin
```

### Usage

```c
char *get_next_line(int fd);
```

- Returns the line that was read (with `\n` if present), or `NULL` at end of file or on error.
- The caller must `free()` every returned line.
- It works with any positive `BUFFER_SIZE`, including `1` and very large values.

## Algorithm

The core idea is a **static `stash`**: a heap-allocated string that persists between calls and holds everything that has been read from the file descriptor but not yet returned.

Each call to `get_next_line` does three steps:

1. **`read_to_stash`**: while the stash contains no `\n` and `read()` has not hit the end of file, read `BUFFER_SIZE` bytes into a temporary buffer, null-terminate it, and append it to the stash with `ft_strjoin`. Reading stops as soon as a `\n` appears, so the function never reads more of the file than necessary. On a read error, everything is freed and `NULL` is returned.
2. **`extract_line`**: copy the stash from the beginning up to and including the first `\n` (or up to the end of the string if there is no `\n`) into a new string. This is the value that is returned. If the stash is empty, it returns `NULL`, which signals end of file.
3. **`clean_stash`**: build a new stash containing only the text after the returned line, and free the old one. If nothing is left, the stash is freed and set to `NULL`.

### Why this algorithm?

- **A static stash handles the leftover problem.** With a large `BUFFER_SIZE`, one `read()` can return several lines (or half of a line). The text that follows the returned line cannot be lost or re-read from the file descriptor, so it must be kept between calls. A static variable is the simplest way to do this without global variables.
- **Reading until a `\n` appears** keeps memory usage low and keeps the function correct for pipes and standard input, where the data cannot be read again.
- **Three small functions with one job each** (read, extract, clean) make the code easy to test and fit the Norm's limits on function length.
- **Complexity:** each character is copied a small, constant number of times, and `ft_strjoin` is called once per `read()`. For very long lines this makes the cost proportional to (line length / `BUFFER_SIZE`) copies of a growing stash. This is a known trade-off of this approach. It is acceptable for this project and keeps the code simple, compared to a linked list of buffers, which avoids the repeated copying but needs more code.
- **Memory:** every allocation has a matching `free` on all paths (read error, allocation failure, end of file), so there are no leaks.

### Edge cases handled

- empty file (returns `NULL` immediately)
- last line without a trailing `\n`
- consecutive empty lines
- very long lines and `BUFFER_SIZE` of `1`
- invalid file descriptor or `BUFFER_SIZE <= 0` (returns `NULL`)
- read error (`read()` returns `-1`)

## Resources

### References

- `man 2 read`, `man 3 malloc`, `man 3 free`
- [Static variables in C (GeeksforGeeks)](https://www.geeksforgeeks.org/static-variables-in-c/)
- [File descriptors explained](https://www.codequoi.com/en/handling-a-file-by-its-descriptor-in-c/)
- 42 get_next_line subject PDF
- The 42 Norm (Norminette) documentation

### Use of AI

AI (Claude) was used **only as a debugging and explanation aid**, not to write the project:
- **Debugging:** I pasted the output of a GNL tester and my own functions, and the AI pointed out where logic errors could be (for example, an extra index increment in `clean_stash`, and how `ft_strjoin` handled a `NULL` first argument).
- **Understanding:** it explained why different `BUFFER_SIZE` values exercise different code paths and suggested ways to test (a small `main`, `-fsanitize=address`, comparing with `cat -e`).

I wrote, fixed, and tested the code myself, and I understand and can explain every line of it.
