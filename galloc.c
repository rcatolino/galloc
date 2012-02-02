#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include "block.h"

static void * blkalloc(int blocksize)
{
  void* addr = NULL;
  TRACE("Old break address is %p\n",sbrk(0));
  addr = sbrk(blocksize);
  TRACE("New break address is %p\n",sbrk(0));
  return addr;
}

static struct block * getBlock(struct block * lastBlock)
{
  struct block * newBlock=NULL;
  TRACE("Allocating new 0x%zx bytes block.\n",BLOCKSIZE);
  newBlock=blkalloc(BLOCKSIZE);
  TRACE("New block address is %p.\n",newBlock);
  newBlock->size=MMAP_THRESHOLD;
  newBlock->next=NULL;
  newBlock->prev=lastBlock;
  TRACE("Is this the first block?. %p\n",lastBlock);
  if (lastBlock)
  {
    if (lastBlock->next){
      lastBlock->next->prev=newBlock;
      newBlock->next=lastBlock->next;
    }
    TRACE("No, adding block in free linked list, %p\n",sbrk(0));
    lastBlock->next=newBlock;
  } else {
    TRACE("Yes, %p\n",sbrk(0));
  }
  return newBlock;
}
static struct block * split(struct block * freeBlock, int newBlockSize)
{
  struct block * usedBlock = NULL;
  TRACE("Shrinking free block by 0x%zx bytes\n",newBlockSize+sizeof(struct block));
  if (freeBlock->size<newBlockSize+sizeof(struct block))
  {
    TRACE("Free block %p is too small to shrink,\
 move it to used linked list instead\n",freeBlock);
    removeFromList(freeBlock);
    freeBlock->next=firstUsedBlock; //Add it in front of used block list
    freeBlock->prev=NULL;
    firstUsedBlock->prev=freeBlock;
    firstUsedBlock=freeBlock;
    return freeBlock;
  }
  freeBlock->size-=(newBlockSize+sizeof(struct block));
  TRACE("Create new used block of size %d bytes with this spare space.\n",newBlockSize); 
  TRACE("Free block size is now 0x%x bytes.\n",freeBlock->size);
  usedBlock=(void*)((long)freeBlock+sizeof (struct block)+freeBlock->size);
  TRACE("New block address is %p.\n",usedBlock);
  usedBlock->size=newBlockSize;
  usedBlock->prev=NULL;
  usedBlock->next=firstUsedBlock;
  if (firstUsedBlock)
    firstUsedBlock->prev=usedBlock;
  firstUsedBlock=usedBlock;
  return usedBlock;
}
static void * alloc(int requestSize)
{
  struct block * i;
  struct block * pb=firstFreeBlock;
  struct block * bb=NULL;
  int bestSize = BLOCKSIZE+1;

  TRACE("Allocating 0x%x bytes of memory.\n",requestSize);
  //Best Fit, Test wether we have enough memory for request :
  for (i=firstFreeBlock; i!=NULL; i=i->next)
  {
    if (i->size>=requestSize && i->size<bestSize) //This block is ok.
    {
      TRACE("Found good block with 0x%x bytes free.\n",i->size);
      bb=i;
      bestSize=i->size;
    }
    pb=i;
  }
  if (!bb)
  {
    TRACE("No suitable block found for 0x%x bytes.\n",requestSize);
    //We need another block :
    TRACE("Append block after end of list: %p\n",pb);
    pb=getBlock(pb);
    i=split(pb,requestSize);
  } else {
    i=split(bb,requestSize);
  }
  TRACE("Suitable block available at %p.\n",i);
  return DATA(i);
}
void * malloc(int requestSize)
{
  struct bblock * hugeBlock;
  TRACE("0x%x bytes requested\n", requestSize);
  TRACE("Header size is 0x%zx.\n",sizeof(struct block));
  if (!initialized)
  {
    TRACE("First call, initializing, %p\n",sbrk(0));
    firstFreeBlock=getBlock(NULL);
    initialized=1;
  }
  if (requestSize>MMAP_THRESHOLD)
  {
    TRACE("Mapping 0x%lx bytes of memory\n",requestSize+sizeof(struct block) );
    hugeBlock=mmap(
    0, //Let the kernel decide which address to use for this mapping
    requestSize+sizeof(struct bblock), //Length of memory thus allocated
    PROT_READ | PROT_WRITE, //Memory won't be executable
    MAP_PRIVATE | MAP_ANONYMOUS, //Just rtfm...
    -1,0);
    TRACE("Got memory, at address %p\n",hugeBlock);
    hugeBlock->next=firstBBlock;
    hugeBlock->size=requestSize+sizeof(struct block);
    firstBBlock=hugeBlock;
    return DATA(hugeBlock);
  }
  return alloc(requestSize+requestSize%4); //to align mem blocks
}

