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

static void normalize_vertices_coordinates(double max_scale, object_t *object) {
  double max_absolute_coordinate = get_absolute_max_among_coordinates(object);

  double scale = max_absolute_coordinate / fabs(max_scale);

  for (uint32_t i = 0; i != object->vertices_amount; ++i) {
    object->vertices[i].x /= scale;
    object->vertices[i].y /= scale;
    object->vertices[i].z /= scale;
  }
}

void normalize_vertices(double max_scale, object_t *object) {
  normalize_vertices_alignment(object);
  normalize_vertices_coordinates(max_scale, object);
}

void move_object_x_axis(double x, object_t *object) {
  for (uint32_t i = 0; i != object->vertices_amount; ++i) {
    object->vertices[i].x += x;
  }
}

void move_object_y_axis(double y, object_t *object) {
  for (uint32_t i = 0; i != object->vertices_amount; ++i) {
    object->vertices[i].y += y;
  }
}

void move_object_z_axis(double z, object_t *object) {
  for (uint32_t i = 0; i != object->vertices_amount; ++i) {
    object->vertices[i].z += z;
  }
}
