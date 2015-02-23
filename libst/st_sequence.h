
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
   1.1   3.0
   ...
   etc
 */
// use dof = 0 if number of degrees of freedom is unknown
extern
struct StSequence *stSequence_createFile(const char *path, int dof);
extern
size_t stSequence_getLen(struct StSequence *v);
extern
int stSequence_getDof(struct StSequence *v);
extern
void stSequence_destroy(struct StSequence *v);
extern
void stSequence_print(struct StSequence *v, FILE *file);
extern
const char *stSequence_getLabel(const struct StSequence *s, int dof);
extern
void stSequence_setLabel(struct StSequence *v, int dim,
    const char *fmt, ...) ST__printf(3,4);
extern
void stSequence_deriv(struct StSequence *s, int from, int to,
    int deriv_num, int poly_order, int points);
extern
void stSequence_int(struct StSequence *s, int from, int to,
    int poly_order, int points, StReal_t startValue);
