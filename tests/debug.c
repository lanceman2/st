#include <stdbool.h>
#include "../libst/st.h"
#include "../libstg/stg.h"

int main(void)
{
  st_init();

  ST_VASSERT(0, "testing ST_ASSERT(0)\n");

  return 0;
}
