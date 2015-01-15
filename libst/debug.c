#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <signal.h>
#include "st_debug.h"
#include "debug.h"


void st_spew(const char *file, int line,
    const char *func, const char *format, ...)
{
  fprintf(stderr, "%s:%s():%d: ", file, func, line);
  va_list ap;
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
}

static
void sigHandler(int sig_num)
{
  ST_VASSERT(0, "We caught signal %d", sig_num);
}

void st_init(void)
{
  static __thread bool init = false;
  if(init) return;
  init = true;
  errno = 0;
  ST_ASSERT(signal(SIGSEGV, sigHandler) != SIG_ERR);
  ST_ASSERT(signal(SIGABRT, sigHandler) != SIG_ERR);

  dlerror();
  void (*stg_init)(void) = NULL;
  // See if we are linked with libstg.so
  // If so we call the stg_init().
  stg_init = dlsym(RTLD_DEFAULT, "stg_init");
  char *error;
  error = dlerror();
  if(!error && stg_init)
    // stg_init() is re-entrance safe
    stg_init();
}

void st_assert(const char *file, int line,
    const char *func, bool bool_arg,
    const char *arg, const char *format, ...)
{
  if(!bool_arg)
  {
    va_list ap;

    if(errno)
    {
      char error[64];
      error[63] = '\0';
      strerror_r(errno, error, 64);
      fprintf(stderr, "ST_ASSERT: %s:%d:%s(): errno=%d:%s\n"
        "ASSERTION(%s) FAILED: pid: %d\n",
        file, line, func, errno, error, arg, getpid());
    }
    else
      fprintf(stderr, "ST_ASSERT: %s:%d:%s():\n"
        "ASSERTION(%s) FAILED: pid: %d\n",
        file, line, func, arg, getpid());

    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    fprintf(stderr, "\nsignal to exit, st app sleeping here ...\n"
        "try running:   gdb --pid %d\n", getpid());
    // We could run GDB here but who are we to take control and tell
    // the parent process what to do, we can't be sure what kind of
    // shell it is, if it is one, anyway.  If they have job control they
    // are better off doing it themselves.

    // Stop. Why not stop?  Works nice from bash parent process.
    // kill(getpid(), SIGSTOP);
    // if not stopped or awakened then hang in sleep.
    int i = 1; // unset in debugger to stop sleeping and return.
    while(i)
      sleep(10);
  }
}
