#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "backend/object_t.h"
#include "backend/parser.h"


START_TEST(test_no_file) {
  int err_orig = 2;
  int err_my;
  object_t object_my;
  err_my = parser("no_file.obj", &object_my);
  ck_assert_int_eq(err_orig, err_my);
}

START_TEST(test_empty_file) {
  int err_orig = 1;
  int err_my;
  object_t object_my;
  err_my =
      parser("../../../src/tests/backend/tests_obj/empty_file.obj", &object_my);
  ck_assert_int_eq(err_orig, err_my);
}

START_TEST(test_empty_with_vertices_polygons) {
  int err_orig = 1;
  int err_my;
  object_t object_my;
  err_my = parser(
      "../../../src/tests/backend/tests_obj/"
      "empty_with_vertices_polygons.obj",
      &object_my);
  ck_assert_int_eq(err_orig, err_my);
}

START_TEST(test_no_vertices) {
  int err_orig = 1;
  int err_my;
  object_t object_my;
  err_my = parser("../../../src/tests/backend/tests_obj/no_vertices.obj",
                  &object_my);
  ck_assert_int_eq(err_orig, err_my);
}

START_TEST(test_no_polygons) {
  int err_orig = 1;
  int err_my;
  object_t object_my;
  err_my = parser("../../../src/tests/backend/tests_obj/no_polygons.obj",
                  &object_my);
  ck_assert_int_eq(err_orig, err_my);
}

START_TEST(test_short_vertices) {
  int err_orig = 1;
  int err_my;
  object_t object_my;
  err_my =
      parser("../../../src/tests/backend/tests_obj/short_vertices.obj",
             &object_my);
  ck_assert_int_eq(err_orig, err_my);
}

START_TEST(test_short_polygons) {
  int err_orig = 1;
  int err_my;
  object_t object_my;
  err_my =
      parser("../../../src/tests/backend/tests_obj/short_polygons.obj",
             &object_my);
  ck_assert_int_eq(err_orig, err_my);
}

