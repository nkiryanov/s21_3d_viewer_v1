#ifndef BACKEND_VECTOR_T_H_
#define BACKEND_VECTOR_T_H_

/**
 * @brief Structure to store 3D vector.
 * 
 * The vector is also capable of storing Wavefront .obj vertex coordinates.
 */
typedef struct vector_t {
  double x;
  double y;
  double z;
} vector_t;

#endif  // BACKEND_VECTOR_T_H_
