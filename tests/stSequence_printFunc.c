#include "../libst/st.h"

#define NPOINTS 12
static
StReal_t n;

#define POW  (4) // x^POW

#if 1 // 0 for sin and cos

// testing 4 point fit.
static inline
StReal_t derivF(size_t i)
{
  StReal_t d, x;
  d = i/n - 1;
  x = POW/n;
  int j;
  for(j=1;j<POW;++j)
    x *= d;
  return x;
}

static inline
StReal_t func(size_t i)
{
  StReal_t d, x;
  d = i/n - 1;
  x = 1;
  int j;
  for(j=0;j<POW;++j)
    x *= d;
  return x;  
}
#else
#include <math.h>

#define IFREQ 5

static inline
StReal_t phase(size_t i)
{
  return IFREQ*(i/n - 1);
}

static inline
StReal_t derivF(size_t i)
{
  return IFREQ*cosf(phase(i))/n;
}

static inline
StReal_t func(size_t i)
{
  return sinf(phase(i)); 
}
#endif


int main(int argc, char **argv)
{
  n = (NPOINTS-1)/2.0;

  ST_ASSERT(POW >= 1);

  if(argc > 1)
  {
    printf("Usage: %s [-h|--help]\n"
        "\n"
        "  Prints to stdout.\n",
        argv[0]);
    return 1;
  }

  struct StSequence *s;

  s = stSequence_create(NPOINTS, 3, "X F F_deriv");
  StReal_t *x, *y, *deriv;
  x = stSequence_x(s)[0];
  y = stSequence_x(s)[1];
  deriv = stSequence_x(s)[2];
  size_t i;
  for(i=0;i<NPOINTS;++i)
  {
    x[i] = i;
    y[i] = func(i);
    deriv[i] = derivF(i);
  }

  stSequence_deriv(s, 1, 3, 1, 2/*x^n*/, 9/*points*/);
  stSequence_deriv(s, 1, 4, 1, 3/*x^n*/, 9/*points*/);
  stSequence_deriv(s, 1, 5, 1, 4/*x^n*/, 9/*points*/);
  //stSequence_deriv(s, 1, 5, 1, 4/*x^n*/, 4/*points*/);
  //stSequence_deriv(s, 1, 6, 1, 3/*x^n*/, 9/*points*/);




  stSequence_print(s, stdout);

  stSequence_destroy(s);
  return 0;
}
