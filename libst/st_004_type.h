#define ST_FLOAT
//#define ST_DOUBLE


#ifdef ST_FLOAT
   typedef float StReal_t;
#  define stSin(x) sinf((x))
#  define stCos(x) cosf((x))
#  define stSqrt(x) sqrtf((x))
#endif

#ifdef ST_DOUBLE
#  ifdef ST_FLOAT
#    error "ST_FLOAT and ST_DOUBLE are both defined"
#  endif
   typedef double StReal_t;
#  define stSin(x) sin((x))
#  define stCos(x) cos((x))
#  define stSqrt(x) sqrt((x))
#endif
