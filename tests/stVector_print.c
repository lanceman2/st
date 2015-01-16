#include "../libst/st.h"

int main(int argc, char **argv)
{
  if(argc > 1)
  {
    printf("Usage: %s [-h|--help]\n"
        "\n"
        "  Reads stdin and prints to stdout.\n",
        argv[0]);
    return 1;
  }

  struct StVector *v;

  v = stVector_create("-", 0);

  stVector_print(v, stdout);

  stVector_destroy(v);
  return 0;
}
