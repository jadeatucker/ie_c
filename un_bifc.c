/* $Id: un_bifc.c,v 1.1 2000/08/18 23:37:01 jedwin Exp $ */

/*
 * un_bifc: unpack a compressed .bif file (BG2 style)
 *
 * This is a sample program from the Infinity Engine File Format Hacking
 * Project.  Use it as you like.  Author assumes no responsibility, yada yada
 * yada.
 */

#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>

int extractFile( FILE *fIn, FILE *outfile )
{
  unsigned long cmplen = 0;
  unsigned long uncmplen = 0;
  void *destBuf, *srcBuf;
  int status;

  /* read block header */
  if ( fread( &uncmplen, 4, 1, fIn ) != 1 ) return 0;
  if ( fread( &cmplen, 4, 1, fIn ) != 1 ) return 0;

  /* allocate buffers for decompression */
  srcBuf = malloc( cmplen );
  destBuf = malloc( uncmplen );

  /* read block to compress */
  if ( !srcBuf || !destBuf || fread( srcBuf, 1, cmplen, fIn ) != cmplen )
  {
    if ( destBuf ) free( destBuf );
    if ( srcBuf ) free( srcBuf );
    return 0;
  }

  /* try the decompression */
  status = uncompress( destBuf, &uncmplen, srcBuf, cmplen );

  /* if success, write the decompressed data to the outfile */
  if ( status == Z_OK )
    fwrite( destBuf, uncmplen, 1, outfile );
  else
    printf( "Error decompressing data.\n" );

  /* clean up our buffers */
  free( destBuf );
  free( srcBuf );

  /* if decompression was a success, return 1 */
  if ( status == Z_OK )
      return 1;
  else
      return 0;
}

void unBifc( const char *filename )
{
  char buffer[1024];
  char signature[4], version[4];
  unsigned long uncmplen = 0;
  FILE *fOut;
  FILE *fIn;

  /* open input file */
  fIn = fopen( filename, "rb" );
  if ( !fIn ) return;

  /* create output file */
  sprintf( buffer, "%s.uncompressed", filename );
  fOut = fopen( buffer, "wb" );
  if ( !fOut )
  {
    fclose( fIn );
    return;
  }

  /* read header */
  fread( &signature, 4, 1, fIn );
  fread( &version, 4, 1, fIn );
  fread( &uncmplen, 4, 1, fIn );

  /* extract all blocks */
  while( extractFile( fIn, fOut ) );

  fclose( fIn );
  fclose( fOut );
}

int main( int c, char **v )
{
  int i;

  if( c < 2 )
    printf( "usage: un_bifc file ...\n" );
  else
    for( i = 1; i <= c; i++ )
      unBifc( v[i] );

  return 0;
}
