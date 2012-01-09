#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "block.h" //for testing purposes only
#include "gmem.h"

#define alloc galloc
#define free gfree
struct list{
  long long value;
  struct list * next;
};
//Test function :
int main(int argc, char * argv[])
{

  int * a; 
  struct list * first;
  /*
  struct list * i;
  int * b;
  int * c;
  a = galloc(sizeof(int));
  printUsedList();
  printFreeList();
  printf("Got pointer to %p.\n\n",a);
  b = galloc(2*sizeof(int));
  printUsedList();
  printFreeList();
  printf("Got pointer to %p.\n\n",b);
  c = galloc(sizeof(int));
  printUsedList();
  printFreeList();
  printf("Got pointer to %p.\n\n",c);
  *c = 4;
  gfree(a);
  printUsedList();
  printFreeList();
  printf("\n");
  a = galloc(2);
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
  a = galloc(sizeof(int));
  printUsedList();
  printFreeList();
  printf("Got pointer to %p.\n\n",a);
  printUsedList();
  printFreeList();
  printf("Got pointer to %p.\n\n",first);
  first->next=NULL;
  first->value=0;
  */
  first = alloc(sizeof(struct list));
  a = alloc(sizeof(int));
  /*
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
  free(first);
  free(a);
  a=alloc(MMAP_THRESHOLD+10);
  memset(a,'a',MMAP_THRESHOLD);
  free(a);
  return 0;
}
