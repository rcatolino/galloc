#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define MMAP_THRESHOLD 0x20000
#define BLOCKSIZE MMAP_THRESHOLD+sizeof(struct block)

struct block {
  int size;
  struct block * next;
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

static struct block * getBlock()
{
  struct block * newBlock=NULL;
  newBlock=blkalloc(BLOCKSIZE);
  newBlock->size=MMAP_THRESHOLD;
  newBlock->next=NULL;
  return newBlock;
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
    pb->next=getBlock();
  }

}
void * galloc(int requestSize)
{
  if (!initialized)
  {
    firstFreeBlock=getBlock();
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

