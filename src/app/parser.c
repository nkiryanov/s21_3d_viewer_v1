#include "backend/parser.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "backend/object_t.h"

int main() {
  object_t object_data;
  int err = parser("1.obj", &object_data);
  printf("err = %d\n", err);
  if (err == 0) {
    printf_object_data(&object_data);
    free_vertices_and_polygonc(&object_data);
  }
  return 0;
}
