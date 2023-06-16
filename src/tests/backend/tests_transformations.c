#include <check.h>

#include "backend/object_t.h"
#include "backend/transformations.h"

vertex_t vertices[2] = {{0.0, 0.0, 0.0}};

object_t object;

void setup(void) {
  vertices[0].x = -50.0;
  vertices[0].y = 100.0;
  vertices[0].z = 200.0;

  vertices[1].x = 2.0;
  vertices[1].y = -75.0;
  vertices[1].z = 150;

  object.vertices = (vertex_t *)vertices;
  object.vertices_amount = 2;

  object.polygons = NULL;
  object.polygons_amount = 0;

  object.x_min = -50;
  object.y_min = -75;
  object.z_min = 150;

  object.x_max = 2.0;
  object.y_max = 100;
  object.z_max = 200;
}

START_TEST(test_normalize_vertices_normalize_objects_min_max_values) {
  double max_scale = 1.0;

  normalize_vertices(max_scale, &object);

  ck_assert_double_eq(object.x_min, -0.25);
  ck_assert_double_eq(object.y_min, -0.375);
  ck_assert_double_eq(object.z_min, 0.75);
  ck_assert_double_eq(object.x_max, 0.01);
  ck_assert_double_eq(object.y_max, 0.5);
  ck_assert_double_eq(object.z_max, 1);
}
END_TEST

START_TEST(test_normalize_all_vertices_values) {
  double max_scale = 1.0;

  normalize_vertices(max_scale, &object);

  vertex_t first_vertex = object.vertices[0];
  vertex_t second_vertex = object.vertices[1];
  ck_assert_double_eq(first_vertex.x, -0.25);
  ck_assert_double_eq(first_vertex.y, 0.5);
  ck_assert_double_eq(first_vertex.z, 1);
  ck_assert_double_eq(second_vertex.x, 0.01);
  ck_assert_double_eq(second_vertex.y, -0.375);
  ck_assert_double_eq(second_vertex.z, 0.75);
}
END_TEST

Suite *make_transformations_suite(void) {
  Suite *s = suite_create("Object transformations suite");
  TCase *tc = tcase_create("Core");

  suite_add_tcase(s, tc);
  tcase_add_checked_fixture(tc, setup, NULL);

  tcase_add_test(tc, test_normalize_vertices_normalize_objects_min_max_values);
  tcase_add_test(tc, test_normalize_all_vertices_values);

  return s;
}
