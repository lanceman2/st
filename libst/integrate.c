#include <alloca.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>
#include "st.h"
#include "debug.h"


/* See file ../doc/integrating.html for details:
 *
 * "Notes on Numerically Integrating" for calculations of the constants
 * in the expressions below in the generated file int_coef.h
 *
 * We used axiom (open-axiom) to calculate them.
 *
 */
// This code assumes that the sample rate is fixed at 1 so h = 1.
// If h is not 1 than do a change of variables, a scaling.


#include "int_matrix.h"

// Multiply a vector by a matrix   theta = matrix * vec
static inline
void _multiply_MxV(StReal_t *theta, const StReal_t *matrix,
    const StReal_t *vec, int columns, int rows)
{
  while(rows--)
  {
    int c = 0;
    *theta = 0;
    while(c<columns)
      *theta += (*matrix++) * vec[c++];
    ++theta;
  }
}

void stIntegrate(const StReal_t *in, StReal_t *out,
    size_t len, int poly_order, int points, StReal_t start)
{
  ASSERT(points >= 0);
  ASSERT(in != out);
  ASSERT(poly_order >= 1);
  ST_VASSERT(points > poly_order,
      "You cannot fit a polynomial of order x^%d with "
      "just %d points.\n", poly_order, points);
  ST_VASSERT(len >= points,
      "You cannot integrate just %zu data points, "
      "at least %d points are needed.\n", len, points);

  const StReal_t *matrix;
  matrix = _get_matrix(poly_order, points);

  ST_VASSERT(matrix,
    "Case with poly_order=%d, points=%d was not configured.\n",
    poly_order, points);

  int rows;
  rows = poly_order + 1; // number rows in all matrices

#if 0
#ifdef DEBUG
  {
    int r,c,m; // row, column, matrix_index
    int rc;
    rc = points * rows; // numColumns x numRows
    fprintf(stderr,
      "-------------------------------------\n"
      "   %d matrices for x^%d  %d points\n"
      "-------------------------------------\n",
      points-1, poly_order, points);
    for(m=0;m<points-1;++m)
    {
      fprintf(stderr, "--------------- %d ---------------\n", m);
      for(r=0;r<=poly_order;++r)
      {
        for(c=0;c<points;++c)
          fprintf(stderr, "%g,", matrix[m*rc + r*points + c]);
        fprintf(stderr, "\n");
      }
      fprintf(stderr, "\n");
    }
  }
#endif
#endif


  // There are 
  // The matrix is size: (points) columns by (poly_order+1) rows.
  // There are (poly_order+1) \theta component values in the \theta
  // vector.

  StReal_t *theta;
  // stack memory.  SIGSEGV signal on error.
  // I always wanted to use this and this is a good
  // use case.
  theta = alloca((poly_order+1)*sizeof(StReal_t));

  // This is pretty well minimized, in the tight loop, C code, and so the
  // array indexing is hell to follow.  If you wish to proof it look at a
  // odd value for points and then a even value for points.  The odd and
  // even cases work a little differently.  One might argue the you
  // should not use this code with points being odd, given the region that
  // we integrate over is not centered in the points that we fix the
  // polynomial to, but we have no pressing reason to require such
  // symmetry, and besides the differentiation is symmetry if the number
  // of points is odd, and symmetry in differentiation may trump symmetry
  // in integration.
  size_t i = 0, iMax;
  iMax = len - 1; // so we only have len-1 values to do.
  int rc, i1, i2;
  i1 = points/2;
  i2 = iMax - (points-1)/2;
  // rc = numColumns x numRows = number of elements in matrix
  rc = points * rows;
  // We don't need a matrix multiply for the first value out,
  *out++ = start; // starting value as given by user

  while(i<iMax)
  {
    // i is the data array index  like F(x/h) = in[i]
    /* get vector theta = Matrix * F(\chi)
     * vector theta being the polynomial coefficients */
    _multiply_MxV(theta, matrix, in, points, rows);

    int j;
    *out = *(out-1); // start at previous value
    for(j=0;j<rows;++j)
      // evaluate integral of polynomial fit
      *out = *out + theta[j]/(j+1);
    ++out; // next

    // get matrix[] indexing for next loop
    // get in[] indexing for next loop

    if(++i >= i1 && i < i2)
      // matrix stays the same
      ++in; // go to next in[]
    else
      // in[] stays the same
      matrix += rc; // go to next matrix
  }
}
