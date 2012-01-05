#include <stdio.h>
#include "gmem.h"

//Test function :
int main(int argc, char * argv[])
{
  int * a = galloc(sizeof(int));
  printf("Got pointer to %p.\n",a);
  *a = 4;
  return 0;
}

