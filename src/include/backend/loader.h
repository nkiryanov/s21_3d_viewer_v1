#ifndef BACKEND_PARSER_H_
#define BACKEND_PARSER_H_

#include <stdint.h>

#include "object_t.h"

int load_object(char *file_model, object_t *object_data);
void free_object(object_t *object_data);
void printf_object_data(object_t *object_data);

#endif  // BACKEND_PARSER_H_
