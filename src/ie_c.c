/*
 * ie_c.c
 *
 * Simple Library of functions and datastructures used by the Bioware
 * Infinity Engine.
 *
 * DISCLAIMER: Use it as you like.  Author assumes no responsibility.
 *
 * Jade Tucker <jadeatucker@gmail.com> 03/13
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#include "ie_c.h"

void *keread(FILE *file, size_t size, size_t offset)
{
  assert(file != NULL);

  int tmp;
  void *result;

  /* find offset */
  tmp = ftell(file);
  fseek(file, offset, SEEK_SET);

  /* allocate space */
  result = malloc(size);
  assert(result != NULL);

  /* read data */
  if(fread(result, size, 1, file) != 1)
   free(result);

  /* reset file pointer pos */
  fseek(file, tmp, SEEK_SET);

  return result;
}

struct Key *kread(FILE *file)
{
  assert(file != NULL);
  struct Key *key = malloc(sizeof(struct Key));

  /* read header */
  if(fread(key, IE_KEY_H_LEN, 1, file) != 1) 
    return 0;

  /* load BIF entries */
  if(key->countbif > 0)
    key->bifents = keread(file, key->countbif * IE_BIFE_LEN, key->offsetbif);

  /* load BIF str data */
  if(key->offsetbif > 0 && key->offsetres > 0) {
    size_t strdoffset = KSTRDOFF(key);
    size_t size = key->offsetres - strdoffset;
    key->strdata = keread(file, size, strdoffset);
  }

  /* Load Resource entries */
  if(key->countres > 0) {
    key->resents = keread(file, key->countres * IE_RESE_LEN, key->offsetres);

  }

  return key;
}  

void kdestroy(struct Key *key)
{
  if(key != NULL) {
    if(key->bifents != NULL)
      free(key->bifents);
    if(key->resents != NULL)
      free(key->resents);
    free(key);
  }
}

void klist(struct Key *key)
{
  assert(key != NULL);
  int i = key->countres;
  char *resname;
  for(; i > 0; i--) {
    struct KResEnt kre;
    kre = key->resents[i];
    resname = strdup(kre.resname);
    resname[7] = '\0';
    printf("%s\n", resname);
  } 
}

char *kbiffstr(struct Key *key, struct KBifEnt kbe)
{
  assert(key != NULL);
  if(kbe.lenfname > 0) {
    size_t size = kbe.lenfname;
    size_t stroffset = kbe.offsetfname - KSTRDOFF(key);
    char *str = malloc(size);
    assert(str != NULL);
    memcpy(str, &key->strdata[stroffset], size);
    return str;
  }
  else {
    return NULL;
  }
}
