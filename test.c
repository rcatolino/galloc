#include <stdio.h>
#include <string.h>
#include <gmem.h>
#include "block.h" //for testing purposes only
//#include "gmem.h"

#define alloc malloc
#define free free
struct list{
  long long value;
  struct list * next;
};
//Test function :
int main(int argc, char * argv[])
{

  int * a; 
  struct list * first;
  int * b;
  int * c;
  a = gmalloc(sizeof(int));
  printUsedList();
  printFreeList();
  printf("Got pointer to %p.\n\n",a);
  b = gmalloc(2*sizeof(int));
  printUsedList();
  printFreeList();
  printf("Got pointer to %p.\n\n",b);
  c = gmalloc(sizeof(int));
  printUsedList();
  printFreeList();
  printf("Got pointer to %p.\n\n",c);
  *c = 4;
  gfree(a);
  printUsedList();
  printFreeList();
  printf("\n");
  a = gmalloc(2);
  printUsedList();
  printFreeList();
  printf("Got pointer to %p.\n\n",b);
  gfree(a);
  printUsedList();
  printFreeList();
  printf("\n");
  gfree(c);
  printUsedList();
  printFreeList();
  printf("\n");
  gfree(b);
  printUsedList();
  printFreeList();
  printf("\n");
  a = gmalloc(sizeof(int));
  printUsedList();
  printFreeList();
  printf("Got pointer to %p.\n\n",a);
  printUsedList();
  printFreeList();
  /*
  printf("Got pointer to %p.\n\n",first);
  first->next=NULL;
  first->value=0;
  first = alloc(sizeof(struct list));
  a = alloc(sizeof(int));
  for (*a=1; *a<150000; (*a)++){
    i=alloc(sizeof(struct list));   
    i->value=*a;
    i->next=first;
    first=i;
  }
  printf("\n");
  for (*a=1; *a<150000; (*a)++){
    i=first->next;
    free(first);
    first=i;
  }
  printf("\n");
  */
  gfree(first);
  gfree(a);
  a=gmalloc(MMAP_THRESHOLD+10);
  memset(a,'a',MMAP_THRESHOLD);
  gfree(a);
  return 0;
}
