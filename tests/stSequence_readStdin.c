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

  struct StSequence *s;

  s = stSequence_createFile("-", 0);

  stSequence_print(s, stdout);

  stSequence_destroy(s);
  return 0;
}
