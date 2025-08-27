#include "../include/s21_decimal.h"
#include "../include/s21_helpers.h"
#include "../include/s21_suites.h"

Suite* s21_mul_suite() {
  Suite* ps = suite_create("add");
  TCase* tc = tcase_create("core");

  suite_add_tcase(ps, tc);
  return ps;
}