#include "../libst/st.h"

static
const size_t N = 100;
static
StReal_t n;

static inline
StReal_t func(size_t i)
{
  StReal_t d;
  d = i/n - 1;
  return d*d*d;  
}


int main(int argc, char **argv)
{
  n = N/2.0;

  if(argc > 1)
  {
    printf("Usage: %s [-h|--help]\n"
        "\n"
        "  Prints to stdout.\n",
        argv[0]);
    return 1;
  }

  struct StSequence *s;

  s = stSequence_create(N, 2, "x y");
  StReal_t *x, *y;
  x = stSequence_x(s)[0];
  y = stSequence_x(s)[1];
  size_t i;
  for(i=0;i<N;++i)
  {
    x[i] = i;
    y[i] = func(i);
  }

  stSequence_print(s, stdout);

  stSequence_destroy(s);
  return 0;
}
