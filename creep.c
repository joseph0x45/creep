#include "cJSON.h"
#include "string_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>

#define CREEP_CONFIG_FILE_PATH "creep.config.json"

typedef struct {
  // the command creep runs every time it detects a change
  char *cmd;
  // a list of directories to be ignored by creep
  string_array exclude_dir;
  // a list of file extensions to be watched by creep
  string_array include_ext;
  // a list of directories to be watched by creep
  string_array include_dir;
  // a list of files to be watched by creep
  string_array include_file;
} creep_config_t;

int string_array_parse_from_json(cJSON *json, char *field,
                                 string_array *array) {
  cJSON *json_array = cJSON_GetObjectItem(json, field);
  if (!cJSON_IsArray(json_array)) {
    return -1;
  }
  size_t count = cJSON_GetArraySize(json_array);
  array->array = (char **)malloc(count * sizeof(char *));
  array->size = count;
  for (size_t i = 0; i < count; i++) {
    cJSON *array_item = cJSON_GetArrayItem(json_array, i);
    if (!cJSON_IsString(array_item)) {
      printf("'%s' must contain strings only", field);
      string_array_free(array);
    }
  }
  return 0;
}

void read_creep_config(creep_config_t *config) {
  FILE *f = fopen(CREEP_CONFIG_FILE_PATH, "r");
  if (f == NULL) {
    perror("fopen");
    return;
  }
  fseek(f, 0, SEEK_END);
  long config_size = ftell(f);
  rewind(f);
  char *config_content = malloc(config_size + 1);
  if (config_content == NULL) {
    fclose(f);
    perror("malloc");
    return;
  }
  long read = fread(config_content, 1, config_size, f);
  fclose(f);
  if (read != config_size) {
    perror("fread");
    fclose(f);
    free(config_content);
    return;
  }
  cJSON *json = cJSON_Parse(config_content);
  if (json == NULL) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr == NULL) {
      printf("Error while parsing config: %s\n", error_ptr);
    }
    cJSON_Delete(json);
    free(config_content);
    return;
  }
  cJSON *cmd = cJSON_GetObjectItem(json, "cmd");
  if (cJSON_IsString(cmd) && (cmd->valuestring != NULL)) {
    config->cmd = cmd->valuestring;
  } else {
    printf("Failed to parse config");
    goto end;
  }
end:
  cJSON_Delete(json);
  free(config_content);
  return;
}

int main(int argc, char *argv[]) {
  creep_config_t config;
  read_creep_config(&config);
  return EXIT_SUCCESS;
}
