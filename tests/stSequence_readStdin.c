#include <string.h>
#include "../libst/st.h"

int main(int argc, char **argv)
{
  if(argc < 2 || argc > 2 ||
    (argc == 2 && (
         strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0)
    )
  )
  {
    printf("Usage: %s [FILE] [-h|--help]\n"
        "\n"
        "  Reads FILE and prints to stdout.\n",
        argv[0]);
    return 1;
  }

  struct StSequence *s;

  s = stSequence_createFile(argv[1], 0);

  if(stSequence_getDof(s) == 2)
  {
    stSequence_setLabel(s, 0, "X");
    stSequence_setLabel(s, 1, "Y");
  }

  stSequence_deriv(s, 0, 1, 1, 4/*poly_order*/, 5/*points*/);
  stSequence_deriv(s, 0, 2, 1, 3/*poly_order*/, 5/*points*/);
  stSequence_deriv(s, 0, 3, 1, 2/*poly_order*/, 5/*points*/);
  stSequence_deriv(s, 0, 4, 1, 4/*poly_order*/, 7/*points*/);
  stSequence_deriv(s, 0, 5, 1, 3/*poly_order*/, 7/*points*/);
  stSequence_deriv(s, 0, 6, 1, 2/*poly_order*/, 7/*points*/);
  stSequence_deriv(s, 0, 4, 1, 5/*poly_order*/, 9/*points*/);
  stSequence_deriv(s, 0, 5, 1, 4/*poly_order*/, 9/*points*/);
  stSequence_deriv(s, 0, 6, 1, 3/*poly_order*/, 9/*points*/);
  stSequence_deriv(s, 0, 7, 1, 2/*poly_order*/, 9/*points*/);




  stSequence_print(s, stdout);

  stSequence_destroy(s);
  return 0;
}
