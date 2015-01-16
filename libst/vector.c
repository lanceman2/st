#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>
#include "st_debug.h"
#include "type.h"
#include "st_vector.h"
#include "debug.h"


// Example: x[1000*3] is a 3D vector with 1000 points

struct StVector
{
  // channels (dimensions) are interlaced
  StReal_t *x;
  int dim;    // dimension 1, 2, 3, ...
  size_t len; // number of dim-D points like 10,000
#ifdef DEBUG
  size_t bufLen; // bufLen = number of StReal_t allocated
#endif
};


static inline
void checkSize(StReal_t **x, size_t *bufLen, size_t fillLen)
{
  if(*bufLen <= fillLen)
    ST_ASSERT(*x = realloc(
          *x, (*bufLen += 128)*sizeof(StReal_t)));
}


int stVector_getDim(struct StVector *v)
{
  ASSERT(v);
  ASSERT(v->x);
  ASSERT(v->dim);
  ASSERT(v->len);
  return v->dim;
}

static inline
int getValue(char **str, StReal_t *val)
{
  char *s, *ptr = NULL;
  s = *str;
  while(*s)
  {
    *val = STRTOF(s, &ptr);
    if(s != ptr)
    {
      *str = ptr;
      return 1;
    }
    ++s;
  }
  return 0; // got no value this time
}


struct StVector *stVector_create(const char *path, int dim)
{
  FILE *file;
  if(!path || !path[0] || (path[0] == '-' && path[1] == '\0'))
    ST_ASSERT(file = stdin);
  else
    ST_ASSERT(file = fopen(path, "r"));

  size_t fillLen = 0, bufLen = 0, n = 0;
  char *line = NULL;
  StReal_t *x = NULL;
  size_t lineNum = 0, len = 0;

  while(getline(&line, &n, file) != -1)
  {
    ++lineNum;
    if(!line || !line[0] || line[0] == '#')
      continue;

    char *nptr;
    int d = 0;
    nptr = line;
    StReal_t val;
    nptr = line;

    while(getValue(&nptr, &val))
    {
      ++d;
      checkSize(&x, &bufLen, fillLen);
      x[fillLen++] = val;
    }
    if(d)
      ++len;
    if(dim && d)
      ST_VASSERT(d == dim,
          "file: %s bad data at line number %zu has %d points not %d\n",
          path, lineNum, d, dim);
    else if(d)
      dim = d;
  }

  ST_VASSERT(len, "file: %s no vector data found\n", path);

  if(path && path[0] && (path[0] != '-' || path[1] != '\0'))
    fclose(file);

  struct StVector *v;

  ST_ASSERT(v = malloc(sizeof(*v)));
  v->x = x;
  v->dim = dim; // dim = 1, 2, 3 as in 1D, 2D ...
  v->len = len; // number of dimD points
#ifdef DEBUG
  v->bufLen = bufLen;
#endif
  return v;
}

void stVector_print(struct StVector *v, FILE *file)
{
  ASSERT(v);
  ASSERT(v->x);
  ASSERT(v->dim);
  ASSERT(v->len);
  ASSERT(file);

  size_t i, len;
  int dim, j;
  StReal_t *x;
  len = v->len;
  dim = v->dim;
  x = v->x;
  for(i=0; i<len; ++i)
  {
    for(j=0; j<dim; ++j)
      fprintf(file, FMT " ", *x++);
    fprintf(file, "\n");
  }
}

void stVector_destroy(struct StVector *v)
{
  ASSERT(v);
  ASSERT(v->x);
  ASSERT(v->dim);
  ASSERT(v->len);

#ifdef DEBUG
  memset(v->x, 0, v->bufLen*sizeof(StReal_t));
#endif
  free(v->x);
#ifdef DEBUG
  memset(v, 0, sizeof(*v));
#endif

  free(v);
}
