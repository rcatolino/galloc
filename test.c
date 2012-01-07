#include <stdio.h>
#include "block.h" //for testing purposes only
#include "gmem.h"

//Test function :
int main(int argc, char * argv[])
{
  int * a; 
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

  return 0;
}

