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
