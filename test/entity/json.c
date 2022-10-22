#include <jansson.h>

int main() {
    json_error_t *err = NULL;
    char *dumped = NULL;
    json_t *j = json_loads("{\"x\": 42, \"y\": 213}", 0, err);

    dumped = json_dumps(j, 0);
    printf("%s\n", dumped);

    if (json_object_set(j, "x", json_integer(555)) < 0) {
        fprintf(stderr, "json set is failed\n");
    }

    dumped = json_dumps(j, 0);
    printf("%s\n", dumped);

    free(j);
    free(dumped);
}