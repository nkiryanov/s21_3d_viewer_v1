#ifndef BACKEND_OBJECT_T_H_
#define BACKEND_OBJECT_T_H_

#include <stdint.h>

#include "polygon_t.h"
#include "vector_t.h"
#include "parser.h"

/**
 * @brief Stores Waveforms .obj file data
 *
 */
typedef struct object_t {
  vector_t *vertices;       /**< Pointer to array of vertices */
  int vertices_amount; /**< Amount of vertices in array */

  polygon_t *polygons;      /**< Pointer to array of polygons */
  int polygons_amount; /**< Amount of the polygons in array */
} object_t;

#endif  // BACKEND_OBJECT_T_H_

