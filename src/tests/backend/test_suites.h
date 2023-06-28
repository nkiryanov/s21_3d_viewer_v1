#ifndef TESTS_BACKEND_TEST_SUITES_H_
#define TESTS_BACKEND_TEST_SUITES_H_

#include <check.h>

Suite *make_master_suite(void);
Suite *make_transformations_suite(void);
Suite *make_loader_suite(void);

#endif  // TESTS_BACKEND_TEST_SUITES_H_
