/*
 * un_bif: unpack a .bif file (BG2 style)
 *
 * This is a sample program from the Infinity Engine File Format Hacking
 * Project.  Use it as you like.  Author assumes no responsibility, yada yada
 * yada.
 *
 * Jade Tucker <jadeatucker@gmail.com> 03/13
 */

#include <stdio.h>
#include <stdlib.h>

void extractFile( FILE *fIn, FILE *fOut, unsigned long offset, unsigned long size )
{
  void *buffer;
  unsigned long temp;

  /* allocate buffer */
  buffer = malloc( size );

  /* find the offset and read data */
  temp = ftell( fIn );
  fseek( fIn, offset, SEEK_SET );

  if( fread( buffer, 1, size, fIn ) != size ) return;
  
  /* write the data to output file */
  fwrite( buffer, size, 1, fOut );

  /* reset file pointer and clean up */
  fseek( fIn, temp, SEEK_SET );

  free( buffer );
}

int extractFileEntries( FILE *fIn, int filenum )
{
  unsigned char resLoc[4], unknown[2], filename[1024];
  unsigned long offset = 0;
  unsigned long size = 0;
  unsigned long type = 0;
  FILE *fOut;

  if( fread( &resLoc, 4, 1, fIn ) != 1 ) return 0;
  if( fread( &offset, 4, 1, fIn ) != 1 ) return 0;
  if( fread( &size, 4, 1, fIn ) != 1 ) return 0;
  if( fread( &type, 2, 1, fIn ) != 1 ) return 0;
  if( fread( &unknown, 2, 1, fIn ) != 1 ) return 0;

  /* case BMP */
  switch( type )
  {
    case 1: /* .bmp */
      sprintf( filename, "image%d.bmp", (unsigned char) resLoc[0] );
      break;
    case 0x03ed:
      sprintf( filename, "item%d.itm", (unsigned char) resLoc[0] );
      break;
    default:
      sprintf( filename, "unknown%d.bad", (unsigned char) resLoc[0] );
      break;
  }
  fOut = fopen( filename, "wb");
  if( !fOut ) return;

  extractFile( fIn, fOut, offset, size);
  fclose( fOut );

  return 0;
}

void unBif( const char *filename )
{
  char signature[4], version[4];
  unsigned long numfiles = 0;
  unsigned long numtilesets = 0;
  unsigned long offset = 0;

  FILE *fIn;
  
  /* open input file */
  fIn = fopen( filename, "rb" );
  if( !fIn ) return;

  /* read header */
  fread( &signature, 4, 1, fIn );
  fread( &version, 4, 1, fIn );
  fread( &numfiles, 4, 1, fIn );
  fread( &numtilesets, 4, 1, fIn );
  fread( &offset, 4, 1, fIn );

  int i;
  for( i = 1; i <= numfiles; i++ )
    extractFileEntries( fIn, i );

  fclose( fIn );
}

int main( int c, char **v )
{
  int i;

  if( c < 2 )
    printf( "usage: un_bif file ...\n" );
  else
    for( i = 1; i <= c; i++ )
      unBif( v[i] );

  return 0;
}
