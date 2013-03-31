/*
 * tlklookup: lookup a strref in a .tlk file V1 based on 
 * the Infinity Engine File Format Hacking Project
 *
 * Reads a strref from stdin and looks up the associated value
 * in a tlk file given by argv[0]
 *
 * usage:
 * tlklookup file
 *
 * DISCLAIMER: Use it as you like.  Author assumes no responsibility.
 *
 * Jade Tucker <jadeatucker@gmail.com> 03/13
 */

#include <stdio.h>
#include <stdlib.h>

#define LEN_HEADER 18

#pragma pack( push, 1 )
struct tlk {
  char signature[4];
  char version[4];
  char unknown[2];
  int numentries;
  int offset;
};

struct entry {
  char unknown[2];
  char name[8];
  unsigned int vol;
  unsigned int pitch;
  unsigned int offset;
  unsigned int length;
};
#pragma pack(pop)

void tlklookup( const char *filename )
{
  struct tlk tlkobj;
  struct entry ent;
  FILE *fIn;
  unsigned int strref;
  char *string;

  /* open file handle */
  fIn = fopen( filename, "rb" );
  if( !fIn ) return;

  /* read tlk header */
  if( fread( &tlkobj, sizeof( struct tlk ), 1, fIn ) != 1 ) return;

  /* find entry data indexed by strref */
  if( fread( &strref, 4, 1, stdin ) != 1) return;
  fseek( fIn, LEN_HEADER + (strref * sizeof(struct entry)), SEEK_SET );
  fread( &ent, sizeof( struct entry ), 1, fIn );

  /* read string data */
  string = malloc( ent.length );
  fseek( fIn, tlkobj.offset + ent.offset, SEEK_SET );
  fread( string, ent.length, 1, fIn );

  printf( "%s\n", string );

  fclose( fIn );
}

int main( int c, char **v )
{
  if(c > 1)
    tlklookup( v[1] );
  return 0;
}
