#include "backend/transformations.h"

#include <math.h>
#include <stdint.h>

#include "backend/object_t.h"

static double get_absolute_max_among_coordinates(const object_t *object) {
  double max_value = fabs(object->x_max);

  double possible_absolute_max_values[5] = {
      object->y_max, object->z_max, object->x_min, object->y_min, object->z_min,
  };

  for (int i = 0; i != 5; ++i) {
    double possible_absolute_max = fabs(possible_absolute_max_values[i]);
    if (possible_absolute_max > max_value) max_value = possible_absolute_max;
  }

  return max_value;
}

void normalize_vertices(double max_scale, object_t *object) {
  double max_absolute_coordinate = get_absolute_max_among_coordinates(object);

  double scale = max_absolute_coordinate / fabs(max_scale);

  for (uint32_t i = 0; i != object->vertices_amount; ++i) {
    object->vertices[i].x /= scale;
    object->vertices[i].y /= scale;
    object->vertices[i].z /= scale;
  }

  object->x_min /= scale;
  object->y_min /= scale;
  object->z_min /= scale;
  object->x_max /= scale;
  object->y_max /= scale;
  object->z_max /= scale;
}
