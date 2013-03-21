#include "ie_c.h"

int main(int c, char **v) {
  if(c < 2) return 1;

  FILE *fIn;
  fIn = fopen( v[1], "rb");
  if(!fIn) return 1;

  struct Key *key = kread(fIn);
  klist(key);
  kdestroy(key);

  return 0;
}
