#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define MMAP_THRESHOLD 0x20000
#define BLOCKSIZE MMAP_THRESHOLD+sizeof(struct block)

struct block {
  int size;
  struct block * next;
  struct block * prev;
};

static int initialized=0;
static struct block * firstFreeBlock=NULL;
static struct block * firstUsedBlock=NULL;

static void * blkalloc(int blocksize)
{
  void* addr = NULL;
  addr = sbrk(blocksize);
  printf ("New break address is %p\n",sbrk(0));
  return addr;
}

static struct block * getBlock(struct block * lastBlock)
{
  struct block * newBlock=NULL;
  newBlock=blkalloc(BLOCKSIZE);
  newBlock->size=MMAP_THRESHOLD;
  newBlock->next=NULL;
  newBlock->prev=lastBlock;
  if (lastBlock)
    lastBlock->next=newBlock;
  return newBlock;
}
static struct block * split(struct block * freeBlock, int newBlockSize)
{
  struct block * usedBlock = NULL;
  freeBlock->size-=(newBlockSize+sizeof(struct block));
  if (freeBlock->size==0)
  {
    freeBlock->size+=(newBlockSize+sizeof(struct block));
    freeBlock->prev->next=freeBlock->next; //Delete block from free block list
    freeBlock->next=firstUsedBlock;
    firstUsedBlock=freeBlock;
    return freeBlock;
  }
  usedBlock=freeBlock+sizeof(struct block)+freeBlock->size;
  usedBlock->size=newBlockSize;
  usedBlock->prev=NULL;
  usedBlock->next=firstUsedBlock;
  firstUsedBlock=usedBlock;
  return usedBlock;
}
static void * alloc(int requestSize)
{
  struct block * i;
  struct block * pb=firstFreeBlock;
  struct block * bb=NULL;
  int bestSize = BLOCKSIZE+1;

  //Best Fit Test wether we have enough memory for request :
  for (i=firstFreeBlock; i!=NULL; i=i->next)
  {
    if (i->size>=requestSize && i->size<bestSize) //This block is ok.
    {
      bb=i;
      bestSize=i->size;
    }
    pb=i;
  }
  if (!bb)
  {
    //We need another block :
    getBlock(pb);
    i=split(pb,requestSize);
  } else {
    i=split(bb,requestSize);
  }
  return i;
}
void * galloc(int requestSize)
{
  if (!initialized)
  {
    firstFreeBlock=getBlock(NULL);
    initialized=1;
  }
  return alloc(requestSize);
}
//Test function :
int main(int argc, char * argv[])
{
  int * a = malloc(sizeof(int));
  *a = 4;
  return 0;
}

