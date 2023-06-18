#ifndef BACKEND_OBJECT_T_H_
#define BACKEND_OBJECT_T_H_

#include <stdint.h>
#include "polygon_t.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct object_t {
  double **V;      /**< Pointer to array of vertices */
  int vertices_amount; /**< Amount of vertices in array */

  polygon_t *P;      /**< Pointer to array of polygons */
  int polygons_amount; /**< Amount of the polygons in array */

  double x_max; /**< Max value of `x` among the vertices */
  double y_max; /**< Max value of `y` among the vertices */
  double z_max; /**< Max value of `y` among the vertices */

  double x_min; /**< Min value of `x` among the vertices */
  double y_min; /**< Min value of `y` among the vertices */
  double z_min; /**< Min value of `z` among the vertices */
} object_t;

int f_parser(char *file_model, object_t *Data);
void f_clean(object_t *Data);
int f_countvf(FILE *file, object_t *Data);
int f_calloc(object_t *Data);
void f_free(object_t *Data);
int f_getstruct(FILE *file, object_t *Data);
int f_check(char *str_f);
int f_check_P(char *str_f, object_t *Data, int *rowP);
void f_getV(char *str_f, object_t *Data, int *num);
void f_getP(char *str_f, object_t *Data, int *num);
void f_printf(object_t *Data);
void f_minmax(object_t *Data);

#endif  // BACKEND_OBJECT_T_H_
