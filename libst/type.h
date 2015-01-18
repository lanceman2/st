/* private type stuff */

#ifdef ST_FLOAT
#  define STRTOF    strtof
#  define FMT       "%.7g"
#endif
#ifdef ST_DOUBLE
#  define STRTOF    strtod
#  define FMT       "%.16g"
#endif

