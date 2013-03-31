#include <stdlib.h>

#include "minunit.h"
#include "dbg.h"
#include "ie_c.h"

#define FILE_NAME "data/CHITIN.key"
#define RESNAME "AMUL05"

struct Key *key = NULL;
struct KResEnt *kre = NULL;

char *test_kread()
{
  FILE *fIn;
  fIn = fopen(FILE_NAME, "rb");
  check(fIn, "Could not open file for reading: %s", FILE_NAME);

  key = kread(fIn);
  mu_assert(key != NULL, "should be able to read a valid key file.");

  if(fIn) fclose(fIn);
  return NULL;
error:
  return 0;
}

char *test_kbifstr()
{
  int i;
  for(i = 0; i < (int) key->countbif; i++) {
    struct KBifEnt kbe = key->bifents[i];
    mu_assert(kbifstr(key, kbe), "failed to lookup string for key bif entry index");
  }

  return NULL;
}

char *test_kfindres()
{
  kre = kfindres(RESNAME, key);
  mu_assert(kre != NULL, "failed to find resource");

  return NULL;
}

char *test_kfindbif()
{
  struct KBifEnt *kbe = NULL;

  kbe = kfindbif(key, kre);
  mu_assert(kbe != NULL, "failed to lookup bif entry frome resource entry");

  return NULL;
}

char *test_kdestroy()
{
  mu_assert(kdestroy(key) == 0, "should destroy the key");

  return NULL;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_kread);
  mu_run_test(test_kbifstr);
  mu_run_test(test_kfindres);
  mu_run_test(test_kfindbif);
  mu_run_test(test_kdestroy);

  return NULL;
}

RUN_TESTS(all_tests);
