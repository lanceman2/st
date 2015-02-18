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

// This code assumes that the sample rate is fixed at 1


/* See file ../doc/differentiation.html
 * "Notes on Numerical Differentiation" for calculations of the
 * constants in the expressions below.
 *
 * TODO: Find the best way to compile constant rational numbers.
 * example:
 *     -689/1386  is  -689/1386.0
 *     because -689/1386 compiles to integer 0
 *     the 1386.0 in the denominator make it a float.
 *
 *     writing it as -0.497113997113997114 sucks too much
 *     we let the compiler do some work.
 */

#include "sequence_deriv_coef.h"

#if 0

static const
StReal_t coef1_p5_n2[] = 
{
  /////////////////////////////////////////////////////////
  //                   x^2  fit
  //                   5 points
  //  not necessarily a perfect fit
  /* \chi = [0,4] */
  -27/35.0, 13/70.0, 4/7.0, 27/70.0, -13/35.0,
  /* \chi = [-1,3] */
  -17/35.0, 3/70.0, 2/7.0, 17/70.0, -3/35.0,
  /* \chi = [-2,2] */
  -0.2, -0.1, 0, 0.1, 0.2
};

static const
StReal_t coef1_p5_n3[] = 
{
  /////////////////////////////////////////////////////////
  //                   x^3  fit
  //                   5 points
  //  not necessarily a perfect fit
  /* \chi = [0,4] */
  -125/84.0, 34/21.0, 4/7.0, -22/21.0, 29/84.0,
  /* \chi = [-1,3] */
  -19/42.0, -1/42.0, 2/7.0, 13/42.0, -5/42.0,
  /* \chi = [-2,2] */
  1/12.0, -2/3.0, 0, 2/3.0, -1/12.0
};

static const
StReal_t coef1_p5_n4[] = 
{
  /////////////////////////////////////////////////////////
  //                   x^4  fit
  //                   5 points
  //        so this is a perfect fit
  /* \chi = [0,4] */
  -25/12.0, 4, -3, 4/3.0, -0.25,
  /* \chi = [-1,3] */
  -0.25, -5/6.0, 1.5, -0.5, 1/12.0,
  /* \chi = [-2,2] */
  1/12.0, -2/3.0, 0, 2/3.0, -1/12.0
};

static const
StReal_t coef1_p7_n2[] = 
{
  /////////////////////////////////////////////////////////
  //                   x^2  fit
  //                   7 points
  //  not necessarily a perfect fit
  /* \chi = [0,6] */
  -13/28.0, -1/14.0, 5/28.0, 2/7.0, 0.25, 1/14.0, -0.25,
  /* \chi = [-1,5] */
  -29/84.0, -1/14.0, 3/28.0, 4/21.0, 5/28.0, 1/14.0, -11/84.0,
  /* \chi = [-2,4] */
  -19/84.0, -1/14.0, 1/28.0, 2/21.0, 3/28.0, 1/14.0, -1/84.0,
  /* \chi = [-3,3] */
  -3/28.0, -1/14.0, -1/28.0, 0, 1/28.0, 1/14.0, 3/28.0
};

static const
StReal_t coef1_p7_n3[] = 
{
  /////////////////////////////////////////////////////////
  //                   x^3  fit
  //                   7 points
  //  not necessarily a perfect fit
  /* \chi = [0,6] */
  -257/252.0, 61/126.0, 185/252.0, 2/7.0, -11/36.0, -61/126.0, 11/36.0,
  /* \chi = [-1,5] */
  -61/126.0, 17/252.0, 31/126.0, 4/21.0, 5/126.0, -17/252.0, 1/126.0,
  /* \chi = [-2,4] */
  -29/252.0, -23/126.0, -19/252.0, 2/21.0, 55/252.0, 23/126.0, -31/252.0,
  /* \chi = [-3,3] */
  11/126.0, -67/252.0, -29/126.0, 0, 29/126.0, 67/252.0, -11/126.0
};

static const
StReal_t coef1_p7_n4[] = 
{
  /////////////////////////////////////////////////////////
  //                   x^4  fit
  //                   7 points
  //  not necessarily a perfect fit
  /* \chi = [0,6] */
  -1105/693.0, 5059/2772.0, 376/693.0, -19/22.0, -689/1386.0, 2375/2772.0, -373/1386.0,
  /* \chi = [-1,5] */
  -26/63.0, -25/252.0, 17/63.0, 1/3.0, 4/63.0, -59/252.0, 5/63.0,
  /* \chi = [-2,4] */
  79/1386.0, -1619/2772.0, -25/1386.0, 29/66.0, 191/693.0, -607/2772.0, 34/693.0,
  /* \chi = [-3,3] */
  11/126.0, -67/252.0, -29/126.0, 0, 29/126.0, 67/252.0, -11/126.0
};


#endif

static inline void
_deriv(StReal_t *out, const StReal_t *in,
    const StReal_t *coef, int inc, int points)
{
  *out = 0;
  int i;
  for(i=0; i<points; ++i)
  {
    *out += inc * in[i] * coef[inc * i];
//SPEW("%g = %d*(in[%d]=%g)*(%g)\n", inc * in[i] * coef[inc * i],
//    inc, inc * i, in[i], coef[inc * i]);
  }

//SPEW("val      =%g\n\n", *out);
}

#if 0
static inline
const StReal_t *_get_coef(int deriv_num, int poly_order, int points)
{
  if(deriv_num == 1 && points == 5 && poly_order == 2)
    return coef1_p5_n2;
  if(deriv_num == 1 && points == 5 && poly_order == 3)
    return coef1_p5_n3;
  if(deriv_num == 1 && points == 5 && poly_order == 4)
    return coef1_p5_n4;

  if(deriv_num == 1 && points == 7 && poly_order == 2)
    return coef1_p7_n2;
  if(deriv_num == 1 && points == 7 && poly_order == 3)
    return coef1_p7_n3;
  if(deriv_num == 1 && points == 7 && poly_order == 4)
    return coef1_p7_n4;

  return NULL;
}
#endif

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
    "case with poly_order=%d, points=%d not written",
    poly_order, points);

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
 
  int i,n;
  n = (points-1)/2;
  for(i=0;i<n;++i)
  {
    _deriv(out++, in, coef, 1, points);
    coef += points;
  }
  n = s->len - points/2;
  for(;i<n;++i)
    _deriv(out++, in++, coef, 1, points);
  n = s->len;
  if(points & 01)
    // odd number of points
    --coef;
  else
    // even number of points
    coef += points - 1;

  --in;
  for(;i<n;++i)
  {
    _deriv(out++, in, coef, -1, points);
    coef -= points;
  }
}
