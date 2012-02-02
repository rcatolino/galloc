#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include "block.h"

int initialized=0;
struct block * firstFreeBlock=NULL;
struct block * firstUsedBlock=NULL;
struct bblock * firstBBlock=NULL;

static int merge(struct block * block1, struct block * block2)
{
  int endBlock=0;
  //Remove block2 from list :
  removeFromList(block2);
  block1->size+=block2->size+sizeof(struct block);
  /*
  if (block2->prev){
    block2->prev->next=block1;
  } else endBlock++;
  if (block2->next){
    block2->next->prev=block1;
  } else endBlock++;
  block1->prev=block2->prev;
  block1->next=block2->next;
  */
  return endBlock;
}
static void release(struct block * usedBlock){
  struct block * i;
  int mergeDone=0;
  TRACE("Remove block from used linked list.%d\n",0);
  //Erase it from used linked list :
  removeFromList(usedBlock); 
  //Add block it in front of free linked list
  TRACE("Move block to free linked list.%p\n",usedBlock);
  usedBlock->next=firstFreeBlock; //Add it in front of used block list
  usedBlock->prev=NULL;
  firstFreeBlock->prev=usedBlock;
  firstFreeBlock=usedBlock;
  //Find if the following block in memory is free.
  for (i=firstFreeBlock->next; i!=NULL && mergeDone<2; i=i->next){
    if (END(usedBlock)==i){
      TRACE("Next block in memory %p is also free, merge it with us.\n",i);
      mergeDone+=merge(usedBlock,i);
      mergeDone++;
    }
    if (END(i)==usedBlock){
      TRACE("Previous block in memory %p is also free, merge it with us.\n",i);
      mergeDone+=merge(i,usedBlock);
      mergeDone++;
    }
  }  
}
void free(void * memory) {
  struct block * memBlock;
  struct bblock * hugeBlock;
  TRACE("Block to free in memory at %p.\n",memory);
  if(!initialized)
  {
    TRACE("Nothing has been allocated yet. %d.\n",0);
    return; //No memory could have been allocated
  }
  //First locate mem block in linked list
  for (hugeBlock=firstBBlock; hugeBlock!=NULL; hugeBlock=hugeBlock->next){
    TRACE("Checking huge block at %p.\n",hugeBlock);
    if (BDATA(hugeBlock)==memory){
      TRACE("Found huge block to free at address %p.\n",hugeBlock);
      munmap(hugeBlock,hugeBlock->size);
      return;
    }
  }
  for(memBlock=firstUsedBlock; memBlock!=NULL; memBlock=memBlock->next ){
    TRACE("Checking block at %p.\n",memBlock);
     if (DATA(memBlock)==memory){
       TRACE("Found block to free at address %p.\n",memBlock);
       release(memBlock);
       return;
     }
  }
  return;
}
