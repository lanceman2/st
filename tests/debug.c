#include <stdbool.h>
#include "../libst/st_debug.h"
#include "../libstg/stg_debug.h"

int main(void)
{
  ST_DEBUG_INIT();

  ST_VASSERT(0, "testing ST_ASSERT(0)\n");

  return 0;
}
