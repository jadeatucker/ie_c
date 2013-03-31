/*
 * ie_c_itm.c
 *
 * Simple Library of functions and datastructures used by the Bioware
 * Infinity Engine.
 *
 * DISCLAIMER: Use it as you like.  Author assumes no responsibility.
 *
 * Jade Tucker <jadeatucker@gmail.com> 03/13
 */

#include <stdlib.h>

#include "dbg.h"
#include "ie_c_itm.h"

struct Itm *itmread(FILE *in)
{
  struct Itm *itm = NULL;
  check(in != NULL, "file cannot be null");

  itm = malloc(sizeof(struct Itm));
  check_mem(itm);

  check(fread(itm, sizeof(struct Itm), 1, in) == 1, \
      "problem reading itm from file");

  return itm;
error:
  if(itm) free(itm);
  return NULL;
}
