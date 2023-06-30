#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "backend/loader.h"
#include "backend/object_t.h"

#define OBJ_READ_OK 0
#define OBJ_FILE_INCORRECT 1
#define OBJ_FILE_NOT_FOUND 2

static int returned;
static object_t object = {
    .vertices = NULL,
    .count_vertices = 0,
    .polygons = NULL,
    .count_polygons = 0,
};

static void setup(void) {
  // Set returned to not possible values
  returned = 990;
}

static void teardown(void) {
  free_object(&object);
}

START_TEST(test_not_existed_file) {
  returned = load_object("no_existed_file.obj", &object);

  ck_assert_int_eq(returned, OBJ_FILE_NOT_FOUND);
  ck_assert_int_eq(object.count_vertices, 0);
  ck_assert_ptr_null(object.vertices);
  ck_assert_int_eq(object.count_polygons, 0);
  ck_assert_ptr_null(object.polygons);
}

START_TEST(test_empty_file) {
  returned = load_object("fixtures/empty_file.obj", &object);

  ck_assert_int_eq(returned, OBJ_FILE_INCORRECT);
  ck_assert_int_eq(object.count_vertices, 0);
  ck_assert_ptr_null(object.vertices);
  ck_assert_int_eq(object.count_polygons, 0);
  ck_assert_ptr_null(object.polygons);
}

START_TEST(test_empty_with_vertices_polygons) {
  returned = load_object("fixtures/empty_with_vertices_polygons.obj", &object);

  ck_assert_int_eq(returned, OBJ_FILE_INCORRECT);
}

START_TEST(test_no_vertices) {
  returned = load_object("fixtures/no_vertices.obj", &object);

  ck_assert_int_eq(returned, OBJ_FILE_INCORRECT);
}

START_TEST(test_no_polygons) {
  returned = load_object("fixtures/no_polygons.obj", &object);

  ck_assert_int_eq(returned, OBJ_FILE_INCORRECT);
}

START_TEST(test_short_vertices) {
  returned = load_object("fixtures/short_vertices.obj", &object);

  ck_assert_int_eq(returned, OBJ_FILE_INCORRECT);
}

START_TEST(test_short_polygons) {
  returned = load_object("fixtures/short_polygons.obj", &object);

  ck_assert_int_eq(returned, OBJ_FILE_INCORRECT);
}

START_TEST(test_2vertices_2polygons) {
  returned = load_object("fixtures/2vertices_2polygons.obj", &object);

  ck_assert_int_eq(returned, OBJ_READ_OK);
  ck_assert_uint_eq(object.count_vertices, 2);
  ck_assert_float_eq(object.vertices[0].x, 1.0);
  ck_assert_float_eq(object.vertices[0].y, 2.0);
  ck_assert_float_eq(object.vertices[0].z, 3.0);
  ck_assert_float_eq(object.vertices[1].x, 4.0);
  ck_assert_float_eq(object.vertices[1].y, 5.0);
  ck_assert_float_eq(object.vertices[1].z, 6.0);
  ck_assert_uint_eq(object.count_polygons, 2);
  ck_assert_uint_eq(object.polygons[0].count_indices, 3);
  ck_assert_float_eq(object.polygons[0].vertex_indices[0], 0);
  ck_assert_float_eq(object.polygons[0].vertex_indices[1], 1);
  ck_assert_float_eq(object.polygons[0].vertex_indices[2], 2);
  ck_assert_uint_eq(object.polygons[1].count_indices, 3);
  ck_assert_float_eq(object.polygons[1].vertex_indices[0], 3);
  ck_assert_float_eq(object.polygons[1].vertex_indices[1], 4);
  ck_assert_float_eq(object.polygons[1].vertex_indices[2], 5);
}

