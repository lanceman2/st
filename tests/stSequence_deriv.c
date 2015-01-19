#include <math.h>
#include "../libst/st.h"
#include "../libstg/stg.h"

static const size_t N = 1000;
static size_t NM1;

static double T = 30;

static inline
StReal_t Y(size_t i)
{
  return sin(i/T);  
}

static inline
StReal_t YDot(size_t i)
{
  return cos(i/T)/T;  
}

int main(int argc, char **argv)
{
  if(argc > 1)
  {
    printf("Usage: %s [-h|--help]\n"
        "\n"
        "  Prints to stdout.\n",
        argv[0]);
    return 1;
  }
  NM1 = N - 1;

  struct StSequence *s;

  s = stSequence_create(N, 3);
  stSequence_setLabel(s, 0, "t");
  stSequence_setLabel(s, 1, "x");
  stSequence_setLabel(s, 2, "x_deriv");

  StReal_t **xx, *t, *x, *xderiv;
  xx = stSequence_x(s);
  t = xx[0];
  x = xx[1];
  xderiv = xx[2];

  size_t i;
  for(i=0;i<N;++i)
  {
    t[i] = i;
    x[i] = Y(i);
    xderiv[i] = YDot(i);
  }

  int d[] = { 1, -1 };

  stSequence_appendDeriv(s, d);

  stSequence_print(s, stdout);

  stSequence_destroy(s);
  return 0;
}
