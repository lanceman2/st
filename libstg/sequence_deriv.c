#include <stdarg.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_deriv.h>

#include "../libst/st.h"
#include "../libst/debug.h"
#include "../libst/type.h"
#include "../libst/sequence.h"


struct DerivParam
{
  double *y; // value array
#ifdef DEBUG
  size_t N;  // limiting index
#endif
};

static
double f(double x, struct DerivParam *dp)
{
  size_t i;
  i = (x + 0.5);
  // Make sure they don't overshoot the array.
  ASSERT(i >= 0 && i < dp->N);
  return dp->y[i];
}

// example: dims = { 2, 3, -1 };
void stSequence_appendDeriv(struct StSequence *s, int *dims)
{
  ASSERT(s);
  ASSERT(s->x);
  ASSERT(s->x[0]);
  ASSERT(s->dim > 0);
  ASSERT(s->len);
  ASSERT(dims && dims[0]>=0);
  gsl_function F;
  F.function = (double (*)(double, void *)) f;

  int d, j=0;
  for(d=dims[j++];d>=0;d=dims[j++])
  {
    ST_ASSERT(d < s->dim);
    struct DerivParam dp;
    F.params = &dp;
    dp.y = s->x[d];
#ifdef DEBUG
    dp.N = s->len;
#endif
    StReal_t *xdot, *err;
    _stSequence_appendDim(s, "%s_dot", s->label[d]);
    xdot = s->x[s->dim-1];
    _stSequence_appendDim(s, "%s_dot_err", s->label[d]);
    err = s->x[s->dim-1];

    size_t i;
    for(i=0;i<2;++i)
      gsl_deriv_forward(&F, i, 4, &xdot[i], &err[i]);
    size_t n;
    n = s->len-2;
    for(;i<n;++i)
      gsl_deriv_central(&F, i, 2, &xdot[i], &err[i]);
    n = s->len;
    for(;i<n;++i)
      gsl_deriv_backward(&F, i, 4, &xdot[i], &err[i]);
  }
}

