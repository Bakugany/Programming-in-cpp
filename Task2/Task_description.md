# String Queues

## Introduction

The C++ Standard Library provides very useful containers such as `unordered_map` and `deque` that are not available in the C library.

Often, it is necessary to combine C++ code with legacy C code. The goal of this assignment is to write a C++ module that handles string queues so that they can be used in C. The module consists of a header file (with a `.h` extension) and an implementation file (with a `.cpp` extension).

In solving this assignment, students should learn about:

- Additional STL containers,
- How to combine C++ code with C code,
- Methods for initializing global objects in C++ and the resulting issues,
- The preprocessor and elements of conditional compilation.

## Task

The `strqueue` module should provide the following functions:

### `unsigned long strqueue_new();`

Creates a new, empty string queue and returns its identifier.

### `void strqueue_delete(unsigned long id);`

If a string queue with the identifier `id` exists, deletes it; otherwise, does nothing.

### `size_t strqueue_size(unsigned long id);`

If a string queue with the identifier `id` exists, returns the number of its elements; otherwise, returns 0.

### `void strqueue_insert_at(unsigned long id, size_t position, const char* str);`

If a string queue with the identifier `id` exists and `str != NULL`, inserts the string `str` before the position `position`, or at the end of the queue if the value of `position` is greater than or equal to the number of strings in the queue. If the queue does not exist or `str == NULL`, then does nothing. Positions in the queue are numbered starting from zero.

### `void strqueue_remove_at(unsigned long id, size_t position);`

If a string queue with the identifier `id` exists and the value of `position` is less than the number of strings in the queue, removes the string at position `position`; otherwise, does nothing.

### `const char* strqueue_get_at(unsigned long id, size_t position);`

If a string queue with the identifier `id` exists and the value of `position` is less than the number of strings in the queue, returns a pointer to the string at position `position`; otherwise, returns `NULL`.

### `void strqueue_clear(unsigned long id);`

If a string queue with the identifier `id` exists, removes all strings from it; otherwise, does nothing.

### `int strqueue_comp(unsigned long id1, unsigned long id2);`

Lexicographically compares the string queues with identifiers `id1` and `id2`, returning:

- `-1` when `queue(id1) < queue(id2)`,
- `0` when `queue(id1) == queue(id2)`,
- `1` when `queue(id1) > queue(id2)`.

If a string queue corresponding to one of the identifiers does not exist, it is treated as an empty queue.
