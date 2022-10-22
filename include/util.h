#define DEBUG 1
#define VERBOSE 0

#define VECTOR_MAX_SIZE_DEFAULT 16
#define VECTOR_EXTEND_RATE 2

#define SQL_SCRIPT_MAX_SIZE 255

#define refsizeof(__TYPE__) (sizeof(__TYPE__ *))
#define INITIALIZE(__TYPE__) ((__TYPE__ *)malloc(refsizeof(__TYPE__)))
#define INITIALIZE_SIZE(__TYPE__, __SIZE__) \
    ((__TYPE__ *)malloc(__SIZE__ * refsizeof(__TYPE__)))
#define INITIALIZE_STRING(__SIZE__) ((char *)malloc(__SIZE__ * sizeof(char)))

#define errordebug(__MSG__) fprintf(stderr, "[ERROR] %s\n", __MSG__)
