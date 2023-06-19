#ifndef BACKEND_OBJECT_T_H_
#define BACKEND_OBJECT_T_H_

#include <stdint.h>

#include "polygon_t.h"
#include "vector_t.h"

/**
 * @brief Stores Waveforms .obj file data
 *
 */
typedef struct object_t {
  vector_t *vertices;       /**< Pointer to array of vertices */
  uint32_t vertices_amount; /**< Amount of vertices in array */

  polygon_t *polygons;      /**< Pointer to array of polygons */
  uint32_t polygons_amount; /**< Amount of the polygons in array */

  double *x_max; /**< Pointer to max value of `x` among the vertices */
  double *y_max; /**< Pointer to max value of `y` among the vertices */
  double *z_max; /**< Pointer to max value of `y` among the vertices */

  double *x_min; /**< Pointer to min value of `x` among the vertices */
  double *y_min; /**< Pointer to min value of `y` among the vertices */
  double *z_min; /**< Pointer to min value of `z` among the vertices */
} object_t;

#endif  // BACKEND_OBJECT_T_H_
