#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include "../libst/st.h"

#define LEN 107
#define SUB_LEN (LEN/2)
#define OMEGA (2*M_PI/(LEN-1))
#define FILENAME "/tmp/binaryFileTest_XXX_TODELETE_23rf3fef3r"

int main(void)
{
  {
    unsigned int seed;
    struct timeval tv;
    ST_ASSERT(gettimeofday(&tv, 0) == 0);
    seed = tv.tv_sec + tv.tv_usec;
    srand(seed);
  }

  struct StSequence *s;
  s = stSequence_create(LEN, 2, "X sin(X)");

  StReal_t *x, *y;
  x = stSequence_x(s)[0];
  y = stSequence_x(s)[1];

  size_t i;
  for(i=0; i<LEN; ++i)
  {
    x[i] = i;
    y[i] = sinf(i*OMEGA + 0.1*rand()/RAND_MAX);
  }

  struct StSequence *c;
  c = stSequence_createCopy(s);

  stSequence_writeBinFile(s, FILENAME);

  struct StSequence *b, *d;
  b = stSequence_createBinFile(FILENAME);
  
  // Clobber file making a different file
  const int dofs[] = { 0, 1, -1};
  stSequence_writeBinFileFull(b, FILENAME, dofs, 3, SUB_LEN - 1);
  d = stSequence_createBinFile(FILENAME);
  ST_ASSERT(stSequence_getLen(d) == SUB_LEN-3);

  // remove the file
  unlink(FILENAME);

  StReal_t **x_b, **x_c, **x_d;
  x_b = stSequence_x(b);
  x_c = stSequence_x(c);
  x_d = stSequence_x(d);

  ST_ASSERT(stSequence_getDof(s) == stSequence_getDof(b));
  ST_ASSERT(stSequence_getDof(s) == stSequence_getDof(c));
  ST_ASSERT(stSequence_getDof(s) == stSequence_getDof(d));

  for(i=0; i<LEN; ++i)
  {
    ST_ASSERT(x[i] == x_b[0][i]);
    ST_ASSERT(x[i] == x_c[0][i]);
    ST_ASSERT(y[i] == x_b[1][i]);
    ST_ASSERT(y[i] == x_c[1][i]);
    if(i < SUB_LEN && i >= 3)
    {
      ST_ASSERT(x[i] == x_d[0][i-3]);
      ST_ASSERT(y[i] == x_d[1][i-3]);
    }
   }

  ST_ASSERT(strcmp(stSequence_getLabel(s, 0), stSequence_getLabel(b, 0)) == 0);
  ST_ASSERT(strcmp(stSequence_getLabel(s, 0), stSequence_getLabel(c, 0)) == 0);
  ST_ASSERT(strcmp(stSequence_getLabel(s, 0), stSequence_getLabel(d, 0)) == 0);
  ST_ASSERT(strcmp(stSequence_getLabel(s, 1), stSequence_getLabel(b, 1)) == 0);
  ST_ASSERT(strcmp(stSequence_getLabel(s, 1), stSequence_getLabel(c, 1)) == 0);
  ST_ASSERT(strcmp(stSequence_getLabel(s, 1), stSequence_getLabel(d, 1)) == 0);

  stSequence_destroy(d);

  d = stSequence_createCopyFull(b, dofs, 3, SUB_LEN - 1);
  ST_ASSERT(stSequence_getLen(d) == SUB_LEN-3);
  x_d = stSequence_x(d);
  for(i=3; i<SUB_LEN; ++i)
  {
    ST_ASSERT(x[i] == x_d[0][i-3]);
    ST_ASSERT(y[i] == x_d[1][i-3]);
  }
  ST_ASSERT(strcmp(stSequence_getLabel(s, 0), stSequence_getLabel(d, 0)) == 0);
  ST_ASSERT(strcmp(stSequence_getLabel(s, 1), stSequence_getLabel(d, 1)) == 0);

  stSequence_print(c, stdout);

  stSequence_destroy(s);
  stSequence_destroy(c);
  stSequence_destroy(b);
  stSequence_destroy(d);

  fprintf(stderr, "Success\n");
  return 0;
}
