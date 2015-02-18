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


int stSequence_getDof(struct StSequence *v)
{
  ASSERT(v);
  ASSERT(v->x);
  ASSERT(v->x[0]);
  ASSERT(v->dof > 0);
  ASSERT(v->len);
  return v->dof;
}


size_t stSequence_getLen(struct StSequence *v)
{
  ASSERT(v);
  ASSERT(v->x);
  ASSERT(v->x[0]);
  ASSERT(v->dof > 0);
  ASSERT(v->len);
  return v->len;
}


static inline
int getValueFromStr(char **str, StReal_t *val)
{
  char *s, *ptr = 0;
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
  ASSERT(v->dof > 0);
  ASSERT(v->len);
  return v->x;
}

const char *stSequence_getLabel(const struct StSequence *s, int dof)
{
  ASSERT(s);
  ASSERT(s->x);
  ASSERT(s->x[0]);
  ASSERT(s->dof > 0);
  ASSERT(s->len);
  ST_ASSERT(dof < s->dof);
  ASSERT(s->label[dof]);

  return s->label[dof];
}

void stSequence_setLabel(struct StSequence *s, int dof,
    const char *fmt, ...)
{
  ASSERT(s);
  ASSERT(s->x);
  ASSERT(s->x[0]);
  ASSERT(s->dof > 0);
  ASSERT(s->len);
  ST_ASSERT(dof < s->dof);
  ASSERT(s->label[dof]);

  free(s->label[dof]);

  const size_t LL = 64;
  char str[LL]; // We limit the label length
  va_list ap;
  va_start(ap, fmt);
  ST_ASSERT(vsnprintf(str, LL, fmt, ap) >= 0);
  va_end(ap);
  s->label[dof] = strdup(str);
}

void _stSequence_appendDof(struct StSequence *s, const char *labelFmt, ...)
{
  ASSERT(s);
  ASSERT(s->x);
  ASSERT(s->x[0]);
  ASSERT(s->dof > 0);
  ASSERT(s->len);
  s->x = realloc(s->x, sizeof(StReal_t *)*(s->dof+1));
  ST_ASSERT(s->x);
  s->x[s->dof] = malloc(sizeof(StReal_t)*s->len);
  ST_ASSERT(s->x[s->dof]);
  
  s->label = realloc(s->label, sizeof(char **)*(s->dof+2));
  s->label[s->dof+1] = 0;

  if(!labelFmt || !labelFmt[0])
  {
    char str[16];
    ST_ASSERT(snprintf(str, 16, "x%d", s->dof) > 0);
    s->label[s->dof] = strdup(str);
    ST_ASSERT(s->label[s->dof]);
  }
  else
  {
    char str[64];
    va_list ap;
    va_start(ap, labelFmt);
    ST_ASSERT(vsnprintf(str, 64, labelFmt, ap) >= 0);
    va_end(ap);
    s->label[s->dof] = strdup(str);
    ST_ASSERT(s->label[s->dof]);
  }

  ++s->dof;
}

struct StSequence *stSequence_create(size_t len, int dof, const char *fmt, ...)
{
  ASSERT(dof > 0);
  ASSERT(len);
  StReal_t **x;
  char **label;
  x = malloc(sizeof(*x)*dof);
  ST_ASSERT(x);
  label = malloc(sizeof(char*)*dof+1);
  ST_ASSERT(label);
  label[dof] = 0; // terminated
  int n_labels = 0; // so far

  if(fmt && fmt[0])
  {
    size_t ll = 128;
    char *l;
    l = malloc(sizeof(char)*ll);
    ST_ASSERT(l);
    va_list ap;
    va_start(ap, fmt);
    size_t n;
    n = vsnprintf(l, ll, fmt, ap);
    ST_ASSERT(n >= 0);
    va_end(ap);

    while(n == ll)
    {
      l = realloc(l, sizeof(char)*(ll += 64));
      ST_ASSERT(l);
      va_start(ap, fmt);
      n = vsnprintf(l, ll, fmt, ap);
      ST_ASSERT(n >= 0);
      va_end(ap);
    } 

    char *s;
    s = l;
    while(*s == ' ')
      ++s;

    while(*s)
    {
      char *end;
      end = s;
      while(*end && *end != ' ')
        ++end;
      if(*end)
      {
        *end = '\0';
        ++end;
        while(*end == ' ')
          ++end;
      }

      label[n_labels++] = strdup(s);
      if(n_labels == dof)
        break;
      s = end;
    }

    free(l);
  }

