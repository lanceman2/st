// Example: a 3D sequence with 1000 points
// It's more like a vector function x[], y[], z[]
// A vector sequence.

struct StSequence
{
  // channels are not interlaced
  // x[0], x[1], x[2] are like x[], y[], z[]
  StReal_t **x;
  char **label;
  int dim;    // dimensions 1, 2, 3, ...
  size_t len; // number of dim-D points like 10,000
  size_t bufLen; // bufLen = number of StReal_t allocated
};


extern
void _stSequence_appendDim(struct StSequence *s,
    const char *labelFmt, ...) ST__printf(2,3);
