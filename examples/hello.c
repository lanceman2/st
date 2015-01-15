#include <stg.h>

int main(void)
{
  ST_DEBUG_INIT();

  ST_VASSERT(0, "testing ST_VASSERT(0)\n");

  return 0;
}
