/*
 * ie_c_bif.c
 *
 * Simple Library of functions and datastructures used by the Bioware
 * Infinity Engine.
 *
 * DISCLAIMER: Use it as you like.  Author assumes no responsibility.
 *
 * Jade Tucker <jadeatucker@gmail.com> 03/13
 */

#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "dbg.h"
#include "ie_c_bif.h"

int readblock(FILE *fIn, FILE *fOut)
{
  unsigned long cmplen = 0;
  unsigned long uncmplen = 0;
  void *destBuf = NULL, *srcBuf = NULL;
  int status = 0;

  /* read block header */
  check(fread(&uncmplen, 4, 1, fIn) == 1 && \
        fread(&cmplen, 4, 1, fIn) == 1, \
        "problem reading compressed bif block header");

  srcBuf = malloc(cmplen);
  check_mem(srcBuf);
  destBuf = malloc(uncmplen);
  check_mem(destBuf);

  check(fread(srcBuf, cmplen, 1, fIn) == 1, "problem reading compressed bif block");

  check(uncompress(destBuf, &uncmplen, srcBuf, cmplen) == Z_OK,\
        "problem uncompressing compressed bif block");

  check(fwrite(destBuf, uncmplen, 1, fOut) == 1, "problem writing bif block to outfile");
  status = 1;

error:
  if(destBuf) free(destBuf);
  if(srcBuf) free(srcBuf);
  return status;
}

int unbifc(FILE *in, FILE *out)
{
  char signature[4], version[4];
  uint32_t uncmplen = 0;

  /* read header */
  fread(&signature, 4, 1, in);
  fread(&version, 4, 1, in);
  fread(&uncmplen, 4, 1, in);

  check(strncmp(signature, "BIFC", 4) == 0 && strncmp(version, "V1.0", 4) == 0, \
        "Unsupported BIF file version. Signature and Version should be BIFC V1.0"); 

  while(readblock(in, out));

  return 1;
error:
  return 0;
}

// TODO: This is exactly the same as keread,
// need to move to shared file.
void *beread(FILE *file, size_t size, size_t offset)
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

struct BFileEnt *bferead(FILE *fIn, int count, size_t offset)
{
  struct BFileEnt *bfe = NULL;

  check(fIn != NULL, "File cannot be NULL");
  check(count >= 0, "Count cannot be negative");
  check(offset >= 0, "Offset cannot be negative");

  bfe = malloc(count * sizeof(struct BFileEnt));
  check_mem(bfe);

  check(fseek(fIn, offset, SEEK_SET) == 0, \
      "Problem seeking to file entry offset.");

  int i;
  for(i = 0; i < count; i++) {
    struct BFileEnt *b = &bfe[i];
    check(fread(&b->resLoc, 4, 1, fIn) == 1 && \
          fread(&b->offset, 4, 1, fIn) == 1 && \
          fread(&b->size, 4, 1, fIn) == 1 && \
          fread(&b->type, 2, 1, fIn) == 1 && \
          fread(&b->uk, 2, 1, fIn) == 1,\
          "Problem reading file entry");

    b->data = beread(fIn, (size_t) b->size, (size_t) b->offset);
    check(b->data != NULL, "Problem reading file data");
  }

  return bfe;

error:
  if(bfe) free(bfe);
  return NULL;
}

struct Bif *unbif(FILE *in)
{
  struct Bif *bif = NULL;
  bif = malloc(sizeof(struct Bif));
  check_mem(bif);

  check(fread(bif, BIF_H_LEN, 1, in) == 1,\
      "Problem reading bif header");

  if(bif->numfiles > 0)
    bif->bfents = bferead(in, (int) bif->numfiles, (size_t) bif->offset);

  return bif;

error:
  if(bif) free(bif);
  return NULL;
}
