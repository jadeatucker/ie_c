/*
 * eitm: edit/export a .itm file V1 based on the Infinity Engine File Format Hacking Project
 * Export a .itm file into a human ledgible text file.
 *
 * usage:
 * eitm file ...
 *
 * DISCLAIMER: Use it as you like.  Author assumes no responsibility.
 *
 * Jade Tucker <jadeatucker@gmail.com> 03/13
 */

#include <stdio.h>
#include <stdlib.h>

struct itm {
  char signature[4];
  char version[4];
  char genericName[4];
  char name[4];
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
  char genDesc[4];
  char desc[4];
  char carryIcon[8];
  char enchantment[4];
  char abilityOffset[4];
  char abilityCount[2];
  char effectsOffset[4];
  char unknown[2];
  char effectsCount[2];
};

void exportitm( const char *filename )
{
  struct itm item;
  FILE *fIn;
  FILE *fOut;
  char outfile[1024];

  fIn = fopen( filename, "rb" );
  if( !fIn ) return;

  fread( &item, sizeof( struct itm ), 1, fIn );
  
  fwrite( item.name, sizeof( item.name ), 1, stdout );

  fclose( fIn );
}

int main( int c, char **v )
{
  int i;

  if( c < 2 )
    printf( "usage: eitm file ...\n" );
  else
    for( i = 1; i <= c; i++ )
      exportitm( v[i] );

  return 0;
}
