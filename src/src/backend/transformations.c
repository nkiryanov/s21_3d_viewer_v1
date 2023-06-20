#include "backend/transformations.h"

#include <math.h>
#include <stdint.h>

#include "backend/object_t.h"

static double convert_degrees_to_radians(double degree) {
  return degree * M_PI / 180;
}

static void mul_object_on_matrix(vector_t matrix[3], object_t *object) {
  for (uint32_t i = 0; i != object->vertices_amount; ++i) {
    vector_t *vertex = &(object->vertices[i]);
    vector_t rotated_vertex = {.x = 0.0, .y = 0.0, .z = 0.0};

    rotated_vertex.x = matrix[0].x * vertex->x + matrix[0].y * vertex->y +
                       matrix[0].z * vertex->z;
    rotated_vertex.y = matrix[1].x * vertex->x + matrix[1].y * vertex->y +
                       matrix[1].z * vertex->z;
    rotated_vertex.z = matrix[2].x * vertex->x + matrix[2].y * vertex->y +
                       matrix[2].z * vertex->z;

    *vertex = rotated_vertex;
  }
}

static void get_object_midpoint_vector(const object_t *object,
                                       vector_t *midpoint_vector) {
  vector_t min_vector = {.x = INFINITY, .y = INFINITY, .z = INFINITY};
  vector_t max_vector = {.x = -INFINITY, .y = -INFINITY, .z = -INFINITY};

  for (uint32_t i = 0; i != object->vertices_amount; ++i) {
    vector_t vertex = object->vertices[i];
    if (vertex.x < min_vector.x) min_vector.x = vertex.x;
    if (vertex.x > max_vector.x) max_vector.x = vertex.x;
    if (vertex.y < min_vector.y) min_vector.y = vertex.y;
    if (vertex.y > max_vector.y) max_vector.y = vertex.y;
    if (vertex.z < min_vector.z) min_vector.z = vertex.z;
    if (vertex.z > max_vector.z) max_vector.z = vertex.z;
  }

  midpoint_vector->x = min_vector.x + (max_vector.x - min_vector.x) / 2;
  midpoint_vector->y = min_vector.y + (max_vector.y - min_vector.y) / 2;
  midpoint_vector->z = min_vector.z + (max_vector.z - min_vector.z) / 2;
}

static double get_absolute_max_among_object(const object_t *object) {
  double absolute_max = 0.0;

  for (uint32_t i = 0; i != object->vertices_amount; ++i) {
    vector_t vertex = object->vertices[i];
    if (fabs(vertex.x) > absolute_max) absolute_max = fabs(vertex.x);
    if (fabs(vertex.y) > absolute_max) absolute_max = fabs(vertex.y);
    if (fabs(vertex.z) > absolute_max) absolute_max = fabs(vertex.z);
  }

  return absolute_max;
}

void object_move_x_axis(double x, object_t *object) {
  for (uint32_t i = 0; i != object->vertices_amount; ++i) {
    object->vertices[i].x += x;
  }
}

void object_move_y_axis(double y, object_t *object) {
  for (uint32_t i = 0; i != object->vertices_amount; ++i) {
    object->vertices[i].y += y;
  }
}

void object_move_z_axis(double z, object_t *object) {
  for (uint32_t i = 0; i != object->vertices_amount; ++i) {
    object->vertices[i].z += z;
  }
}

void object_rotate_x_axis(double degree, object_t *object) {
  double radians = convert_degrees_to_radians(degree);

  vector_t x_rotation_matrix[3] = {
      {.x = 1, .y = 0.000000000, .z = 0.00000000000},
      {.x = 0, .y = cos(radians), .z = -sin(radians)},
      {.x = 0, .y = sin(radians), .z = cos(radians)},
  };

  mul_object_on_matrix(x_rotation_matrix, object);
}

void object_rotate_y_axis(double degree, object_t *object) {
  double radians = convert_degrees_to_radians(degree);

  vector_t y_rotation_matrix[3] = {
      {.x = cos(radians), .y = 0.00000000, .z = sin(radians)},
      {.x = 0.00000000000, .y = 1.0000000, .z = 0.000000000},
      {.x = -sin(radians), .y = 0.0000000, .z = cos(radians)},
  };

  mul_object_on_matrix(y_rotation_matrix, object);
}

void object_rotate_z_axis(double degree, object_t *object) {
  double radians = convert_degrees_to_radians(degree);

  vector_t z_rotation_matrix[3] = {
      {.x = cos(radians), .y = -sin(radians), .z = 0.000000000},
      {.x = sin(radians), .y = cos(radians), .z = 0.0000000000},
      {.x = 0.0000000000, .y = 0.0000000000, .z = 1.0000000000},
  };

  mul_object_on_matrix(z_rotation_matrix, object);
}

void object_scale(double relative_scale, object_t *object) {
  // Edge case. Do not scale object by 0 and any close enough values
  if (fabs(relative_scale) < 10e-7) return;

  vector_t scale_matrix[3] = {
      {.x = relative_scale, .y = 0.000000000000, .z = 0.000000000000},
      {.x = 0.000000000000, .y = relative_scale, .z = 0.000000000000},
      {.x = 0.000000000000, .y = 0.000000000000, .z = relative_scale},
  };

  mul_object_on_matrix(scale_matrix, object);
}

void object_normalize(double max_scale, object_t *object) {
  vector_t midpoint_vector;

  get_object_midpoint_vector(object, &midpoint_vector);

  // Normalize object alignment. Move to the center of the axises.
  object_move_x_axis(-midpoint_vector.x, object);
  object_move_y_axis(-midpoint_vector.y, object);
  object_move_z_axis(-midpoint_vector.z, object);

  // Normalize scaling. All coordinates should not exceed a `max_scale` value.
  double scale = fabs(max_scale) / get_absolute_max_among_object(object);
  object_scale(scale, object);
}
