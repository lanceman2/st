/* private type stuff */

#ifdef ST_FLOAT
#  define STRTOF    strtof
#  define FMT       "%.7g"
#endif
#ifdef ST_DOUBLE
#  define STRTOF    strtod
#  define FMT       "%.16g"
#endif

#if 0
static inline
int _stUtil_isLittleEndian(void)
{
  const int i = 1;
  // there is one in the first byte if is Little Endian
  // there is one in the last int byte if is Big Endian
  return (*(char *) &i) == 1;
}
#endif
