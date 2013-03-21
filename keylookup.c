/*
 * usage:
 * keylookup file
 *
 * DISCLAIMER: Use it as you like.  Author assumes no responsibility.
 *
 * Jade Tucker <jadeatucker@gmail.com> 03/13
 */

#include <stdio.h>
#include <stdlib.h>

#pragma pack( push, 1 )
struct key_header {
  char signature[4];
  char version[4];
  int countbif;
  int countres;
  int offsetbif;
  int offsetres;
};

struct bif_entry {
  int length;
  int offsetfname;
  char lenfname[2];
  char locator[2];
};

struct res_entry {
  char resname[8];
  char restype[2];
  char reslocator[4];
};
#pragma pack( pop )

void *getentries( FILE *file, size_t size, size_t offset )
{
  if( !file ) return;
  int temp = ftell(file);
  int rc;
  void *result;

  /* find entries offset */
  fseek( file, offset, SEEK_SET );

  /* allocate enough space the entries */
  result = malloc( size );
  if( !result ) return;

  /* read the entries */
  if( fread( result, size, 1, file ) != 1) {
   free(result);
  }

  return result;
}

void keylookup( const char *filename )
{
  FILE *fIn;
  struct key_header header;
  struct bif_entry *bifArr;
  struct res_entry *resArr;

  /* open key file */
  fIn = fopen( filename, "rb" );
  if( !fIn ) return;

  /* read header */
  if( fread( &header, sizeof( header ), 1, fIn ) != 1) return;

  /* Load the Bif entries */
  if( header.countbif > 0 ) {
    bifArr = getentries( fIn, header.countbif * sizeof(struct bif_entry), header.offsetbif );
    /*printf( "Bif entries not loaded.  Expected %d entries at offset %d\n", header.countbif, header.offsetbif );*/
  }

  /* Load the Resource entries */
  if( header.countres > 0 ) {
    resArr = getentries( fIn, header.countres * sizeof(struct res_entry), header.offsetres );
    /*printf( "Bif entries not loaded.  Expected %d entries at offset %d\n", header.countbif, header.offsetbif );*/

    int i;
    char *resname;
    for(i = 0; i < header.countres; i++) {
      resname = resArr[i].resname;
      resname[7] = '\0';
      printf("%s\n", resname);
    }
  }

  /* cleanup */
  free( bifArr );
  free( resArr );
  fclose(fIn);
}  


int main(int c, char **v)
{
  keylookup( v[1] );
  return 0;
}
