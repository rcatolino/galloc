#include <unistd.h>
#include <stdio.h>
#include "block.h"

void release(struct block * usedBlock){
  struct block * i;
  //Erase it from used linked list :
  usedBlock->prev->next=usedBlock->next; 
  //Find if the following block in memory is free.
  for (i=firstFreeBlock; i!=NULL; i=i->next){
    if (END(usedBlock)==i){
      TRACE("Next block in memory %p is also free, merge it with us.\n",i);
      usedBlock->next=i;
      usedBlock->prev=i->prev;
      i->prev->next=usedBlock;
      i->prev=usedBlock;
    }
  }
}
void free(void * memory) {
  struct block * memBlock;
  if(!initialized)
    return; //No memory could have been allocated
  //First locate mem block in linked list
  for(memBlock=firstUsedBlock; memBlock!=NULL; memBlock=memBlock->next ){
     if (DATA(memBlock)==memory){
       TRACE("Found block to free at address %p.\n",memBlock);
       release(memBlock);
     }
  }
  return;
}
