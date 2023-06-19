#ifndef FRONTEND_TEMP_CUBE_OBJECT_H_
#define FRONTEND_TEMP_CUBE_OBJECT_H_

extern "C" {
#include "backend/object_t.h"
#include "backend/polygon_t.h"
#include "backend/vertex_t.h"
}

#include <cstdint>

static vertex_t cube_vertices[8] = {
    {1.0, -1.0, -1.0},  {1.0, -1.0, 1.0},      {-1.0, -1.0, 1.0},
    {-1.0, -1.0, -1.0}, {1.0, 1.0, -0.999999}, {0.999999, 1.0, 1.000001},
    {-1.0, 1.0, 1.0},   {-1.0, 1.0, -1.0},
};

static uint32_t cube_polygon_vertex_indexes[12][3] = {
    {2, 3, 4}, {8, 7, 6}, {5, 6, 2}, {6, 7, 3}, {3, 7, 8}, {1, 4, 8},
    {1, 2, 4}, {5, 8, 6}, {1, 5, 2}, {2, 6, 3}, {4, 3, 8}, {5, 1, 8},
};

static polygon_t cube_polygons[12] = {
    {.vertex_indexes = cube_polygon_vertex_indexes[0], .amount = 3},
    {.vertex_indexes = cube_polygon_vertex_indexes[1], .amount = 3},
    {.vertex_indexes = cube_polygon_vertex_indexes[2], .amount = 3},
    {.vertex_indexes = cube_polygon_vertex_indexes[3], .amount = 3},
    {.vertex_indexes = cube_polygon_vertex_indexes[4], .amount = 3},
    {.vertex_indexes = cube_polygon_vertex_indexes[5], .amount = 3},
    {.vertex_indexes = cube_polygon_vertex_indexes[6], .amount = 3},
    {.vertex_indexes = cube_polygon_vertex_indexes[7], .amount = 3},
    {.vertex_indexes = cube_polygon_vertex_indexes[8], .amount = 3},
    {.vertex_indexes = cube_polygon_vertex_indexes[9], .amount = 3},
    {.vertex_indexes = cube_polygon_vertex_indexes[10], .amount = 3},
    {.vertex_indexes = cube_polygon_vertex_indexes[11], .amount = 3},
};

static object_t cube_object = {
    .vertices = (vertex_t *)cube_vertices,
    .vertices_amount = 8,
    .polygons = (polygon_t *)cube_polygons,
    .polygons_amount = 12,

    .x_min = -1.0,
    .y_min = -1.0,
    .z_min = -1.0,
    .x_max = 1.0,
    .y_max = 1.0,
    .z_max = 1.0,
};

#endif  // FRONTEND_TEMP_CUBE_OBJECT_H_