#include "src/encode1d.c"

#include "constants/1dDouble.h"
#include "utils/rand64.h"
#include "zfpEncodeBlockBase.c"

int main()
{
  const struct CMUnitTest tests[] = {
    #include "testcases/block.c"
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
