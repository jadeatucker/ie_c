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

#include "ie_c_bif.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>

int extractFile(FILE *fIn, FILE *outfile)
{
  unsigned long cmplen = 0;
  unsigned long uncmplen = 0;
  void *destBuf, *srcBuf;
  int status;

  /* read block header */
  if (fread(&uncmplen, 4, 1, fIn) != 1) return 0;
  if (fread(&cmplen, 4, 1, fIn) != 1) return 0;

  /* allocate buffers for decompression */
  srcBuf = malloc(cmplen);
  destBuf = malloc(uncmplen);

  /* read block to compress */
  if (!srcBuf || !destBuf || fread(srcBuf, 1, cmplen, fIn) != cmplen)
  {
    if (destBuf) free(destBuf);
    if (srcBuf) free(srcBuf);
    return 0;
  }

  /* try the decompression */
  status = uncompress(destBuf, &uncmplen, srcBuf, cmplen);

  /* if success, write the decompressed data to the outfile */
  if (status == Z_OK)
    fwrite(destBuf, uncmplen, 1, outfile);
  else
    printf("Error decompressing data.\n");

  /* clean up our buffers */
  free(destBuf);
  free(srcBuf);

  /* if decompression was a success, return 1 */
  if (status == Z_OK)
      return 1;
  else
      return 0;
}

FILE *unbifc(const char *filename)
{
  char buffer[1024];
  char signature[4], version[4];
  uint32_t uncmplen = 0;
  FILE *fOut;
  FILE *fIn;

  /* open input file */
  fIn = fopen(filename, "rb");
  if (!fIn) return NULL;

  /* create output file */
  sprintf(buffer, "%s.uncompressed", filename);
  fOut = fopen(buffer, "wb");
  if (!fOut) {
    fclose(fIn);
    return NULL;
  }

  /* read header */
  fread(&signature, 4, 1, fIn);
  fread(&version, 4, 1, fIn);
  fread(&uncmplen, 4, 1, fIn);

  if(strncmp(signature, "BIFC", 4) == 0 && strncmp(version, "V1.0", 4) == 0) {
    while(extractFile(fIn, fOut));
  } else {
    printf("Error not a compressed bif file or unsupported version.\n");
    fclose(fIn);
    return NULL;
  }

  fclose(fIn);
  rewind(fOut);
  return fOut;
}
