/*
 * unbifc: unpack a compressed Bif file BIFC V1.0
 *
 * DISCLAIMER: Use it as you like.  Author assumes no responsibility.
 *
 * Jade Tucker <jadeatucker@gmail.com> 03/13
 */

#include "ie_c_bif.h"
#include <stdio.h>

int main(int c, char **v)
{
  int i;

  if(c < 2)
    printf( "usage: unbifc file ...\n");
  else
    for(i = 1; i <= c; i++)
      fclose(unbifc(v[i]));

  return 0;
}
