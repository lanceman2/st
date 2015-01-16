
struct StVector;

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
struct StVector *stVector_create(const char *path, int dim);

extern
int stVector_getDim(struct StVector *v);

extern
void stVector_destroy(struct StVector *v);

extern
void stVector_print(struct StVector *v, FILE *file);