START_TEST(test_with_invalid) {
  double eps = 10e-7;
  int err_orig = 0;
  uint32_t vertices_amount_orig = 2;
  double vertices_orig[2][3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

  uint32_t polygons_amount_orig = 2;
  uint32_t polygons_orig[2][3] = {{0, 1, 2}, {3, 4, 5}};

  int err_my;
  object_t object_my;
  err_my = load_object("fixtures/with_invalid.obj", &object_my);

  ck_assert_int_eq(err_orig, err_my);
  ck_assert_uint_eq(vertices_amount_orig, object_my.count_vertices);
  ck_assert_uint_eq(polygons_amount_orig, object_my.count_polygons);
  for (uint32_t i = 0; i < vertices_amount_orig; i++) {
    ck_assert_double_eq_tol(vertices_orig[i][0], object_my.vertices[i].x, eps);
    ck_assert_double_eq_tol(vertices_orig[i][1], object_my.vertices[i].y, eps);
    ck_assert_double_eq_tol(vertices_orig[i][2], object_my.vertices[i].z, eps);
  }
  for (uint32_t i = 0; i < polygons_amount_orig; i++) {
    for (uint32_t j = 0; j < 3; j++) {
      ck_assert_uint_eq(polygons_orig[i][j],
                        object_my.polygons[i].vertex_indices[j]);
    }
  }
  free_object(&object_my);
}

START_TEST(test_with_space) {
  double eps = 10e-7;
  int err_orig = 0;
  uint32_t vertices_amount_orig = 2;
  double vertices_orig[2][3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

  uint32_t polygons_amount_orig = 2;
  uint32_t polygons_orig[2][3] = {{0, 1, 2}, {3, 4, 5}};

  int err_my;
  object_t object_my;
  err_my = load_object("fixtures/with_space.obj", &object_my);
  ck_assert_int_eq(err_orig, err_my);
  ck_assert_uint_eq(vertices_amount_orig, object_my.count_vertices);
  ck_assert_uint_eq(polygons_amount_orig, object_my.count_polygons);
  for (uint32_t i = 0; i < vertices_amount_orig; i++) {
    ck_assert_double_eq_tol(vertices_orig[i][0], object_my.vertices[i].x, eps);
    ck_assert_double_eq_tol(vertices_orig[i][1], object_my.vertices[i].y, eps);
    ck_assert_double_eq_tol(vertices_orig[i][2], object_my.vertices[i].z, eps);
  }
  for (uint32_t i = 0; i < polygons_amount_orig; i++) {
    for (uint32_t j = 0; j < 3; j++) {
      ck_assert_uint_eq(polygons_orig[i][j],
                        object_my.polygons[i].vertex_indices[j]);
    }
  }
  free_object(&object_my);
}

START_TEST(test_with_slash) {
  double eps = 10e-7;
  int err_orig = 0;
  uint32_t vertices_amount_orig = 2;
  double vertices_orig[2][3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

  uint32_t polygons_amount_orig = 2;
  uint32_t polygons_orig[2][3] = {{0, 1, 2}, {3, 4, 5}};

  int err_my;
  object_t object_my;
  err_my = load_object("fixtures/with_slash.obj", &object_my);
  ck_assert_int_eq(err_orig, err_my);
  ck_assert_uint_eq(vertices_amount_orig, object_my.count_vertices);
  ck_assert_uint_eq(polygons_amount_orig, object_my.count_polygons);
  for (uint32_t i = 0; i < vertices_amount_orig; i++) {
    ck_assert_double_eq_tol(vertices_orig[i][0], object_my.vertices[i].x, eps);
    ck_assert_double_eq_tol(vertices_orig[i][1], object_my.vertices[i].y, eps);
    ck_assert_double_eq_tol(vertices_orig[i][2], object_my.vertices[i].z, eps);
  }
  for (uint32_t i = 0; i < polygons_amount_orig; i++) {
    for (uint32_t j = 0; j < 3; j++) {
      ck_assert_uint_eq(polygons_orig[i][j],
                        object_my.polygons[i].vertex_indices[j]);
    }
  }
  free_object(&object_my);
}

START_TEST(test_5vertices_5polygons) {
  double eps = 10e-7;
  int err_orig = 0;
  uint32_t vertices_amount_orig = 5;
  double vertices_orig[5][3] = {{-1.1, -2.2, -3.3},
                                {4.0, 5.0, 6.0},
                                {7.0, 8.0, 9.0},
                                {100.0, 200.0, 300.0},
                                {444.444, 555.555, 666.666}};

  uint32_t polygons_amount_orig = 5;
  uint32_t polygons_orig[5][6] = {{0, 1, 2, 3, 4, 5},
                                  {98, 19, 29, 39, 49, 59},
                                  {100, 200, 300, 400, 500, 600},
                                  {1000, 2000, 3000, 4000, 5000, 6000},
                                  {11, 22, 33, 44, 55, 66}};

  int err_my;
  object_t object_my;
  err_my = load_object("fixtures/5vertices_5polygons.obj", &object_my);

  ck_assert_int_eq(err_orig, err_my);
  ck_assert_uint_eq(vertices_amount_orig, object_my.count_vertices);
  ck_assert_uint_eq(polygons_amount_orig, object_my.count_polygons);
  for (uint32_t i = 0; i < vertices_amount_orig; i++) {
    ck_assert_double_eq_tol(vertices_orig[i][0], object_my.vertices[i].x, eps);
    ck_assert_double_eq_tol(vertices_orig[i][1], object_my.vertices[i].y, eps);
    ck_assert_double_eq_tol(vertices_orig[i][2], object_my.vertices[i].z, eps);
  }
  for (uint32_t i = 0; i < polygons_amount_orig; i++) {
    for (uint32_t j = 0; j < 6; j++) {
      ck_assert_uint_eq(polygons_orig[i][j],
                        object_my.polygons[i].vertex_indices[j]);
    }
  }
  free_object(&object_my);
}

Suite *make_loader_suite(void) {
  Suite *s = suite_create("Object loader suite");
  TCase *tc = tcase_create("Core");

  suite_add_tcase(s, tc);
  tcase_add_checked_fixture(tc, setup, teardown);

  tcase_add_test(tc, test_not_existed_file);
  tcase_add_test(tc, test_empty_file);
  tcase_add_test(tc, test_empty_with_vertices_polygons);
  tcase_add_test(tc, test_no_vertices);
  tcase_add_test(tc, test_no_polygons);
  tcase_add_test(tc, test_short_vertices);
  tcase_add_test(tc, test_short_polygons);

  tcase_add_test(tc, test_2vertices_2polygons);
  tcase_add_test(tc, test_with_invalid);
  tcase_add_test(tc, test_with_space);
  tcase_add_test(tc, test_with_slash);
  tcase_add_test(tc, test_5vertices_5polygons);

  return s;
}
