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
  int length=1;
  unsigned long int total=0;
  struct block * i;
  for(i=firstFreeBlock; i!=NULL; i=i->next)
  {
    length++;
    total+=i->size;
    //printf("Free block at %p with %x bytes\n",i,i->size);
  }
  printf("There is %d blocks free, with an average of %ld \
bytes per block.\n",length,total/length);
  printf("The total free \
memory is %ld bytes\n",total);
}
void printUsedList()
{
  int length=1;
  unsigned long int total=0;
  struct block * i;
  for(i=firstUsedBlock; i!=NULL; i=i->next)
  {
    length++;
    total+=i->size;
  //  printf("Used block at %p with %x bytes\n",i,i->size);
  }
printf("There is %d blocks allocated, with an average of %ld \
bytes per block.\n",length,total/length);
printf("The total allocated memory is %ld bytes, the total used \
memory is %ld bytes\n",BLOCKSIZE+total%BLOCKSIZE,total);
}
