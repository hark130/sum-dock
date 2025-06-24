/*
 *  Check unit test suit for sudo_logic.h's solve_strategy_two() function.
 *
 *  Copy/paste the following from the repo's top-level directory...

make -C code dist/check_sudo_logic_solve_strategy_two.bin && \
code/dist/check_sudo_logic_solve_strategy_two.bin && CK_FORK=no valgrind --leak-check=full --show-leak-kinds=all code/dist/check_sudo_logic_solve_strategy_two.bin

 *
 *  The test cases have been split up by normal, error, boundary, and special (NEBS).
 *  Execute this command to run just one NEBS category:
 *

export CK_RUN_CASE="Normal" && ./code/dist/check_sudo_logic_solve_strategy_two.bin; unset CK_RUN_CASE  # Just run the Normal test cases
export CK_RUN_CASE="Error" && ./code/dist/check_sudo_logic_solve_strategy_two.bin; unset CK_RUN_CASE  # Just run the Error test cases
export CK_RUN_CASE="Boundary" && ./code/dist/check_sudo_logic_solve_strategy_two.bin; unset CK_RUN_CASE  # Just run the Boundary test cases
export CK_RUN_CASE="Special" && ./code/dist/check_sudo_logic_solve_strategy_two.bin; unset CK_RUN_CASE  # Just run the Special test cases

 *
 */

#ifndef SUDO_DEBUG
#define SUDO_DEBUG
#endif  /* SUDO_DEBUG */

#include <check.h>                      // START_TEST(), END_TEST, Suite
#include <errno.h>                      // EINVAL, ENODATA
#include <stdlib.h>                     // EXIT_FAILURE, EXIT_SUCCESS
// Local includes
// #include "sudo_logic.h"                 // solve_strategy_two() (Not exposed)
#include "sudo_macros.h"                // ENOERR
#include "unit_test_code.h"             // CANARY_INT, free_devops_mem()

/*
 *  This function is "internal" to sudo_logic.c but I should still be able to link against it.
 */
int solve_strategy_two(char board[81]);

/**************************************************************************************************/
/************************************ HELPER CODE DECLARATION *************************************/
/**************************************************************************************************/

/*
 *  Create the Check test suite.
 */
Suite *create_test_suite(void);

/*
 *  Make the function call, check the expected return value, and validate the results.
 */
void run_test_case(char test_input[81], char exp_solution[81], int exp_return);


/**************************************************************************************************/
/*************************************** NORMAL TEST CASES ****************************************/
/**************************************************************************************************/


