/*
 * ie_c_itm.h
 *
 * Simple Library of functions and datastructures used by the Bioware
 * Infinity Engine.
 *
 * DISCLAIMER: Use it as you like.  Author assumes no responsibility.
 *
 * Jade Tucker <jadeatucker@gmail.com> 03/13
 */

#include <stdint.h>
#include <stdlib.h>

#include "dbg.h"

#pragma pack(push, 1)
struct Itm {
  char signature[4];
  char version[4];
  uint32_t genericName;
  uint32_t name;
  char usedUpItem[8];
  char flags[4];
  char category[2];
  char usability[4];
  char bamri[2];
  char minlvl[2];
  char minstr[2];
  char minstrbonus[2];
  char minint[2];
  char mindex[2];
  char minwis[2];
  char mincon[2];
  char minchr[2];
  char baseVal[4];
  char maxStack[2];
  char icon[8];
  char lore[2];
  char groundIcon[8];
  char weight[4];
  uint32_t genDesc;
  uint32_t desc;
  char carryIcon[8];
  char enchantment[4];
  char abilityOffset[4];
  char abilityCount[2];
  char effectsOffset[4];
  char unknown[2];
  char effectsCount[2];
};
#pragma pack(pop)

struct Itm *itmread(FILE *in);
