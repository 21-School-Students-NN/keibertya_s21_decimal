#include "../include/suites.h"
#include "../include/s21_decimal.h"

// Базовое сложение положительных чисел
START_TEST(test_add_positive_basic) {
    s21_decimal a = {{5, 0, 0, 0}};
    s21_decimal b = {{3, 0, 0, 0}};
    s21_decimal result;
    int error = s21_add(a, b, &result);
    
    ck_assert_int_eq(error, 0);
    ck_assert_int_eq(result.bits[0], 8);
    ck_assert_int_eq(result.bits[1], 0);
    ck_assert_int_eq(result.bits[2], 0);
    ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

// Сложение с разными scale
START_TEST(test_add_different_scale) {
    s21_decimal a = {{123, 0, 0, 0x00010000}};  // 12.3
    s21_decimal b = {{45, 0, 0, 0x00020000}};   // 0.45
    s21_decimal result;
    int error = s21_add(a, b, &result);
    
    ck_assert_int_eq(error, 0);
    ck_assert_int_eq(result.bits[0], 1275);  // 12.75 * 100
    ck_assert_int_eq(result.bits[3] >> 16 & 0xFF, 2);  // scale=2
}
END_TEST

// Положительное + отрицательное = вычитание
START_TEST(test_add_positive_negative) {
    s21_decimal a = {{10, 0, 0, 0}};        // 10
    s21_decimal b = {{3, 0, 0, 0x80000000}}; // -3
    s21_decimal result;
    int error = s21_add(a, b, &result);
    
    ck_assert_int_eq(error, 0);
    ck_assert_int_eq(result.bits[0], 7);
    ck_assert_int_eq(result.bits[3], 0);  // положительный результат
}
END_TEST

// Оба отрицательные
START_TEST(test_add_both_negative) {
    s21_decimal a = {{5, 0, 0, 0x80000000}}; // -5
    s21_decimal b = {{2, 0, 0, 0x80000000}}; // -2
    s21_decimal result;
    int error = s21_add(a, b, &result);
    
    ck_assert_int_eq(error, 0);
    ck_assert_int_eq(result.bits[0], 7);
    ck_assert_int_eq(result.bits[3], 0x80000000);  // отрицательный результат
}
END_TEST

// Переполнение (число слишком велико)
START_TEST(test_add_overflow) {
    s21_decimal max_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    s21_decimal one = {{1, 0, 0, 0}};
    s21_decimal result;
    int error = s21_add(max_val, one, &result);
    
    ck_assert_int_eq(error, 1);  // Ошибка переполнения
}
END_TEST

// Переполнение отрицательное (число слишком мало)
START_TEST(test_add_underflow) {
    s21_decimal min_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
    s21_decimal one = {{1, 0, 0, 0}};
    s21_decimal result;
    int error = s21_sub(min_val, one, &result);
    
    ck_assert_int_eq(error, 2);  // Ошибка нижнего переполнения
}
END_TEST

// Базовое вычитание положительных
START_TEST(test_sub_positive_basic) {
    s21_decimal a = {{10, 0, 0, 0}};
    s21_decimal b = {{3, 0, 0, 0}};
    s21_decimal result;
    int error = s21_sub(a, b, &result);
    
    ck_assert_int_eq(error, 0);
    ck_assert_int_eq(result.bits[0], 7);
    ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

// Вычитание с отрицательным результатом
START_TEST(test_sub_negative_result) {
    s21_decimal a = {{3, 0, 0, 0}};
    s21_decimal b = {{10, 0, 0, 0}};
    s21_decimal result;
    int error = s21_sub(a, b, &result);
    
    ck_assert_int_eq(error, 0);
    ck_assert_int_eq(result.bits[0], 7);
    ck_assert_int_eq(result.bits[3], 0x80000000);  // отрицательный
}
END_TEST

// Вычитание нуля
START_TEST(test_sub_zero) {
    s21_decimal a = {{100, 0, 0, 0}};
    s21_decimal b = {{0, 0, 0, 0}};
    s21_decimal result;
    int error = s21_sub(a, b, &result);
    
    ck_assert_int_eq(error, 0);
    ck_assert_int_eq(result.bits[0], 100);
    ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

// Вычитание равных чисел
START_TEST(test_sub_equal_numbers) {
    s21_decimal a = {{42, 0, 0, 0}};
    s21_decimal b = {{42, 0, 0, 0}};
    s21_decimal result;
    int error = s21_sub(a, b, &result);
    
    ck_assert_int_eq(error, 0);
    ck_assert_int_eq(result.bits[0], 0);
    ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

// Сложение с большим scale
START_TEST(test_add_large_scale) {
    s21_decimal a = {{123456789, 0, 0, 0x00080000}}; // 1.23456789
    s21_decimal b = {{1, 0, 0, 0x00010000}};         // 0.1
    s21_decimal result;
    int error = s21_add(a, b, &result);
    
    ck_assert_int_eq(error, 0);
    // Ожидается 1.33456789 с scale=8
    ck_assert_int_eq(result.bits[0], 133456789);
    ck_assert_int_eq(result.bits[3] >> 16 & 0xFF, 8);
}
END_TEST

// Некорректный decimal (ненулевые младшие биты)
// START_TEST(test_add_invalid_decimal) {
//     s21_decimal a = {{1, 0, 0, 0x0000FF00}}; // Некорректный
//     s21_decimal b = {{1, 0, 0, 0}};
//     s21_decimal result;
//     int error = s21_add(a, b, &result);
    
//     // Ожидается либо ошибка, либо корректная обработка
//     // (зависит от реализации)
//     ck_assert(error == 0 || error > 0);
// }
// END_TEST

// NULL pointer test
// START_TEST(test_add_null_pointer) {
//     s21_decimal a = {{1, 0, 0, 0}};
//     s21_decimal b = {{1, 0, 0, 0}};
//     int error = s21_add(a, b, NULL);
    
//     ck_assert_int_eq(error, 1);  // Ожидается ошибка
// }
// END_TEST

Suite *s21_add_sub_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Decimal Add/Sub");

    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_add_positive_basic);
    tcase_add_test(tc_core, test_add_different_scale);
    tcase_add_test(tc_core, test_add_positive_negative);
    tcase_add_test(tc_core, test_add_both_negative);
    tcase_add_test(tc_core, test_add_overflow);
    tcase_add_test(tc_core, test_add_underflow);
    tcase_add_test(tc_core, test_sub_positive_basic);
    tcase_add_test(tc_core, test_sub_negative_result);
    tcase_add_test(tc_core, test_sub_zero);
    tcase_add_test(tc_core, test_sub_equal_numbers);
    tcase_add_test(tc_core, test_add_large_scale);
    //tcase_add_test(tc_core, test_add_invalid_decimal);
    //tcase_add_test(tc_core, test_add_null_pointer);

    suite_add_tcase(s, tc_core);

    return s;
}