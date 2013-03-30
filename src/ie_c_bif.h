/*
 * ie_c_bif.h
 *
 * Simple Library of functions and datastructures used by the Bioware
 * Infinity Engine.
 *
 * DISCLAIMER: Use it as you like.  Author assumes no responsibility.
 *
 * Jade Tucker <jadeatucker@gmail.com> 03/13
 */

#include <stdint.h>
#include <stdio.h>

#ifndef _IE_C_BIF_H
#define _IE_C_BIF_H

#define BIF_H_LEN 20

#pragma pack(push, 1)
struct BFileEnt {
  unsigned char resLoc[4], uk[2];
  uint32_t offset;
  uint32_t size;
  uint32_t type;

  void *data;
};

struct BTileEnt {
  unsigned char resLoc[4], uk[2];
  uint32_t offset;
  uint32_t numtiles;
  uint32_t tilesize;
  uint16_t type;

  void *data;
};

struct Bif {
  char signature[4], version[4];
  uint32_t numfiles;
  uint32_t numtilesets;
  uint32_t offset;

  struct BFileEnt *bfents;
  struct BTileEnt *btents;
};
#pragma pack(pop)

int unbifc(FILE *in, FILE *out);
struct Bif *unbif(FILE *in);
int bdestroy(struct Bif *bif);
#endif
