# C Programming Assignments

A collection of academic projects and algorithms implemented in C during the Programming and Data Structures course. 

## Contents

### 1. Rotating Polygon
A graphical program rendering a smoothly rotating and scaling polygon using the Simple Graphics Library (SGL). It demonstrates handling continuous animation loops, managing rotation angles without overflow, and basic 2D transformations.

### 2. Text Processing (Octal Sum)
A utility that reads streams of text from standard input, identifies positive octal numbers, and computes their total sum. It utilizes dynamic memory allocation (`malloc`, `realloc`) to handle infinitely long lines of input and processes addition character-by-character to avoid integer overflow limits.

### 3. Custom `strtol` Implementation
A custom, from-scratch implementation of the standard C library function `strtol()`. It safely converts strings to long integers with support for bases from 2 to 36, carefully handling potential overflows and negative numbers at every step of the conversion.

### 4. Custom `bsearch` Implementation
A generic implementation of the standard C library binary search function `bsearch()`. It utilizes function pointers for custom elements comparison and handles generic memory mapping using raw `void*` pointers to safely and efficiently locate elements within sorted arrays.

---

## Technical Highlights
* **Compiler Compliance**: All projects strictly compiled using `-g`, `-Wall`, `-Wextra`, and `-pedantic` flags to ensure standard adherence and memory safety.
* **Low-Level Control**: Focus on dynamic memory tracking, pointer arithmetic, and generic data handling in C.
