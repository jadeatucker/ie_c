#include "ie_c.h"
#include <assert.h>
#include <stdlib.h>

int main(int c, char **v) {
  if(c < 2) return 1;

  FILE *fIn;
  fIn = fopen( v[1], "rb");
  if(!fIn) return 1;

  struct Key *key = kread(fIn);
  assert(key != NULL);
  fclose(fIn);

  /*klist(key);*/
  int i;
  char *str;
  for(i = 0; i < key->countbif; i++) {
    struct KBifEnt kbe = key->bifents[i];
    str = kbiffstr(key, kbe);
    if(str != NULL) {
      printf("%s\n", str);
      free(str);
    }
  }
  kdestroy(key);

  return 0;
}
