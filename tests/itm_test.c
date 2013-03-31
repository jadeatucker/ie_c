#include <stdlib.h>

#include "minunit.h"
#include "dbg.h"
#include "ie_c_itm.h"

#define ITMFILE "data/items/item100.itm"
#define ITMNAME 13089

struct Itm *itm;

char *test_itmread()
{
  FILE *fIn = NULL;
  fIn = fopen(ITMFILE, "rb");
  check(fIn, "problem opening file");

  itm = itmread(fIn);
  mu_assert(itm != NULL, "failed to read itm data");
  mu_assert(itm->name == ITMNAME, "item name did not match");

  fclose(fIn);
  return NULL;
error:
  return 0;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_itmread);

  return NULL;
}

RUN_TESTS(all_tests);
