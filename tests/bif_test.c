#include <stdlib.h>

#include "minunit.h"
#include "dbg.h"
#include "ie_c_bif.h"

#define BIFC "data/AREA0300.bif"
#define BIF "data/AREA0300.bif.uncompressed"

char *test_unbifc()
{
  FILE *fIn, *fOut;
  fIn = fopen(BIFC, "rb");
  check(fIn, "Could not open file for reading %s", BIFC);

  fOut = fopen(BIF, "wb");
  check(fOut, "Could not open file for writing %s", BIF);

  mu_assert(unbifc(fIn, fOut), "failed to uncompress bif file");

  return NULL;

error:
  return 0;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_unbifc);

  return NULL;
}

RUN_TESTS(all_tests);
