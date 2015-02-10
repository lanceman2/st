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


// using x[0] and x[1] to get x_dot[0]
// where dt = 1
static inline void _deriv_forward_2(StReal_t *x, StReal_t *x_dot)
{
  *x_dot = x[1] - x[0];
}

// using x[0], x[1], and x[2] to get x_dot[0]
// where dt = 1
static inline void _deriv_forward_3(StReal_t *x, StReal_t *x_dot)
{
  *x_dot = 6*x[0] - 8*x[1] + 2*x[2];
}







// example: dims = { 2, 3, -1 };
void stSequence_appendDeriv(struct StSequence *s, int *dims)
{
  ASSERT(s);
  ASSERT(s->x);
  ASSERT(s->x[0]);
  ASSERT(s->dof > 0);
  ASSERT(s->len);
  ASSERT(dims && dims[0]>=0);
  ASSERT(s->label && s->label[0]);

}

