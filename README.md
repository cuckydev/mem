# mem
A tiny portable C89 memory allocator.

## Usage
This is a single-header library. You must include this file alongside `#define MEM_IMPLEMENTATION` in one file in order to use.

You can then include `mem.h` in other files for the function declarations.

## Additional control defines
Define | Function
--------|--------
`MEM_STAT` | This will enable the Mem_GetStat function which returns information about available memory in the heap.

## Licensing
This library is available under both the MIT license and to the public domain. Choose whichever one you prefer.
