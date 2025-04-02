#include "string_array.h"
#include <stdlib.h>

void string_array_free(string_array *array) {
  for (long i = 0; i < array->size; i++) {
    free(array->array[i]);
  }
  free(array->array);
  free(array);
}
