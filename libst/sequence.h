// Example: a 3D sequence with 1000 points
// It's more like a vector function x[], y[], z[]
// or t[], x[], y[] where t is time.
//
// A vector sequence.


struct StSequence
{
  // channels are not interlaced
  // x[0], x[1], x[2] are like x[], y[], z[]
  StReal_t **x;
  char **label; // NULL terminated like argv
  int dof;    // number of degrees of freedom 1, 2, 3, ...
  size_t len; // number of points like 10,003
};

