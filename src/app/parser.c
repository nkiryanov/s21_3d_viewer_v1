#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "backend/loader.h"
#include "backend/object_t.h"

void print_non_consistent_polygons(const object_t *object) {
  for (uint32_t i = 0; i != object->count_polygons; ++i) {
    for (uint32_t j = 0; j != object->polygons[i].count_indices; ++j) {
      uint32_t index = object->polygons[i].vertex_indices[j];
      if (index >= object->count_vertices || index < 0) {
        printf("Non consistent vertex found.");
        printf("Polygon num = %d, vertex = %d\n", i, index);
      }
    }
  }
}

int main(int argc, char *argv[]) {
  object_t object_data;
  char filename[1024] = {'\0'};

  if (argc > 1) {
    strcpy(filename, argv[1]);
  } else {
    strcpy(filename, "object.obj");
  }

  int err = load_object(filename, &object_data);
  printf("filename = %s, error state = %d\n", filename, err);
  if (err == 0) {
    printf_object_data(&object_data);
    print_non_consistent_polygons(&object_data);
    free_object(&object_data);
  }
  return 0;
}
