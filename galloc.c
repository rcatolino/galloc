#include <unistd.h>
#include <stdio.h>

#define TRACE(a,b) printf(a,b)
#define MMAP_THRESHOLD 0x20000
#define BLOCKSIZE MMAP_THRESHOLD+sizeof(struct block)

struct block {
  unsigned int size;
  struct block * next;
  struct block * prev;
};

static int initialized=0;
static struct block * firstFreeBlock=NULL;
static struct block * firstUsedBlock=NULL;

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
  TRACE("Is this the first block?.\n",newBlock);
  if (lastBlock)
  {
    TRACE("No, adding block at end of free linked list, %p\n",sbrk(0));
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
  freeBlock->size-=(newBlockSize+sizeof(struct block));
  if (freeBlock->size==0)
  {
    TRACE("Free block would become empty, move it to used linked list instead, %d\n",0);
    freeBlock->size+=(newBlockSize+sizeof(struct block));
    freeBlock->prev->next=freeBlock->next; //Delete block from free block list
    freeBlock->next=firstUsedBlock;
    firstUsedBlock=freeBlock;
    return freeBlock;
  }
  TRACE("Create new used block of size %d bytes with this spare space.\n",newBlockSize);   
  TRACE("Free block size is now 0x%x bytes.\n",freeBlock->size);
  usedBlock=(void*)((long)freeBlock+freeBlock->size);
  TRACE("New block address is %p.\n",usedBlock);
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

  TRACE("Allocating 0x%x bytes of memory.\n",requestSize);
  //Best Fit Test wether we have enough memory for request :
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
    getBlock(pb);
    i=split(pb,requestSize);
  } else {
    i=split(bb,requestSize);
  }
  TRACE("Suitable block available at %p.\n",i);
  return i;
}
void * galloc(int requestSize)
{
  TRACE("0x%x bytes requested\n", requestSize);
  TRACE("Header size is 0x%zx.\n",sizeof(int));
  if (!initialized)
  {
    TRACE("First call, initializing, %p\n",sbrk(0));
    firstFreeBlock=getBlock(NULL);
    initialized=1;
  }
  if (requestSize>MMAP_THRESHOLD)
    return NULL;
  return alloc(requestSize);
}
//Test function :
int main(int argc, char * argv[])
{
  int * a = galloc(sizeof(int));
  TRACE("Got pointer to %p.\n",a);
  *a = 4;
  return 0;
}

