//This file contains function used by both gfree and galloc
#include "block.h"
#include <stdio.h>

void removeFromList(struct block * toRemove)
{
  TRACE("Removing block %p from list\n",toRemove);
  if(toRemove->prev && toRemove->next){
    TRACE("Block is in the middle of list, previous: %p \n",toRemove->prev);
    toRemove->prev->next=toRemove->next; 
    toRemove->next->prev=toRemove->prev;
  }else if(toRemove->prev){
    TRACE("Block is in back of list, prev :%p \n",toRemove->prev);
    toRemove->prev->next=NULL;
  }else if(toRemove->next){
    if(firstFreeBlock==toRemove) {
      TRACE("Block is in front of free list, next: %p \n",toRemove->next);
      firstFreeBlock=toRemove->next;
    }else if(firstUsedBlock==toRemove){
      TRACE("Block is in front of used list, next: %p \n",toRemove->next);
      firstUsedBlock=toRemove->next;
    }
    toRemove->next->prev=NULL;
  }else{
    TRACE("Block %p is last element from list.\n",toRemove);
    if(firstFreeBlock==toRemove) {
      firstFreeBlock=NULL;
    }else if(firstUsedBlock==toRemove){
      firstUsedBlock=NULL;
    }
  }
}

void printFreeList()
{
  struct block * i;
  for(i=firstFreeBlock; i!=NULL; i=i->next)
  {
    printf("Free block at %p with %x bytes\n",i,i->size);
  }
}
void printUsedList()
{
  struct block * i;
  for(i=firstUsedBlock; i!=NULL; i=i->next)
  {
    printf("Used block at %p with %x bytes\n",i,i->size);
  }
}
