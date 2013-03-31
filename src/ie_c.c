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

#include "dbg.h"
#include "ie_c.h"

void *keread(FILE *file, size_t size, size_t offset)
{
  int tmp;
  void *result = NULL;

  check(file, "file cannot be NULL.");
  check(size >= 0, "size cannot be negative");
  check(offset >= 0, "offset cannot be negative");

  /* find offset */
  tmp = ftell(file);
  check(fseek(file, offset, SEEK_SET) == 0, "Unable to set file offset");

  /* allocate space */
  result = malloc(size);
  check_mem(result);

  /* read data */
  check(fread(result, size, 1, file) == 1, "Unable to read from file");

  /* reset file pointer pos */
  check(fseek(file, tmp, SEEK_SET) == 0, "Unable to set file offset");

  return result;

error:
  if(result) free(result);
  return NULL;
}

struct Key *kread(FILE *file)
{
  struct Key *key = NULL;
  check(file, "file cannot be NULL");

  key = malloc(sizeof(struct Key));
  check_mem(key);

  /* read header */
  check(fread(key, IE_KEY_H_LEN, 1, file) == 1, "Problem reading key header");

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
  if(key->countres > 0)
    key->resents = keread(file, key->countres * IE_RESE_LEN, key->offsetres);

  return key;
error:
  if(key) free(key);
  return NULL;
}  

int kdestroy(struct Key *key)
{
  if(key != NULL) {
    if(key->bifents != NULL)
      free(key->bifents);
    if(key->resents != NULL)
      free(key->resents);
    if(key->strdata != NULL)
      free(key->strdata);
    free(key);
  }
  return 0;
}

char *kbifstr(struct Key *key, struct KBifEnt kbe)
{
  check(key, "key cannot be NULL");
  if(kbe.lenfname > 0) {
    size_t size = kbe.lenfname;
    size_t stroffset = kbe.offsetfname - KSTRDOFF(key);
    char *str = malloc(size);
    check_mem(str);
    memcpy(str, &key->strdata[stroffset], size);
    return str;
  }
error:
  return NULL;
}

struct KResEnt *kfindres(const char *resname, struct Key *key)
{
  check(key, "key cannot be NULL");
  int i = key->countres;
  struct KResEnt *kre;

  for(; i > 0; i--) {
    kre = &key->resents[i];

    if(strncmp(resname, kre->resname, 8) == 0)
      return kre;
  }
error:
  return NULL;
}

struct KBifEnt *kfindbif(struct Key *key, struct KResEnt *kre)
{
  unsigned int bifindex = (kre->reslocator & 0xFFF00000) >> 20;
  return &key->bifents[bifindex];
}
