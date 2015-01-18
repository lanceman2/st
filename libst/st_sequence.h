
struct StSequence;


extern
struct StSequence *stSequence_create(size_t len, int dim);
extern
StReal_t **stSequence_x(struct StSequence *s);
extern
struct StSequence *stSequence_create(size_t len, int dim);

/* Reads path of format (dim=2) like:
   x , y
   0.4 , 9.0
   0.2   12.0
   1.1   3.0
   ...
   etc
 */
// use dim = 0 if dimension is unknown
extern
struct StSequence *stSequence_createFile(const char *path, int dim);
extern
size_t stSequence_getLen(struct StSequence *v);
extern
int stSequence_getDim(struct StSequence *v);
extern
void stSequence_destroy(struct StSequence *v);
extern
void stSequence_print(struct StSequence *v, FILE *file);

