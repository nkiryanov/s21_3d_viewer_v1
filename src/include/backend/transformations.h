#ifndef BACKEND_TRANSFORMATIONS_H_
#define BACKEND_TRANSFORMATIONS_H_

#include "object_t.h"

void object_normalize(double max_scale, object_t *object);
void object_move_x_axis(double x, object_t *object);
void object_move_y_axis(double y, object_t *object);
void object_move_z_axis(double z, object_t *object);
void object_rotate_x_axis(double degree, object_t *object);
void object_rotate_y_axis(double degree, object_t *object);
void object_rotate_z_axis(double degree, object_t *object);

#endif  // BACKEND_TRANSFORMATIONS_H_
