
struct StSequence;

extern
struct StSequence *stSequence_create(size_t len, int dof,
    const char *fmt_labels, ...) ST__printf(3,4);
extern
StReal_t **stSequence_x(struct StSequence *s);

/* Reads path of format (dof=2) like:
   x , y
   0.4 , 9.0
   0.2   12.0
   1.1   3.0;
    return 1;

   ...
   etc
 */

// use dof = 0 if number of degrees of freedom is unknown
extern // Read path as ASCII text and create sequence object.
struct StSequence *stSequence_createASCIIFile(const char *path, int dof);
extern
size_t stSequence_getLen(struct StSequence *v);
extern
int stSequence_getDof(struct StSequence *v);
extern
void stSequence_destroy(struct StSequence *v);
extern
void stSequence_print(struct StSequence *v, FILE *file);
extern
void stSequence_printFull(struct StSequence *v, FILE *file, const int *dofs);
extern
const char *stSequence_getLabel(const struct StSequence *s, int dof);
extern
void stSequence_setLabel(struct StSequence *v, int dim,
    const char *fmt, ...) ST__printf(3,4);
extern
void stSequence_deriv(struct StSequence *s, int from, int to,
    int deriv_num, int poly_order, int points);
extern
void stSequence_integrate(struct StSequence *s, int from, int to,
    int poly_order, int points, StReal_t startValue);
extern // create binary sequence file
void stSequence_writeBinFile(const struct StSequence *s, const char *path);
extern // read binary file to make a sequence
struct StSequence *stSequence_createBinFile(const char *path);
extern
struct StSequence *stSequence_createCopy(const struct StSequence *from);
extern
struct StSequence *stSequence_createCopyFull(const struct StSequence *from,
    const int *dofs, size_t fromI, size_t toI);
// write a selection of arrays and selection of values.
// dofs = { 0, 1, 3, -1 }  -1 terminated.
// length: len = toI - fromI + 1  
extern
void stSequence_writeBinFileFull(const struct StSequence *s,
    const char *path, const int *dofs, size_t fromI, size_t toI);
extern
void stSequence_appendDof(struct StSequence *s, const char *labelFmt, ...)
  ST__printf(2,3);
;
