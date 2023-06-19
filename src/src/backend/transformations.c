#include "backend/transformations.h"

#include <math.h>
#include <stdint.h>

#include "backend/object_t.h"

static double get_coordinates_midpoint(const double *first,
                                       const double *second) {
  double min_value;
  double max_value;

  if (*first < *second) {
    min_value = *first;
    max_value = *second;
  } else {
    min_value = *second;
    max_value = *first;
  }

  return min_value + (max_value - min_value) / 2;
}

static double get_absolute_max_among_coordinates(const object_t *object) {
  double max_value = fabs(*object->x_max);

  double *possible_absolute_max_pointers[5] = {
      object->y_max, object->z_max, object->x_min, object->y_min, object->z_min,
  };

  for (int i = 0; i != 5; ++i) {
    double possible_absolute_max = fabs(*possible_absolute_max_pointers[i]);
    if (possible_absolute_max > max_value) max_value = possible_absolute_max;
  }

  return max_value;
}

static void normalize_vertices_alignment(object_t *object) {
  double x_midpoint = get_coordinates_midpoint(object->x_min, object->x_max);
  double y_midpoint = get_coordinates_midpoint(object->y_min, object->y_max);
  double z_midpoint = get_coordinates_midpoint(object->z_min, object->z_max);

  for (uint32_t i = 0; i != object->vertices_amount; ++i) {
    object->vertices[i].x -= x_midpoint;
    object->vertices[i].y -= y_midpoint;
    object->vertices[i].z -= z_midpoint;
  }
}

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

static void normalize_vertices_coordinates(double max_scale, object_t *object) {
  double max_absolute_coordinate = get_absolute_max_among_coordinates(object);

  double scale = max_absolute_coordinate / fabs(max_scale);

  for (uint32_t i = 0; i != object->vertices_amount; ++i) {
    object->vertices[i].x /= scale;
    object->vertices[i].y /= scale;
    object->vertices[i].z /= scale;
  }
}

void object_normalize(double max_scale, object_t *object) {
  normalize_vertices_alignment(object);
  normalize_vertices_coordinates(max_scale, object);
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
      {.x = 1, .y = 0, .z = 0},
      {.x = 0, .y = cos(radians), .z = -sin(radians)},
      {.x = 0, .y = sin(radians), .z = cos(radians)},
  };

  mul_object_on_matrix(x_rotation_matrix, object);
}

void object_rotate_y_axis(double degree, object_t *object) {
  double radians = convert_degrees_to_radians(degree);

  vector_t y_rotation_matrix[3] = {
      {.x = cos(radians), .y = 0, .z = sin(radians)},
      {.x = 0, .y = 1, .z = 0},
      {.x = -sin(radians), .y = 0, .z = cos(radians)},
  };

  mul_object_on_matrix(y_rotation_matrix, object);
}

void object_rotate_z_axis(double degree, object_t *object) {
  double radians = convert_degrees_to_radians(degree);

  vector_t z_rotation_matrix[3] = {
      {.x = cos(radians), .y = -sin(radians), .z = 0},
      {.x = sin(radians), .y = cos(radians), .z = 0},
      {.x = 0, .y = 0, .z = 1},
  };

  mul_object_on_matrix(z_rotation_matrix, object);
}
