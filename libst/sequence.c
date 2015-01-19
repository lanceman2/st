#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>
#include "st_debug.h"
#include "st_type.h"
#include "type.h"
#include "st_sequence.h"
#include "debug.h"
#include "sequence.h"


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

void stSequence_setLabel(struct StSequence *s, int dim,
    const char *fmt, ...)
{
  ASSERT(s);
  ASSERT(s->x);
  ASSERT(s->x[0]);
  ASSERT(s->dim > 0);
  ASSERT(s->len);
  ST_ASSERT(dim < s->dim);
  ASSERT(s->label[dim]);

  free(s->label[dim]);

  char str[64];
  va_list ap;
  va_start(ap, fmt);
  ST_ASSERT(vsnprintf(str, 64, fmt, ap) >= 0);
  va_end(ap);
  s->label[dim] = strdup(str);
}

void _stSequence_appendDim(struct StSequence *s, const char *labelFmt, ...)
{
  ASSERT(s);
  ASSERT(s->x);
  ASSERT(s->x[0]);
  ASSERT(s->dim > 0);
  ASSERT(s->len);
  s->x = realloc(s->x, sizeof(StReal_t *)*(s->dim+1));
  ST_ASSERT(s->x);
  s->x[s->dim] = malloc(sizeof(StReal_t)*s->bufLen);
  ST_ASSERT(s->x[s->dim]);
  
  s->label = realloc(s->label, sizeof(char **)*(s->dim+2));
  s->label[s->dim+1] = NULL;

  if(!labelFmt || !labelFmt[0])
  {
    char str[8];
    ST_ASSERT(snprintf(str, 8, "x%d", s->dim) > 0);
    s->label[s->dim] = strdup(str);
  }
  else
  {
    char str[64];
    va_list ap;
    va_start(ap, labelFmt);
    ST_ASSERT(vsnprintf(str, 64, labelFmt, ap) >= 0);
    va_end(ap);
    s->label[s->dim] = strdup(str);
  }

  ++s->dim;
}

struct StSequence *stSequence_create(size_t len, int dim)
{
  ASSERT(dim > 0);
  ASSERT(len);
  StReal_t **x;
  char **label;
  x = malloc(sizeof(*x)*dim);
  ST_ASSERT(x);
  label = malloc(sizeof(char*)*dim+1);
  ST_ASSERT(label);
  label[dim] = NULL;
  int i;
  for(i=0;i<dim;++i)
  {
    char s[8];
    x[i] = malloc(sizeof(StReal_t)*len);
    ST_ASSERT(x[i]);
    snprintf(s,8,"x%d", i);
    label[i] = strdup(s);
    ST_ASSERT(label[i]);
  }
  struct StSequence *v;
  v = malloc(sizeof(*v));
  ST_ASSERT(x);
  v->x = x;
  v->len = len;
  v->dim = dim;
  v->bufLen = len;
  v->label = label;
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

  char **label;
  label = malloc(sizeof(*label)*(dim+1));
  ST_ASSERT(label);
  for(d=0;d<dim;++d)
  {
    char s[8];
    snprintf(s,8,"x%d", d);
    label[d] = strdup(s);
    ST_ASSERT(label[d]);
  }

  struct StSequence *v;

  ST_ASSERT(v = malloc(sizeof(*v)));
  v->x = x;
  v->dim = dim; // dim = 1, 2, 3 as in 1D, 2D ...
  v->len = len; // number of dimD points
  v->bufLen = bufLen;
  return v;
}

void stSequence_print(struct StSequence *v, FILE *file)
{
  ASSERT(v);
  ASSERT(v->x);
  ASSERT(v->dim > 0);
  ASSERT(v->len);
  ASSERT(v->label);
  ASSERT(file);

  size_t i, len;
  int dim, j;
  StReal_t **x;
  len = v->len;
  dim = v->dim;
  x = v->x;

  fprintf(file, "%s", v->label[0]);
  for(j=1; j<dim; ++j)
    fprintf(file, " %s", v->label[j]);
  fprintf(file, "\n");

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
  ASSERT(v->label);
  int i;
#ifdef DEBUG
  for(i=0;i<v->dim;++i)
  {
    memset(v->x[i], 0, v->bufLen*sizeof(StReal_t));
    ASSERT(v->label[i]);
  }
#endif
  for(i=0;i<v->dim;++i)
  {
    free(v->x[i]);
    free(v->label[i]);
  }
#ifdef DEBUG
  memset(v->x, 0, sizeof(StReal_t*)*v->dim);
  memset(v->label, 0, sizeof(char*)*(v->dim+1));
#endif
  free(v->x);
  free(v->label);
#ifdef DEBUG
  memset(v, 0, sizeof(*v));
#endif
  free(v);
}
