#include <check.h>

#include "backend/object_t.h"
#include "backend/transformations.h"

vertex_t vertices[2] = {{0.0, 0.0, 0.0}};

object_t object;

static void add_value_to_object_coordinates(double value, object_t *object) {
  for (uint32_t i = 0; i != object->vertices_amount; ++i) {
    object->vertices[i].x += value;
    object->vertices[i].y += value;
    object->vertices[i].z += value;
  }

  object->x_min += value;
  object->y_min += value;
  object->z_min += value;
  object->x_max += value;
  object->y_max += value;
  object->z_max += value;
}

void setup(void) {
  vertices[0].x = 50.0;
  vertices[0].y = 100.0;
  vertices[0].z = 200.0;

  vertices[1].x = -50.0;
  vertices[1].y = -100.0;
  vertices[1].z = -200.0;

  object.vertices = (vertex_t *)vertices;
  object.vertices_amount = 2;

  object.polygons = NULL;
  object.polygons_amount = 0;

  object.x_min = -50;
  object.y_min = -100;
  object.z_min = -200;

  object.x_max = 50;
  object.y_max = 100;
  object.z_max = 200;
}

START_TEST(test_normalize_vertices_set_min_max_not_more_than_scale) {
  double max_scale = 1.0;

  normalize_vertices(max_scale, &object);

  ck_assert_double_eq(object.x_min, -0.25);
  ck_assert_double_eq(object.y_min, -0.5);
  ck_assert_double_eq(object.z_min, -1.0);
  ck_assert_double_eq(object.x_max, 0.25);
  ck_assert_double_eq(object.y_max, 0.5);
  ck_assert_double_eq(object.z_max, 1.0);
}
END_TEST

START_TEST(test_normalize_vertices_set_vertices_coordinates_in_scale) {
  double max_scale = 1.0;

  normalize_vertices(max_scale, &object);

  vertex_t first_vertex = object.vertices[0];
  vertex_t second_vertex = object.vertices[1];
  ck_assert_double_eq(first_vertex.x, 0.25);
  ck_assert_double_eq(first_vertex.y, 0.5);
  ck_assert_double_eq(first_vertex.z, 1);
  ck_assert_double_eq(second_vertex.x, -0.25);
  ck_assert_double_eq(second_vertex.y, -0.5);
  ck_assert_double_eq(second_vertex.z, -1.0);
}
END_TEST

START_TEST(test_normalize_vertices_centers_objects_min_max) {
  add_value_to_object_coordinates(1000.0, &object);

  normalize_vertices(1.0, &object);

  ck_assert_double_eq(object.x_min, -0.25);
  ck_assert_double_eq(object.y_min, -0.5);
  ck_assert_double_eq(object.z_min, -1.0);
  ck_assert_double_eq(object.x_max, 0.25);
  ck_assert_double_eq(object.y_max, 0.5);
  ck_assert_double_eq(object.z_max, 1.0);
}
END_TEST

START_TEST(test_normalize_vertices_centers_vertices_coordinates) {
  add_value_to_object_coordinates(1000.0, &object);

  normalize_vertices(1.0, &object);

  vertex_t first_vertex = object.vertices[0];
  vertex_t second_vertex = object.vertices[1];
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

  tcase_add_test(tc, test_normalize_vertices_set_min_max_not_more_than_scale);
  tcase_add_test(tc, test_normalize_vertices_set_vertices_coordinates_in_scale);
  tcase_add_test(tc, test_normalize_vertices_centers_objects_min_max);
  tcase_add_test(tc, test_normalize_vertices_centers_vertices_coordinates);

  return s;
}