// Manually execute this test case: ./code/dist/sum_dock.bin "   4      56              4                                                      "
START_TEST(test_n01_row_test_1)
{
    // LOCAL VARIABLES
    int exp_return = ENODATA;  // Expected return value for this test case
    // Expected results for this test case
    char *exp_result = "   4     456              4                                                      ";
    // The sudoku puzzle for this test case
    char test_input[81] = { "   4     "
                            " 56      "
                            "        4"
                            "         "
                            "         "
                            "         "
                            "         "
                            "         "
                            "         " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


// Manually execute this test case: ./code/dist/sum_dock.bin "                              9 7           88                                   "
START_TEST(test_n02_row_test_2)
{
    // LOCAL VARIABLES
    int exp_return = ENODATA;  // Expected return value for this test case
    // Expected results for this test case
    char *exp_result = "                              987           88                                   ";
    // The sudoku puzzle for this test case
    char test_input[81] = { "         "
                            "         "
                            "         "
                            "   9 7   "
                            "        8"
                            "8        "
                            "         "
                            "         "
                            "         " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


// Manually execute this test case: ./code/dist/sum_dock.bin "                                                      3           3           12 "
START_TEST(test_n03_row_test_3)
{
    // LOCAL VARIABLES
    int exp_return = ENODATA;  // Expected return value for this test case
    // Expected results for this test case
    char *exp_result = "                                                      3           3           123";
    // The sudoku puzzle for this test case
    char test_input[81] = { "         "
                            "         "
                            "         "
                            "         "
                            "         "
                            "         "
                            "3        "
                            "   3     "
                            "      12 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


// Manually execute this test case: ./code/dist/sum_dock.bin "         4        7         1                           1                        "
START_TEST(test_n04_col_test_1)
{
    // LOCAL VARIABLES
    int exp_return = ENODATA;  // Expected return value for this test case
    // Expected results for this test case
    char *exp_result = "         4        7         1                           1                        ";
    // The sudoku puzzle for this test case
    char test_input[81] = { "         "
                            "4        "
                            "7        "
                            " 1       "
                            "         "
                            "         "
                            "  1      "
                            "         "
                            "         " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


// Manually execute this test case: ./code/dist/sum_dock.bin "                       5       2                 8       5                       "
START_TEST(test_n05_col_test_2)
{
    // LOCAL VARIABLES
    int exp_return = ENODATA;  // Expected return value for this test case
    // Expected results for this test case
    char exp_result[81] = { "         "
                            "         "
                            "     5   "
                            "    2    "
                            "    5    "
                            "    8    "
                            "   5     "
                            "         "
                            "         " };
    // The sudoku puzzle for this test case
    char test_input[81] = { "         "
                            "         "
                            "     5   "
                            "    2    "
                            "         "
                            "    8    "
                            "   5     "
                            "         "
                            "         " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


// Manually execute this test case: ./code/dist/sum_dock.bin "                        9                           9         3        6      78 "
START_TEST(test_n06_col_test_3)
{
    // LOCAL VARIABLES
    int exp_return = ENODATA;  // Expected return value for this test case
    // Expected results for this test case
    char exp_result[81] = { "         "
                            "         "
                            "      9  "
                            "         "
                            "         "
                            "       9 "
                            "        3"
                            "        6"
                            "      789" };
    // The sudoku puzzle for this test case
    char test_input[81] = { "         "
                            "         "
                            "      9  "
                            "         "
                            "         "
                            "       9 "
                            "        3"
                            "        6"
                            "      78 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


// Manually execute this test case: ./code/dist/sum_dock.bin "  3         1     7 9       1                                                    "
START_TEST(test_n07_perp_test_1)
{
    // LOCAL VARIABLES
    int exp_return = ENODATA;  // Expected return value for this test case
    // Expected results for this test case
    char exp_result[81] = { "1 3      "
                            "   1     "
                            "7 9      "
                            " 1       "
                            "         "
                            "         "
                            "         "
                            "         "
                            "         " };
    // The sudoku puzzle for this test case
    char test_input[81] = { "  3      "
                            "   1     "
                            "7 9      "
                            " 1       "
                            "         "
                            "         "
                            "         "
                            "         "
                            "         " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


// Manually execute this test case: ./code/dist/sum_dock.bin "                              1     3           7 9                         3    "
START_TEST(test_n08_perp_test_2)
{
    // LOCAL VARIABLES
    int exp_return = ENODATA;  // Expected return value for this test case
    // Expected results for this test case
    char exp_result[81] = { "         "
                            "         "
                            "         "
                            "   1 3   "
                            "3        "
                            "   7 9   "
                            "         "
                            "         "
                            "    3    " };
    // The sudoku puzzle for this test case
    char test_input[81] = { "         "
                            "         "
                            "         "
                            "   1     "
                            "3        "
                            "   7 9   "
                            "         "
                            "         "
                            "    3    " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


// Manually execute this test case: ./code/dist/sum_dock.bin "      9                                                      23     9         78 "
START_TEST(test_n09_perp_test_3)
{
    // LOCAL VARIABLES
    int exp_return = ENODATA;  // Expected return value for this test case
    // Expected results for this test case
    char exp_result[81] = { "      9  "
                            "         "
                            "         "
                            "         "
                            "         "
                            "         "
                            "       23"
                            "     9   "
                            "      789" };
    // The sudoku puzzle for this test case
    char test_input[81] = { "      9  "
                            "         "
                            "         "
                            "         "
                            "         "
                            "         "
                            "       23"
                            "     9   "
                            "      78 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


/**************************************************************************************************/
/**************************************** ERROR TEST CASES ****************************************/
/**************************************************************************************************/


/* NOTE: The board has already been validated by the time it gets to this function so go easy. */
START_TEST(test_e01_bad_pointer)
{
    // LOCAL VARIABLES
    int exp_return = EINVAL;  // Expected return value for this test case
    char *exp_result = NULL;  // Expected results for this test case
    // The sudoku puzzle for this test case
    char *test_input = NULL;

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


/**************************************************************************************************/
/************************************** BOUNDARY TEST CASES ***************************************/
/**************************************************************************************************/


/* NOTE: The board has already been validated by the time it gets to this function so go easy. */
START_TEST(test_b01_almost_solved)
{
    // LOCAL VARIABLES
    int exp_return = ENOERR;  // Expected return value for this test case
    // Expected results for this test case
    char *exp_result = "534678912672195348198342567859761423426853791713924856961537284287419635345286179";
    // The sudoku puzzle for this test case
    char test_input[81] = { "534678912"
                            "672195348"
                            "198342567"
                            "859761423"
                            "4268 3791"
                            "713924856"
                            "961537284"
                            "287419635"
                            "345286179" };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


START_TEST(test_b02_almost_empty)
{
    // LOCAL VARIABLES
    int exp_return = ENODATA;  // Expected return value for this test case
    // Expected results for this test case
    char *exp_result = "                                        5                                        ";
    // The sudoku puzzle for this test case
    char test_input[81] = { "                                        5                                        " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


START_TEST(test_b03_empty)
{
    // LOCAL VARIABLES
    int exp_return = ENODATA;  // Expected return value for this test case
    // Expected results for this test case
    char *exp_result = "                                                                                 ";
    // The sudoku puzzle for this test case
    char test_input[81] = { "                                                                                 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


/**************************************************************************************************/
/*************************************** SPECIAL TEST CASES ***************************************/
/**************************************************************************************************/


/* SPECIAL TEST CASE PLACEHOLDER */


/**************************************************************************************************/
/************************************* HELPER CODE DEFINITION *************************************/
/**************************************************************************************************/


Suite *create_test_suite(void)
{
    // LOCAL VARIABLES
    Suite *suite = suite_create("SUDO_Logic-Solve_Strategy_Two");  // Test suite
    TCase *tc_normal = tcase_create("Normal");                     // Normal test cases
    TCase *tc_error = tcase_create("Error");                       // Error test cases
    TCase *tc_boundary = tcase_create("Boundary");                 // Error test cases
    // TCase *tc_special = tcase_create("Special");                   // Special test cases

    // SETUP TEST CASES
    tcase_add_test(tc_normal, test_n01_row_test_1);
    tcase_add_test(tc_normal, test_n02_row_test_2);
    tcase_add_test(tc_normal, test_n03_row_test_3);
    tcase_add_test(tc_normal, test_n04_col_test_1);
    tcase_add_test(tc_normal, test_n05_col_test_2);
    tcase_add_test(tc_normal, test_n06_col_test_3);
    tcase_add_test(tc_normal, test_n07_perp_test_1);
    tcase_add_test(tc_normal, test_n08_perp_test_2);
    tcase_add_test(tc_normal, test_n09_perp_test_3);
    tcase_add_test(tc_error, test_e01_bad_pointer);
    tcase_add_test(tc_boundary, test_b01_almost_solved);
    tcase_add_test(tc_boundary, test_b02_almost_empty);
    tcase_add_test(tc_boundary, test_b03_empty);
    // tcase_add_test(tc_special, test_s01_placeholder);
    suite_add_tcase(suite, tc_normal);
    suite_add_tcase(suite, tc_error);
    suite_add_tcase(suite, tc_boundary);
    // suite_add_tcase(suite, tc_special);

    // DONE
    return suite;
}


void run_test_case(char test_input[81], char exp_solution[81], int exp_return)
{
    // LOCAL VARIABLES
    int actual_ret = CANARY_INT;  // Return value of the tested function

    // RUN IT
    // Call the function
    actual_ret = solve_strategy_two(test_input);
    // Compare actual return value to expected return value
    ck_assert_msg(exp_return == actual_ret, "solve_strategy_two() returned [%d] '%s' "
                  "instead of [%d] '%s'\n", actual_ret, strerror(actual_ret),
                  exp_return, strerror(exp_return));
    // Check test_input against exp_solution
    if (NULL != exp_solution && NULL != test_input && ENOERR == exp_return)
    {
        ck_assert_msg(0 == memcmp(test_input, exp_solution,
                                  SUDO_BOARD_LEN * sizeof(exp_solution[0])),
                      "The modified test_input does not match the exp_solution");
    }

    // DONE
    return;
}


int main(void)
{
    // LOCAL VARIABLES
    int errnum = CANARY_INT;       // Results of execution
    int number_failed = 0;         // Number of test cases that failed
    Suite *suite = NULL;           // Test suite
    SRunner *suite_runner = NULL;  // Test suite runner
    // Relative path for this test case's input
    char log_rel_path[] = { "./code/test/test_output/check_sudo_logic_solve_strategy_two.log" };
    // Absolute path for log_rel_path as resolved against the repo name
    char *log_abs_path = resolve_to_repo(SUDO_REPO_NAME, log_rel_path, false, &errnum);

    // SETUP
    suite = create_test_suite();
    suite_runner = srunner_create(suite);
    srunner_set_log(suite_runner, log_abs_path);

    // RUN IT
    srunner_run_all(suite_runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(suite_runner);

    // CLEANUP
    srunner_free(suite_runner);
    free_devops_mem((void **)&log_abs_path);

    // DONE
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
