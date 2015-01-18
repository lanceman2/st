#include "../libst/st.h"

static
const size_t N = 1000;

static inline
StReal_t func(size_t i)
{
  return (i - 500.0) * (i - 500.0) * 0.03;  
}


int main(int argc, char **argv)
{
  if(argc > 1)
  {
    printf("Usage: %s [-h|--help]\n"
        "\n"
        "  Prints to stdout.\n",
        argv[0]);
    return 1;
  }

  struct StSequence *s;

  s = stSequence_create(N, 2);
  StReal_t *x, *y;
  x = stSequence_x(s)[0];
  y = stSequence_x(s)[1];
  size_t i;
  for(i=0;i<N;++i)
  {
    x[i] = i;
    y[i] = func(i);
  }

  stSequence_print(s, stdout);

  stSequence_destroy(s);
  return 0;
}
