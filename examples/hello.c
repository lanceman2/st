#include <glib.h>
#include <stg.h>


int main(void)
{
  st_init();

  g_print("hello\n");

  ST_VASSERT(0, "testing ST_VASSERT(0)\n");

  return 0;
}
