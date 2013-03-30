/*
 * unbifc: unpack a compressed Bif file BIFC V1.0
 *
 * DISCLAIMER: Use it as you like.  Author assumes no responsibility.
 *
 * Jade Tucker <jadeatucker@gmail.com> 03/13
 */

#include <stdio.h>

#include "ie_c_bif.h"
#include "dbg.h"

int main(int c, char **v)
{
  FILE *fIn = NULL;
  FILE *fOut = NULL;
  int status = 1;

  if(c < 3) {
    printf( "usage: unbifc compfile uncompfile ...\n");
  }
  else {
    fIn = fopen(v[1], "rb");
    check(fIn, "Could not open file for reading: %s", v[1]);

    fOut = fopen(v[2], "wb");
    check(fIn, "Could not open file for writing: %s", v[2]);

    check(unbifc(fIn, fOut) == 0, "Problem unpacking BIF file %s", v[1]);
    status = 0;
  }

error:
  fclose(fIn);
  fclose(fOut);
  return status;
}
