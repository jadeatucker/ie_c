/*
 * ie_c.h
 *
 * Simple Library of functions and data structures used by the Bioware
 * Infinity Engine.
 *
 * DISCLAIMER: Use it as you like.  Author assumes no responsibility.
 *
 * Jade Tucker <jadeatucker@gmail.com> 03/13
 */

#include <stdint.h>
#include <stdio.h>

#ifndef _IE_C_H
#define _IE_C_H

#define IE_KEY_H_LEN 24
#define IE_BIFE_LEN 12
#define IE_RESE_LEN 14

#define KSTRDOFF(N) (IE_KEY_H_LEN + IE_BIFE_LEN * N->countbif)

#pragma pack(push, 1)
struct KBifEnt {
  uint32_t length;
  uint32_t offsetfname;
  uint16_t lenfname;
  uint16_t locator;
};

struct KResEnt {
  char resname[8];
  uint16_t restype;
  uint32_t reslocator;
};

struct Key {
  char signature[4];
  char version[4];
  uint32_t countbif;
  uint32_t countres;
  uint32_t offsetbif;
  uint32_t offsetres;

  struct KBifEnt *bifents;
  char *strdata;
  struct KResEnt *resents;
};
#pragma pack(pop)

void *keread(FILE *file, size_t size, size_t offset);
void kdestroy(struct Key *key);
void klist(struct Key *key);
char *kbifstr(struct Key *key, struct KBifEnt kbe);
struct KResEnt *kfindres(const char *resname, struct Key *key);
struct Key *kread(FILE *file);
#endif
