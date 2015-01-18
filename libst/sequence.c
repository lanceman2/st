#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>
#include "st_debug.h"
#include "st_type.h"
#include "type.h"
#include "st_sequence.h"
#include "debug.h"


// Example: a 3D sequence with 1000 points
// It's more like a vector function x[], y[], z[]
// A vector sequence.

struct StSequence
{
  // channels are not interlaced
  // x[0], x[1], x[2] are like x[], y[], z[]
  StReal_t **x;
  int dim;    // dimensions 1, 2, 3, ...
  size_t len; // number of dim-D points like 10,000
#ifdef DEBUG
  size_t bufLen; // bufLen = number of StReal_t allocated
#endif
};


int stSequence_getDim(struct StSequence *v)
{
  ASSERT(v);
  ASSERT(v->x);
  ASSERT(v->x[0]);
  ASSERT(v->dim > 0);
  ASSERT(v->len);
  return v->dim;
}


size_t stSequence_getLen(struct StSequence *v)
{
  ASSERT(v);
  ASSERT(v->x);
  ASSERT(v->x[0]);
  ASSERT(v->dim > 0);
  ASSERT(v->len);
  return v->len;
}


static inline
int getValueFromStr(char **str, StReal_t *val)
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

StReal_t **stSequence_x(struct StSequence *v)
{
  ASSERT(v);
  ASSERT(v->x);
  ASSERT(v->x[0]);
  ASSERT(v->dim > 0);
  ASSERT(v->len);
  return v->x;
}

struct StSequence *stSequence_create(size_t len, int dim)
{
  ASSERT(dim > 0);
  ASSERT(len);
  StReal_t **x;
  x = malloc(sizeof(*x)*dim);
  ST_ASSERT(x);
  int i;
  for(i=0;i<dim;++i)
    x[i] = malloc(sizeof(StReal_t)*len);
  struct StSequence *v;
  v = malloc(sizeof(*v));
  ST_ASSERT(x);
  v->x = x;
  v->len = len;
  v->dim = dim;
  return v;
}

struct StSequence *stSequence_createFile(const char *path, int dim)
{
  FILE *file;
  if(!path || !path[0] || (path[0] == '-' && path[1] == '\0'))
    ST_ASSERT(file = stdin);
  else
    ST_ASSERT(file = fopen(path, "r"));

  if(dim < 0)
    dim = 0;

  int d = 0;
  char *line = NULL;
  StReal_t **x = NULL;
  size_t lineNum = 0, len = 0, n = 0;

  while(getline(&line, &n, file) != -1)
  {
    ++lineNum;
    if(!line || !line[0] || line[0] == '#')
      continue;

    char *nptr;
    StReal_t val;
    nptr = line;

    while(getValueFromStr(&nptr, &val))
    {
      x = realloc(x, sizeof(StReal_t *)*(d+1));
      ST_ASSERT(x);
      x[d] = malloc(sizeof(StReal_t));
      ST_ASSERT(x[d]);
      x[d++][0] = val;
    }
    if(d)
    {
      len = 1;
      break;
    }
  }
  ST_VASSERT(len, "file: %s no sequence data found\n", path);
  if(dim)
    ST_VASSERT(d == dim,
          "file: %s bad data at line number %zu has %d values not %d\n",
          path, lineNum, d, dim);
  else
    dim = d;


  size_t bufLen = 1;


  while(getline(&line, &n, file) != -1)
  {
    ++lineNum;
    if(!line || !line[0] || line[0] == '#')
      continue;

    char *nptr;
    nptr = line;
    StReal_t val;
    nptr = line;
    d = 0;

    while(getValueFromStr(&nptr, &val))
    {
      if(bufLen <= len)
      {
        int i;
        bufLen += 32;
        for(i=0;i<dim;++i)
        {
          x[i] = realloc(x[i], sizeof(StReal_t)*bufLen);
          ST_ASSERT(x[i]);
        }
      }
      ST_VASSERT(d < dim,
          "file: %s bad data at line number %zu has %d (or more) values not %d\n",
          path, lineNum, d+1, dim);

      x[d++][len] = val;

    }
    if(d)
    {
      ++len;
      ST_VASSERT(d == dim,
          "file: %s bad data at line number %zu has %d values not %d\n",
          path, lineNum, d, dim);
    }
  }


  if(path && path[0] && (path[0] != '-' || path[1] != '\0'))
    fclose(file);

  if(line)
    free(line);

  struct StSequence *v;

  ST_ASSERT(v = malloc(sizeof(*v)));
  v->x = x;
  v->dim = dim; // dim = 1, 2, 3 as in 1D, 2D ...
  v->len = len; // number of dimD points
#ifdef DEBUG
  v->bufLen = bufLen;
#endif
  return v;
}

void stSequence_print(struct StSequence *v, FILE *file)
{
  ASSERT(v);
  ASSERT(v->x);
  ASSERT(v->dim > 0);
  ASSERT(v->len);
  ASSERT(file);

  size_t i, len;
  int dim, j;
  StReal_t **x;
  len = v->len;
  dim = v->dim;
  x = v->x;
  for(i=0; i<len; ++i)
  {
    for(j=0; j<dim; ++j)
      fprintf(file, FMT " ", x[j][i]);
    fprintf(file, "\n");
  }
}

void stSequence_destroy(struct StSequence *v)
{
  ASSERT(v);
  ASSERT(v->x);
  ASSERT(v->dim);
  ASSERT(v->len);
  int i;
#ifdef DEBUG
  for(i=0;i<v->dim;++i)
    memset(v->x[i], 0, v->bufLen*sizeof(StReal_t));
#endif
  for(i=0;i<v->dim;++i)
    free(v->x[i]);
#ifdef DEBUG
  memset(v->x, 0, sizeof(StReal_t*)*v->dim);
#endif
  free(v->x);
#ifdef DEBUG
  memset(v, 0, sizeof(*v));
#endif
  free(v);
}
