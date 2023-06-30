#include <check.h>

#include "backend/object_t.h"
#include "backend/transformations.h"

static vector_t vertices[2] = {{0.0, 0.0, 0.0}};

static object_t object;

static void add_value_to_object_coordinates(double value, object_t *object) {
  for (uint32_t i = 0; i != object->count_vertices; ++i) {
    object->vertices[i].x += value;
    object->vertices[i].y += value;
    object->vertices[i].z += value;
  }
}

void setup(void) {
  vertices[0].x = 50.0;
  vertices[0].y = 100.0;
  vertices[0].z = 200.0;

  vertices[1].x = -50.0;
  vertices[1].y = -100.0;
  vertices[1].z = -200.0;

  object.vertices = (vector_t *)vertices;
  object.count_vertices = 2;

  object.polygons = NULL;
  object.count_polygons = 0;
}

START_TEST(test_move_x_actually_move_vertices_by_x) {
  object_move_x_axis(100, &object);

  vector_t first_vertex = object.vertices[0];
  vector_t second_vertex = object.vertices[1];
  ck_assert_double_eq(first_vertex.x, 150);
  ck_assert_double_eq(first_vertex.y, 100);
  ck_assert_double_eq(first_vertex.z, 200);
  ck_assert_double_eq(second_vertex.x, 50);
  ck_assert_double_eq(second_vertex.y, -100);
  ck_assert_double_eq(second_vertex.z, -200);
}

START_TEST(test_move_y_actually_move_vertices_by_y) {
  object_move_y_axis(100, &object);

  vector_t first_vertex = object.vertices[0];
  vector_t second_vertex = object.vertices[1];
  ck_assert_double_eq(first_vertex.x, 50);
  ck_assert_double_eq(first_vertex.y, 200);
  ck_assert_double_eq(first_vertex.z, 200);
  ck_assert_double_eq(second_vertex.x, -50);
  ck_assert_double_eq(second_vertex.y, 0);
  ck_assert_double_eq(second_vertex.z, -200);
}

START_TEST(test_move_z_actually_move_vertices_by_z) {
  object_move_z_axis(100, &object);

  vector_t first_vertex = object.vertices[0];
  vector_t second_vertex = object.vertices[1];
  ck_assert_double_eq(first_vertex.x, 50);
  ck_assert_double_eq(first_vertex.y, 100);
  ck_assert_double_eq(first_vertex.z, 300);
  ck_assert_double_eq(second_vertex.x, -50);
  ck_assert_double_eq(second_vertex.y, -100);
  ck_assert_double_eq(second_vertex.z, -100);
}

START_TEST(test_rotate_x_actually_rotate_x) {
  double eps = 10e-7;

  object_rotate_x_axis(90, &object);

  vector_t first_vertex = object.vertices[0];
  ck_assert_double_eq(first_vertex.x, 50);
  ck_assert_double_eq_tol(first_vertex.y, -200.0, eps);
  ck_assert_double_eq_tol(first_vertex.z, 100.0, eps);
}

START_TEST(test_rotate_y_actually_rotate_y) {
  double eps = 10e-7;

  object_rotate_y_axis(90, &object);

  vector_t first_vertex = object.vertices[0];
  ck_assert_double_eq_tol(first_vertex.x, 200, eps);
  ck_assert_double_eq(first_vertex.y, 100.0);
  ck_assert_double_eq_tol(first_vertex.z, -50.0, eps);
}

START_TEST(test_rotate_z_actually_rotate_z) {
  double eps = 10e-7;

  object_rotate_z_axis(90, &object);

  vector_t first_vertex = object.vertices[0];
  ck_assert_double_eq_tol(first_vertex.x, -100.0, eps);
  ck_assert_double_eq_tol(first_vertex.y, 50.0, eps);
  ck_assert_double_eq(first_vertex.z, 200.0);
}

