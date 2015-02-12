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
 */

static inline
void diff_4ptForward3(StReal_t *in, StReal_t *out)
{
  *out =
    - (11.0/6.0)*in[0] + 3*in[1] - 1.5*in[2] + (1.0/3.0)*in[3];
}

static inline
void diff_4ptForward2(StReal_t *in, StReal_t *out)
{
  *out =
    - (1.0/3.0)*in[-1] - 0.5*in[0] + in[1] - (1.0/6.0)*in[2];
}

// This is a 5 point fit too, given that in[0] coefficient is zero.
static inline
void diff_4ptCenter(StReal_t *in, StReal_t *out)
{
  *out =
    (1.0/12.0)*in[-2] - 1.5*in[-1] + 1.5*in[1] - (1.0/12.0)*in[2];
}

static inline
void diff_4ptBackward2(StReal_t *in, StReal_t *out)
{
  *out =
    (1.0/6.0)*in[-2] - in[-1] + 0.5*in[0] + (1.0/3.0)*in[1];
}

static inline
void diff_4ptBackward3(StReal_t *in, StReal_t *out)
{
  *out =
    - (1.0/3.0)*in[-3] + 1.5*in[-2] - 3*in[-1] + (11.0/6.0)*in[0];
}



void stSequence_deriv(struct StSequence *s, int from, int to)
{
  ASSERT(s);
  ASSERT(s->x);
  ASSERT(s->x[0]);
  ASSERT(s->dof > 0);
  ASSERT(s->len);
  ASSERT(from != to);
  ST_ASSERT(from < s->dof);

  if(to >= s->dof)
  {
    ST_ASSERT(to == s->dof);
    _stSequence_appendDof(s, "%s_dot", s->label[from]);
    ASSERT(s->dof > to);
  }
  // else we must be clobbering an array

  // There must be at least 4 points
  ST_ASSERT(s->len >= 4);

  StReal_t *in, *out;
  in = s->x[from];
  out = s->x[to];
  diff_4ptForward3(in, out);
  diff_4ptForward2(&in[1], &out[1]);
  int i,n;
  n = s->len-3;
  for(i=2;i<n;++i)
    diff_4ptCenter(&in[i], &out[i]);
  diff_4ptBackward2(&in[i], &out[i]);
  ++i;
  diff_4ptBackward3(&in[i], &out[i]);
}
