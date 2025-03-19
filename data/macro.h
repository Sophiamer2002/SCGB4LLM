#include <stdio.h>

#define FATAL fprintf(stderr, "Fatal error in %s on line %d\n", __FILE__, __LINE__), exit(1);
#define SUCCESS(i, tot) fprintf(stderr, "Success in test %d/%d\n", i, tot);
#define FAILURE(i, tot) fprintf(stderr, "Failure in test %d/%d\n", i, tot);
