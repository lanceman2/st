#include <unistd.h>
#include <math.h>
#include "../libst/st.h"

#define LEN 107
#define OMEGA (2*M_PI/(LEN-1))
#define FILENAME "/tmp/binaryFileTest_XXX_TODELETE_23rf3fef3r"

int main(void)
{
  struct StSequence *s;

  s = stSequence_create(LEN, 2, "X sin(X)");

  StReal_t *x, *y;
  x = stSequence_x(s)[0];
  y = stSequence_x(s)[1];

  size_t i;
  for(i=0; i<LEN; ++i)
  {
    x[i] = i;
    y[i] = sinf(i*OMEGA);
  }

  struct StSequence *c;
  c = stSequence_createCopy(s);

  stSequence_writeBinFile(s, FILENAME);

  struct StSequence *b;
  b = stSequence_createBinFile(FILENAME);

  unlink(FILENAME);

  StReal_t **x_b;
  StReal_t **x_c;
  x_b = stSequence_x(b);
  x_c = stSequence_x(b);

  for(i=0; i<LEN; ++i)
  {
    ST_ASSERT(x[i] == x_b[0][i]);
    ST_ASSERT(x[i] == x_c[0][i]);
    ST_ASSERT(y[i] == x_b[1][i]);
    ST_ASSERT(y[i] == x_c[1][i]);
   }

  stSequence_print(c, stdout);

  stSequence_destroy(s);
  stSequence_destroy(c);
  stSequence_destroy(b);

  fprintf(stderr, "Success\n");
  return 0;
}