START_TEST(test_rotate_all_vertices) {
  double eps = 10e-7;

  object_rotate_x_axis(90, &object);
  object_rotate_y_axis(90, &object);
  object_rotate_z_axis(90, &object);

  vector_t first_vertex = object.vertices[0];
  vector_t second_vertex = object.vertices[1];
  ck_assert_double_eq_tol(first_vertex.x, 200.0, eps);
  ck_assert_double_eq_tol(first_vertex.y, 100.0, eps);
  ck_assert_double_eq_tol(first_vertex.z, -50.0, eps);
  ck_assert_double_eq_tol(second_vertex.x, -200.0, eps);
  ck_assert_double_eq_tol(second_vertex.y, -100.0, eps);
  ck_assert_double_eq_tol(second_vertex.z, 50.0, eps);
}

START_TEST(test_scale_actually_scale_vertex_axis_values) {
  object_scale(10.0, &object);

  vector_t first_vertex = object.vertices[0];
  vector_t second_vertex = object.vertices[1];
  ck_assert_double_eq(first_vertex.x, 500);
  ck_assert_double_eq(first_vertex.y, 1000);
  ck_assert_double_eq(first_vertex.z, 2000);
  ck_assert_double_eq(second_vertex.x, -500);
  ck_assert_double_eq(second_vertex.y, -1000);
  ck_assert_double_eq(second_vertex.z, -2000);
}

START_TEST(test_zero_scale_do_nothing) {
  object_scale(0.0, &object);

  vector_t first_vertex = object.vertices[0];
  ck_assert_double_eq(first_vertex.x, 50);
  ck_assert_double_eq(first_vertex.y, 100);
  ck_assert_double_eq(first_vertex.z, 200);
}

START_TEST(test_normalize_set_vertices_coordinates_in_scale) {
  double max_scale = 1.0;

  object_normalize(max_scale, &object);

  vector_t first_vertex = object.vertices[0];
  vector_t second_vertex = object.vertices[1];
  ck_assert_double_eq(first_vertex.x, 0.25);
  ck_assert_double_eq(first_vertex.y, 0.5);
  ck_assert_double_eq(first_vertex.z, 1);
  ck_assert_double_eq(second_vertex.x, -0.25);
  ck_assert_double_eq(second_vertex.y, -0.5);
  ck_assert_double_eq(second_vertex.z, -1.0);
}
END_TEST

START_TEST(test_normalize_centers_vertices_coordinates) {
  add_value_to_object_coordinates(1000.0, &object);

  object_normalize(1.0, &object);

  vector_t first_vertex = object.vertices[0];
  vector_t second_vertex = object.vertices[1];
  ck_assert_double_eq(first_vertex.x, 0.25);
  ck_assert_double_eq(first_vertex.y, 0.5);
  ck_assert_double_eq(first_vertex.z, 1);
  ck_assert_double_eq(second_vertex.x, -0.25);
  ck_assert_double_eq(second_vertex.y, -0.5);
  ck_assert_double_eq(second_vertex.z, -1.0);
}
END_TEST

Suite *make_transformations_suite(void) {
  Suite *s = suite_create("Object transformations suite");
  TCase *tc = tcase_create("Core");

  suite_add_tcase(s, tc);
  tcase_add_checked_fixture(tc, setup, NULL);

  tcase_add_test(tc, test_move_x_actually_move_vertices_by_x);
  tcase_add_test(tc, test_move_y_actually_move_vertices_by_y);
  tcase_add_test(tc, test_move_z_actually_move_vertices_by_z);
  tcase_add_test(tc, test_rotate_x_actually_rotate_x);
  tcase_add_test(tc, test_rotate_y_actually_rotate_y);
  tcase_add_test(tc, test_rotate_z_actually_rotate_z);
  tcase_add_test(tc, test_rotate_all_vertices);
  tcase_add_test(tc, test_scale_actually_scale_vertex_axis_values);
  tcase_add_test(tc, test_zero_scale_do_nothing);
  tcase_add_test(tc, test_normalize_set_vertices_coordinates_in_scale);
  tcase_add_test(tc, test_normalize_centers_vertices_coordinates);

  return s;
}
