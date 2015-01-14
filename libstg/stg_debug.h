#define SEEN_STG_DEBUG

#ifndef SEEN_ST_DEBUG
#  error "You must include st_debug.h before this file"
#endif


#ifdef ST_DEBUG

#  define STG_DEBUG_INIT()     /* empty macro */

#else

extern
void stgDebug_init(void);

#  define STG_DEBUG_INIT()       stgDebug_init()

#endif