START_TEST(test_2vertices_2polygons) {
  double eps = 10e-7;
  int err_orig = 0;
  uint32_t vertices_amount_orig = 2;
  double vertices_orig[2][3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

  uint32_t polygons_amount_orig = 2;
  uint32_t polygons_orig[2][3] = {{1, 2, 3}, {4, 5, 6}};

  int err_my;
  object_t object_my;
  err_my = parser(
      "../../../src/tests/backend/tests_obj/2vertices_2polygons.obj",
      &object_my);

  ck_assert_int_eq(err_orig, err_my);
  ck_assert_uint_eq(vertices_amount_orig, object_my.vertices_amount);
  ck_assert_uint_eq(polygons_amount_orig, object_my.polygons_amount);
  for (uint32_t i = 0; i < vertices_amount_orig; i++) {
    ck_assert_double_eq_tol(vertices_orig[i][0], object_my.vertices[i].x, eps);
    ck_assert_double_eq_tol(vertices_orig[i][1], object_my.vertices[i].y, eps);
    ck_assert_double_eq_tol(vertices_orig[i][2], object_my.vertices[i].z, eps);
  }
  for (uint32_t i = 0; i < polygons_amount_orig; i++) {
    for (uint32_t j = 0; j < 3; j++) {
      ck_assert_uint_eq(polygons_orig[i][j],
                       object_my.polygons[i].vertex_indexes[j]);
    }
  }
  free_vertices_and_polygonc(&object_my);
}

START_TEST(test_with_invalid) {
  double eps = 10e-7;
  int err_orig = 0;
  uint32_t vertices_amount_orig = 2;
  double vertices_orig[2][3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

  uint32_t polygons_amount_orig = 2;
  uint32_t polygons_orig[2][3] = {{1, 2, 3}, {4, 5, 6}};

  int err_my;
  object_t object_my;
  err_my = parser("../../../src/tests/backend/tests_obj/with_invalid.obj",
                  &object_my);

  ck_assert_int_eq(err_orig, err_my);
  ck_assert_uint_eq(vertices_amount_orig, object_my.vertices_amount);
  ck_assert_uint_eq(polygons_amount_orig, object_my.polygons_amount);
  for (uint32_t i = 0; i < vertices_amount_orig; i++) {
    ck_assert_double_eq_tol(vertices_orig[i][0], object_my.vertices[i].x, eps);
    ck_assert_double_eq_tol(vertices_orig[i][1], object_my.vertices[i].y, eps);
    ck_assert_double_eq_tol(vertices_orig[i][2], object_my.vertices[i].z, eps);
  }
  for (uint32_t i = 0; i < polygons_amount_orig; i++) {
    for (uint32_t j = 0; j < 3; j++) {
      ck_assert_uint_eq(polygons_orig[i][j],
                       object_my.polygons[i].vertex_indexes[j]);
    }
  }
  free_vertices_and_polygonc(&object_my);
}

START_TEST(test_with_space) {
  double eps = 10e-7;
  int err_orig = 0;
  uint32_t vertices_amount_orig = 2;
  double vertices_orig[2][3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

  uint32_t polygons_amount_orig = 2;
  uint32_t polygons_orig[2][3] = {{1, 2, 3}, {4, 5, 6}};

  int err_my;
  object_t object_my;
  err_my = parser("../../../src/tests/backend/tests_obj/with_space.obj",
                  &object_my);
  ck_assert_int_eq(err_orig, err_my);
  ck_assert_uint_eq(vertices_amount_orig, object_my.vertices_amount);
  ck_assert_uint_eq(polygons_amount_orig, object_my.polygons_amount);
  for (uint32_t i = 0; i < vertices_amount_orig; i++) {
    ck_assert_double_eq_tol(vertices_orig[i][0], object_my.vertices[i].x, eps);
    ck_assert_double_eq_tol(vertices_orig[i][1], object_my.vertices[i].y, eps);
    ck_assert_double_eq_tol(vertices_orig[i][2], object_my.vertices[i].z, eps);
  }
  for (uint32_t i = 0; i < polygons_amount_orig; i++) {
    for (uint32_t j = 0; j < 3; j++) {
      ck_assert_uint_eq(polygons_orig[i][j],
                       object_my.polygons[i].vertex_indexes[j]);
    }
  }
  free_vertices_and_polygonc(&object_my);
}

START_TEST(test_with_slash) {
  double eps = 10e-7;
  int err_orig = 0;
  uint32_t vertices_amount_orig = 2;
  double vertices_orig[2][3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

  uint32_t polygons_amount_orig = 2;
  uint32_t polygons_orig[2][3] = {{1, 2, 3}, {4, 5, 6}};

  int err_my;
  object_t object_my;
  err_my = parser("../../../src/tests/backend/tests_obj/with_slash.obj",
                  &object_my);
  ck_assert_int_eq(err_orig, err_my);
  ck_assert_uint_eq(vertices_amount_orig, object_my.vertices_amount);
  ck_assert_uint_eq(polygons_amount_orig, object_my.polygons_amount);
  for (uint32_t i = 0; i < vertices_amount_orig; i++) {
    ck_assert_double_eq_tol(vertices_orig[i][0], object_my.vertices[i].x, eps);
    ck_assert_double_eq_tol(vertices_orig[i][1], object_my.vertices[i].y, eps);
    ck_assert_double_eq_tol(vertices_orig[i][2], object_my.vertices[i].z, eps);
  }
  for (uint32_t i = 0; i < polygons_amount_orig; i++) {
    for (uint32_t j = 0; j < 3; j++) {
      ck_assert_uint_eq(polygons_orig[i][j],
                       object_my.polygons[i].vertex_indexes[j]);
    }
  }
  free_vertices_and_polygonc(&object_my);
}

START_TEST(test_5vertices_5polygons) {
  double eps = 10e-7;
  int err_orig = 0;
  uint32_t vertices_amount_orig = 5;
  double vertices_orig[5][3] = {{1.0, 2.0, 3.0},
                                {4.0, 5.0, 6.0},
                                {7.0, 8.0, 9.0},
                                {100.0, 200.0, 300.0},
                                {444.444, 555.555, 666.666}};

  uint32_t polygons_amount_orig = 5;
  uint32_t polygons_orig[5][6] = {{1, 2, 3, 4, 5, 6},
                                  {99, 20, 30, 40, 50, 60},
                                  {100, 200, 300, 400, 500, 600},
                                  {1000, 2000, 3000, 4000, 5000, 6000},
                                  {11, 22, 33, 44, 55, 66}};

  int err_my;
  object_t object_my;
  err_my = parser(
      "../../../src/tests/backend/tests_obj/5vertices_5polygons.obj",
      &object_my);

  ck_assert_int_eq(err_orig, err_my);
  ck_assert_uint_eq(vertices_amount_orig, object_my.vertices_amount);
  ck_assert_uint_eq(polygons_amount_orig, object_my.polygons_amount);
  for (uint32_t i = 0; i < vertices_amount_orig; i++) {
    ck_assert_double_eq_tol(vertices_orig[i][0], object_my.vertices[i].x, eps);
    ck_assert_double_eq_tol(vertices_orig[i][1], object_my.vertices[i].y, eps);
    ck_assert_double_eq_tol(vertices_orig[i][2], object_my.vertices[i].z, eps);
  }
  for (uint32_t i = 0; i < polygons_amount_orig; i++) {
    for (uint32_t j = 0; j < 6; j++) {
      ck_assert_uint_eq(polygons_orig[i][j],
                       object_my.polygons[i].vertex_indexes[j]);
    }
  }
  free_vertices_and_polygonc(&object_my);
}

Suite *make_parser_suite(void) {
  Suite *s = suite_create("Object parser suite");
  TCase *tc = tcase_create("Core");

  suite_add_tcase(s, tc);
  
  tcase_add_test(tc, test_no_file);
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
