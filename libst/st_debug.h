
#  define __printf(a,b)  __attribute__((format(printf,a,b)))

extern
void st_spew(const char *file, int line,
    const char *func, const char *format, ...) __printf(4,5);
#  define ST_SPEW(fmt, ...)      stSpew(__FILE__, __LINE__, \
                               __func__, fmt, ##__VA_ARGS__)

extern
void st_assert(const char *file, int line,
    const char *func, bool bool_arg,
    const char *arg, const char *format, ...)
    __printf(6,7);

#  define ST_ASSERT(x)          st_assert(__FILE__, __LINE__,            \
                               __func__, (x), #x, " ")
#  define ST_VASSERT(x, fmt, ...)                                       \
                                st_assert(__FILE__, __LINE__, __func__,  \
                                (x), #x, fmt, ##__VA_ARGS__)

extern
void st_init(void);


