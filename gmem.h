//This file contains definitions needed by the users
//of the ghome memory manager.

#ifndef __GMEM_H
#define __GMEM_H

void * gmalloc(int size);
void gfree(void * memory);

#endif