  for(;n_labels<dof;++n_labels)
  {
    // default labels
    char s[16];
    snprintf(s,16,"x%d", n_labels);
    label[n_labels] = strdup(s);
    ST_ASSERT(label[n_labels]);
  }

  int i;
  for(i=0;i<dof;++i)
  {
    x[i] = malloc(sizeof(StReal_t)*len);
    ST_ASSERT(x[i]);
  }

  struct StSequence *v;
  v = malloc(sizeof(*v));
  ST_ASSERT(x);
  v->x = x;
  v->len = len;
  v->dof = dof;
  v->label = label;
  return v;
}

struct StSequence *stSequence_createFile(const char *path, int dof)
{
  FILE *file;
  if(!path || !path[0] || (path[0] == '-' && path[1] == '\0'))
    ST_ASSERT(file = stdin);
  else
    ST_ASSERT(file = fopen(path, "r"));

  if(dof < 0)
    dof = 0;

  int d = 0;
  char *line = 0;
  StReal_t **x = 0;
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
  if(dof)
    ST_VASSERT(d == dof,
          "file: %s bad data at line number %zu has %d values not %d\n",
          path, lineNum, d, dof);
  else
    dof = d;

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
        for(i=0;i<dof;++i)
        {
          x[i] = realloc(x[i], sizeof(StReal_t)*bufLen);
          ST_ASSERT(x[i]);
        }
      }
      ST_VASSERT(d < dof,
          "file: %s bad data at line number %zu has %d (or more) values not %d\n",
          path, lineNum, d+1, dof);

      x[d++][len] = val;

    }
    if(d)
    {
      ++len;
      ST_VASSERT(d == dof,
          "file: %s bad data at line number %zu has %d values not %d\n",
          path, lineNum, d, dof);
    }
  }

  if(bufLen != len)
  {
    // remove the extra allocation in the arrays
    int i;
    for(i=0;i<dof;++i)
    {
      x[i] = realloc(x[i], sizeof(StReal_t)*len);
      ST_ASSERT(x[i]);
    }
  }


  if(path && path[0] && (path[0] != '-' || path[1] != '\0'))
    fclose(file);

  if(line)
    free(line);

  char **label;
  label = malloc(sizeof(*label)*(dof+1));
  ST_ASSERT(label);
  label[dof] = 0;

  for(d=0;d<dof;++d)
  {
    char s[8];
    snprintf(s,8,"x%d", d);
    label[d] = strdup(s);
    ST_ASSERT(label[d]);
  }

  struct StSequence *v;

  ST_ASSERT(v = malloc(sizeof(*v)));
  v->x = x;
  v->label = label;
  v->dof = dof; // dof = 1, 2, 3 as in 1D, 2D ...
  v->len = len; // number of dofD points
  return v;
}

void stSequence_print(struct StSequence *v, FILE *file)
{
  ASSERT(v);
  ASSERT(v->x);
  ASSERT(v->dof > 0);
  ASSERT(v->len);
  ASSERT(v->label);
  ASSERT(file);

  size_t i, len;
  int dof, j;
  StReal_t **x;
  len = v->len;
  dof = v->dof;
  x = v->x;

  fprintf(file, "%s", v->label[0]);
  for(j=1; j<dof; ++j)
    fprintf(file, " %s", v->label[j]);
  fprintf(file, "\n");

  for(i=0; i<len; ++i)
  {
    for(j=0; j<dof; ++j)
      fprintf(file, FMT " ", x[j][i]);
    fprintf(file, "\n");
  }
}

void stSequence_destroy(struct StSequence *v)
{
  ASSERT(v);
  ASSERT(v->x);
  ASSERT(v->dof);
  ASSERT(v->len);
  ASSERT(v->label);
  int i;
#ifdef DEBUG
  for(i=0;i<v->dof;++i)
  {
    memset(v->x[i], 0, v->len*sizeof(StReal_t));
    ASSERT(v->label[i]);
  }
#endif
  for(i=0;i<v->dof;++i)
  {
    free(v->x[i]);
    free(v->label[i]);
  }
#ifdef DEBUG
  memset(v->x, 0, sizeof(StReal_t*)*v->dof);
  memset(v->label, 0, sizeof(char*)*(v->dof+1));
#endif
  free(v->x);
  free(v->label);
#ifdef DEBUG
  memset(v, 0, sizeof(*v));
#endif
  free(v);
}
