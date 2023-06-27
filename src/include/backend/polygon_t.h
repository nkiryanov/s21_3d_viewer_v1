#ifndef BACKEND_POLYGON_T_H_
#define BACKEND_POLYGON_T_H_

#include <stdint.h>

/**
 * @brief Stores Wavefront .obj polygonal face elements
 *
 * It's limited to store vertex indexes only cause we don't need textures and
 * normals according to the task.
 */
typedef struct polygon_t {
  uint32_t *vertex_indices; /**< Pointer to array of vertex indexes */
  uint32_t count_indices;   /**< Amount of the indexes */
} polygon_t;

#endif  // BACKEND_POLYGON_T_H_
