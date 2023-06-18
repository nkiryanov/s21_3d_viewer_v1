#ifndef BACKEND_POLYGON_T_H_
#define BACKEND_POLYGON_T_H_

#include "object_t.h"

/**
 * @brief Stores Wavefront .obj polygonal face elements
 *
 * It's limited to store vertex indexes only cause we don't need textures and
 * normals according to the task.
 */

typedef struct polygon_t {
    // f 0 1 3 2
    int *V_in_P; // значения после f
    int amount_column_P; // их кол-во
} polygon_t;

#endif  // BACKEND_POLYGON_T_H_
