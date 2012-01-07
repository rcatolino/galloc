//This file contains definitions needed by both galloc and gfree.
#ifndef __BLOCK_H
#define __BLOCK_H

#ifdef DEBUG
  #define TRACE(a,b) printf(a,b) 
#else
  #define TRACE(a,b)
#endif

#define MMAP_THRESHOLD 0x20000
#define BLOCKSIZE MMAP_THRESHOLD+sizeof(struct block)

//These macro can be usedr to find the data associated 
//to a header or the header associated to the data.
#define DATA(a) (void*)((long)a+sizeof(struct block)) 
#define HEAD(a) (void*)((long)a-sizeof(struct block)) 
#define END(a) (void*)((long)a+sizeof(struct block)+a->size)

struct block {
  unsigned int size;
  struct block * next;
  struct block * prev;
};

extern int initialized;
extern struct block * firstFreeBlock;
extern struct block * firstUsedBlock;

void removeFromList(struct block * toRemove);
void printFreeList();
void printUsedList();
#endif
