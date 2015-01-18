#include <gsl/gsl_math.h>
#include <gsl/gsl_deriv.h>

#include "../libst/st.h"
#include "../libst/debug.h"

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


double f(double x, double *y)
{
  size_t i;
  i = (x + 0.5);
  ASSERT(i>=0 && i<N);

  return y[i];
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

  s = stSequence_create(N, 5);

  StReal_t **xx, *x, *y, *ydot, *yderiv, *yderiverr;
  xx = stSequence_x(s);
  x = xx[0];
  y = xx[1];
  ydot = xx[2];
  yderiv = xx[3];
  yderiverr = xx[4];

  gsl_function F;
  F.function = (double (*)(double, void *)) f;
  F.params = y;

  size_t i;
  for(i=0;i<N;++i)
  {
    x[i] = i;
    y[i] = Y(i);
    ydot[i] = YDot(i);
  }

  for(i=0;i<2;++i)
    gsl_deriv_forward(&F, i, 4, &yderiv[i], &yderiverr[i]);

  for(;i<N-2;++i)
    gsl_deriv_central(&F, i, 2, &yderiv[i], &yderiverr[i]);

  for(;i<N;++i)
    gsl_deriv_backward(&F, i, 4, &yderiv[i], &yderiverr[i]);


  printf("t x xdot xdriv err\n");

  stSequence_print(s, stdout);

  stSequence_destroy(s);
  return 0;
}
