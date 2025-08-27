#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/s21_suites.h"
#define FILENAME_SIZE 128

int main(void) {
  int number_failed;
  SRunner *sr = srunner_create(s21_helpers_suite());

  //  TODO(all): add after comment your suits...
  srunner_add_suite(sr, s21_add_suite());
  srunner_add_suite(sr, s21_sub_suite());
  srunner_add_suite(sr, s21_mul_suite());
  srunner_add_suite(sr, s21_div_suite());

  //  Check for CK_RUN_SUITE and set a custom log file
  const char *suite = getenv("CK_RUN_SUITE");
  if (suite && strlen(suite) > 0) {
    char logname[FILENAME_SIZE];
    snprintf(logname, sizeof(logname), "./%s.log", suite);
    srunner_set_log(sr, logname);
  } else {
    srunner_set_log(sr, "./test.log");
  }

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
