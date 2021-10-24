/*
	"mem" by Regan "CuckyDev" Green
	A tiny portable C89 memory allocator
	This source form is released to the public domain
*/

#ifndef GUARD_MEM_MEM_H
#define GUARD_MEM_MEM_H

#include <stddef.h>

#define MEM_ALIGNSIZE 0x10
#define MEM_ALIGN(x) (((size_t)(x) + 0xF) & ~0xF)

//#define MEM_STAT (Enables functionality that exposes some memory statistics)

u8 Mem_Init(void *ptr, size_t size);
void *Mem_Alloc(size_t size);
void Mem_Free(void *ptr);
#ifdef MEM_STAT
	void Mem_GetStat(size_t *used, size_t *size, size_t *max);
#endif

#endif
