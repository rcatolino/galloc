#include <stdio.h>
#include "block.h" //for testing purposes only
#include "gmem.h"

struct list{
  long long value;
  struct list * next;
};
//Test function :
int main(int argc, char * argv[])
{

  int * a; 
  int * b;
  int * c;
  struct list * first;
  struct list * i;
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
  first = galloc(sizeof(struct list));
  printUsedList();
  printFreeList();
  printf("Got pointer to %p.\n\n",first);
  first->next=NULL;
  first->value=0;
  printf("allocating 0x%lx bytes of memory.\n",150*sizeof(struct list));
  for (*a=1; *a<550; (*a)++){
    i=galloc(sizeof(struct list));   
    i->value=*a;
    i->next=first;
    first=i;
  }
  printf("\n");
  printf("allocated %ld bytes of memory.\n",150*sizeof(struct list));
  for (i=first; i!=NULL; i=i->next){
    printf("%lld ",i->value);
  }
  printf("\n");
  printUsedList();
  printFreeList();
  return 0;
}
