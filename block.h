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

struct block {
  unsigned int size;
  struct block * next;
  struct block * prev;
};

static int initialized=0;

#endif
