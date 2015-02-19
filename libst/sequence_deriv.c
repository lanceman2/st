#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>
#include "st_debug.h"
#include "st_type.h"
#include "type.h"
#include "st_sequence.h"
#include "debug.h"
#include "sequence.h"


/* See file ../doc/differentiation.html for details:
 *
 * "Notes on Numerical Differentiation" for calculations of the constants
 * in the expressions below in the generated file sequence_deriv_coef.h
 *
 * We used axiom (open-axiom) to calculate them.
 *
 */
// This code assumes that the sample rate is fixed at 1 so h = 1.


#include "sequence_deriv_coef.h"


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


void stSequence_deriv(struct StSequence *s, int from, int to,
    int deriv_num/* 1 for 1st derivative*/, int poly_order, int points)
{
  ASSERT(s);
  ASSERT(s->x);
  ASSERT(s->x[0]);
  ASSERT(s->dof > 0);
  ASSERT(s->len);
  ASSERT(from != to);
  ST_ASSERT(from < s->dof);
  ST_ASSERT(s->len >= points);
  ST_ASSERT(points > poly_order);

  const StReal_t *coef = 0;
  coef = _get_coef(deriv_num, poly_order, points);


  ST_VASSERT(coef,
    "case with deriv_num=%d, poly_order=%d, points=%d not written",
    deriv_num, poly_order, points);

  if(to >= s->dof)
  {
    ST_ASSERT(to == s->dof);
    _stSequence_appendDof(s, "%s^d%d(%dpt,x^%d)",
        s->label[from], deriv_num, points, poly_order);
    ASSERT(s->dof > to);
  }
  else
    // we must be clobbering an array
    stSequence_setLabel(s, to, "%s^d%d(%dpt,x^%d)",
        s->label[from], deriv_num, points, poly_order);

  StReal_t *in, *out;
  in = s->x[from];
  out = s->x[to];
 
  int i,n, sign = 1;

  n = (points-1)/2;
  for(i=0;i<n;++i)
  {
    _deriv(out++, in, coef, 1, sign, points);
    coef += points;
  }
  n = s->len - points/2;
  for(;i<n;++i)
    _deriv(out++, in++, coef, 1, sign, points);
  n = s->len;
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
