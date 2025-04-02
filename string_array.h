#pragma once
typedef struct {
  long size;
  char **array;
} string_array;

void string_array_free(string_array *array);
