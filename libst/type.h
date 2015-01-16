
#define ST_FLOAT
//#define ST_DOUBLE


#ifdef ST_FLOAT
typedef float StReal_t;
#  define STRTOF    strtof
#  define FMT       "%.7g"
#endif


