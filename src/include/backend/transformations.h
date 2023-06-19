#ifndef BACKEND_TRANSFORMATIONS_H_
#define BACKEND_TRANSFORMATIONS_H_

#include "object_t.h"

void normalize_vertices(double max_scale, object_t *object);
void move_object_x_axis(double x, object_t *object);
void move_object_y_axis(double y, object_t *object);
void move_object_z_axis(double z, object_t *object);

#endif  // BACKEND_TRANSFORMATIONS_H_
