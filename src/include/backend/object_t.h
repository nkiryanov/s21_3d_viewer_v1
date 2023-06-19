#ifndef BACKEND_OBJECT_T_H_
#define BACKEND_OBJECT_T_H_

#include <stdint.h>

#include "polygon_t.h"
#include "vertex_t.h"

/**
 * @brief Stores Waveforms .obj file data
 *
 */
typedef struct object_t {
  vertex_t *vertices;       /**< Pointer to array of vertices */
  uint32_t vertices_amount; /**< Amount of vertices in array */

  polygon_t *polygons;      /**< Pointer to array of polygons */
  uint32_t polygons_amount; /**< Amount of the polygons in array */

  double x_max; /**< Max value of `x` among the vertices */
  double y_max; /**< Max value of `y` among the vertices */
  double z_max; /**< Max value of `y` among the vertices */

  double x_min; /**< Min value of `x` among the vertices */
  double y_min; /**< Min value of `y` among the vertices */
  double z_min; /**< Min value of `z` among the vertices */
} object_t;

#endif  // BACKEND_OBJECT_T_H_