#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>
#include "st.h"
#include "debug.h"


/* See file ../doc/differentiation.html for details:
 *
 * "Notes on Numerical Differentiation" for calculations of the constants
 * in the expressions below in the generated file deriv_coef.h
 *
 * We used axiom (open-axiom) to calculate them.
 *
 */
// This code assumes that the sample rate is fixed at 1 so h = 1.


#include "deriv_coef.h"


static inline void
_deriv(StReal_t *out, const StReal_t *in,
    const StReal_t *coef, int inc, int sign, int points)
{
  *out = 0;
  int i;
  for(i=0; i<points; ++i)
  {
    *out += sign * in[i] * coef[inc * i];
//SPEW("%g = %d*(in[%d]=%g)*(%g)\n", sign * in[i] * coef[inc * i],
  //  inc, inc * i, in[i], coef[inc * i]);
  }

//SPEW("val      =%g\n\n", *out);
}

void stDeriv(const StReal_t *in, StReal_t *out, size_t len,
    int deriv_num/* 1 for 1st derivative*/, int poly_order, int points)
{
  ASSERT(in);
  ASSERT(out);
  ASSERT(in != out);
  ASSERT(poly_order >= 1);
  ST_VASSERT(points > poly_order,
      "You cannot fit a polynomial of order x^%d with "
      "just %d points.\n", poly_order, points);
  ST_VASSERT(len >= points,
      "You cannot differentiate just %zu data points, "
      "at least %d points are needed.\n", len, points);

  const StReal_t *coef = 0;
  coef = _get_coef(deriv_num, poly_order, points);

  ST_VASSERT(coef,
    "case with deriv_num=%d, poly_order=%d, points=%d not written",
    deriv_num, poly_order, points);

  int i,n, sign = 1;

  n = (points-1)/2;
  for(i=0;i<n;++i)
  {
    _deriv(out++, in, coef, 1, sign, points);
    coef += points;
  }
  n = len - points/2;
  for(;i<n;++i)
    _deriv(out++, in++, coef, 1, sign, points);
  n = len;
  if(points & 01)
    // odd number of points
    --coef;
  else
    // even number of points
    coef += points - 1;

  if(deriv_num & 01)
    // odd
    sign = -1;

  --in;
  for(;i<n;++i)
  {
    // We opted to use the matrix X symmetry and not have the whole matrix
    // in the code.  So we go back to the upper rows in the matrix and
    // change +/- factor and direction of access along the row.
    _deriv(out++, in, coef, -1, sign, points);
    coef -= points;
  }
}
