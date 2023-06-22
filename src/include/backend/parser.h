#ifndef BACKEND_PARSER_H_
#define BACKEND_PARSER_H_

#include <stdint.h>

#include "object_t.h"

int parser(char *file_model, object_t *object_data);
void clean_object_data(object_t *object_data);
void free_vertices_and_polygonc(object_t *object_data);
void printf_object_data(object_t *object_data);

#endif  // BACKEND_PARSER_H_
