/*
	"mem" by Regan "CuckyDev" Green:
	A tiny portable C89 memory allocator.
	
	This is a single-header library. You must include this file alongside `#define MEM_IMPLEMENTATION` in one file in order to use.
	You can then include `mem.h` in other files for the function declarations.
	
	Additional control defines:
	MEM_STAT - This will enable the Mem_GetStat function which returns information about available memory in the heap.
	
	See the bottom of the file for licensing information.
*/

#ifndef MEM_GUARD_MEM_H
#define MEM_GUARD_MEM_H

#include <stddef.h>

/* Constants and macros */
#define MEM_ALIGNSIZE 0x10
#define MEM_ALIGN(x) (((size_t)(x) + 0xF) & ~0xF)

/* Function declarations */
int Mem_Init(void *ptr, size_t size);
void *Mem_Alloc(size_t size);
void Mem_Free(void *ptr);
#ifdef MEM_STAT
	void Mem_GetStat(size_t *used, size_t *size, size_t *max);
#endif

#endif /* MEM_GUARD_MEM_H */

/* Implementation */
#ifdef MEM_IMPLEMENTATION

#ifndef MEM_IMPLEMENTATION_GUARD
#define MEM_IMPLEMENTATION_GUARD

typedef struct Mem_Header
{
	struct Mem_Header *prev, *next;
	size_t size;
} Mem_Header;
#define MEM_HEDSIZE (MEM_ALIGN(sizeof(Mem_Header)))

static Mem_Header *mem = NULL;
#ifdef MEM_STAT
	static size_t mem_used, mem_max;
#endif

int Mem_Init(void *ptr, size_t size)
{
	/* Make sure there's enough space for mem header */
	if (ptr == NULL || size < MEM_HEDSIZE)
		return 1;
	
	/* Get mem pointer and available range (after 16 byte alignment) */
	mem = (Mem_Header*)MEM_ALIGN(ptr);
	
	/* Initial mem header */
	mem->prev = NULL;
	mem->next = NULL;
	mem->size = ((char*)ptr + size) - (char*)mem;
	
	/* Initial mem state */
	#ifdef MEM_STAT
		mem_max = mem_used = MEM_HEDSIZE;
	#endif
	
	return 0;
}

static Mem_Header *Mem_GetHeader(void *ptr)
{
	if (ptr == NULL)
		return NULL;
	return (Mem_Header*)((char*)ptr - MEM_HEDSIZE);
}

void *Mem_Alloc(size_t size)
{
	/* Ensure we have a heap */
	if (mem == NULL)
		return NULL;
	
	/* Get true size we have to fit */
	size = MEM_ALIGN(size + MEM_HEDSIZE);
	
	/* Get header pointer */
	Mem_Header *head, *prev, *next;
	char *hpos = (char*)mem + MEM_HEDSIZE;
	
	prev = mem;
	next = prev->next;
	
	while (1)
	{
		if (next != NULL)
		{
			/* Check against the next block */
			size_t cleft = (char*)next - hpos;
			if (cleft >= size)
			{
				/* Set pointer */
				head = (Mem_Header*)hpos;
				break;
			}
			
			/* Check next header */
			hpos = (char*)next + next->size;
			prev = next;
			next = prev->next;
		}
		else
		{
			/* Check against end of heap */
			size_t cleft = ((char*)mem + mem->size) - hpos;
			if (cleft < size)
				return NULL;
			
			/* Set pointer */
			head = (Mem_Header*)hpos;
			break;
		}
	}
	
	/* Link header */
	head->size = size;
	head->prev = prev;
	if ((head->next = prev->next) != NULL)
		head->next->prev = head;
	prev->next = head;
	
	#ifdef MEM_STAT
		/* Update stats */
		if ((mem_used += size) >= mem_max)
			mem_max = mem_used;
	#endif
	
	return (void*)(hpos + MEM_HEDSIZE);
}

void Mem_Free(void *ptr)
{
	/* Get header of pointer */
	if (ptr == NULL)
		return;
	Mem_Header *head = Mem_GetHeader(ptr);
	
	/* Unlink header */
	if ((head->prev->next = head->next) != NULL)
		head->next->prev = head->prev;
	
	#ifdef MEM_STAT
		/* Update stats */
		mem_used -= head->size;
	#endif
}

#ifdef MEM_STAT
	void Mem_GetStat(size_t *used, size_t *size, size_t *max)
	{
		if (used != NULL)
			*used = mem_used;
		if (size != NULL)
			*size = mem->size;
		if (max != NULL)
			*max = mem_max;
	}
#endif

#endif /* MEM_IMPLEMENTATION_GUARD */
#endif /* MEM_IMPLEMENTATION */

/*
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2021 Regan "CuckyDev" Green
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
