/*
 *  Check unit test suit for sudo_logic.h's solve_board() function.
 *
 *  Copy/paste the following from the repo's top-level directory...

make -C code dist/check_sudo_logic_solve_board.bin
code/dist/check_sudo_logic_solve_board.bin && CK_FORK=no valgrind --leak-check=full --show-leak-kinds=all code/dist/check_sudo_logic_solve_board.bin

 *
 *  The test cases have been split up by normal, error, boundary, and special (NEBS).
 *  Execute this command to run just one NEBS category:
 *

export CK_RUN_CASE="Normal" && ./code/dist/check_sudo_logic_solve_board.bin; unset CK_RUN_CASE  # Just run the Normal test cases
export CK_RUN_CASE="Error" && ./code/dist/check_sudo_logic_solve_board.bin; unset CK_RUN_CASE  # Just run the Error test cases
export CK_RUN_CASE="Boundary" && ./code/dist/check_sudo_logic_solve_board.bin; unset CK_RUN_CASE  # Just run the Boundary test cases
export CK_RUN_CASE="Special" && ./code/dist/check_sudo_logic_solve_board.bin; unset CK_RUN_CASE  # Just run the Special test cases

 *
 */

#ifndef SUDO_DEBUG
#define SUDO_DEBUG
#endif  /* SUDO_DEBUG */

#include <check.h>                      // START_TEST(), END_TEST, Suite
#include <errno.h>                      // EINVAL, ENODATA
#include <stdlib.h>                     // EXIT_FAILURE, EXIT_SUCCESS
// Local includes
#include "sudo_logic.h"                 // solve_board()
#include "sudo_macros.h"                // ENOERR
#include "unit_test_code.h"             // CANARY_INT, free_devops_mem()


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


// Take from: https://sudoku.com/easy/
// Manually execute this test case: ./code/dist/sum_dock.bin " 683 9 7  42     11 7 5 6    5 7 12 7    158     3 74    19 2 58 162 39 9  543 1 "
START_TEST(test_n01_easy_sudoku)
{
    // LOCAL VARIABLES
    int exp_return = ENOERR;  // Expected return value for this test case
    char *exp_result = NULL;  // Expected results for this test case
    // The sudoku puzzle for this test case
    char test_input[81] = { " 683 9 7 "
                            " 42     1"
                            "1 7 5 6  "
                            "  5 7 12 "
                            "7    158 "
                            "    3 74 "
                            "   19 2 5"
                            "8 162 39 "
                            "9  543 1 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


// Take from: https://sudoku.com/medium/
// Manually execute this test case: ./code/dist/sum_dock.bin "415 782  7 3 29 81   5 14  396  5       326    4 963  6  98  35 31    46 4    72 "
START_TEST(test_n02_medium_sudoku)
{
    // LOCAL VARIABLES
    int exp_return = ENOERR;  // Expected return value for this test case
    char *exp_result = NULL;  // Expected results for this test case
    // The sudoku puzzle for this test case
    char test_input[81] = { "415 782  "
                            "7 3 29 81"
                            "   5 14  "
                            "396  5   "
                            "    326  "
                            "  4 963  "
                            "6  98  35"
                            " 31    46"
                            " 4    72 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


// Take from: https://sudoku.com/hard/
// Manually execute this test case: ./code/dist/sum_dock.bin " 5   3   9 6    783   28  1        7 136 298 8 2    1   91   3 28       7 52   96"
START_TEST(test_n03_hard_sudoku)
{
    // LOCAL VARIABLES
    int exp_return = ENOERR;  // Expected return value for this test case
    char *exp_result = NULL;  // Expected results for this test case
    // The sudoku puzzle for this test case
    char test_input[81] = { " 5   3   "
                            "9 6    78"
                            "3   28  1"
                            "        7"
                            " 136 298 "
                            "8 2    1 "
                            "  91   3 "
                            "28       "
                            "7 52   96" };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


START_TEST(test_n04_expert_sudoku)
{

}
END_TEST


START_TEST(test_n05_master_sudoku)
{

}
END_TEST


START_TEST(test_n06_extreme_sudoku)
{

}
END_TEST


// Taken from: https://sandiway.arizona.edu/sudoku/examples.html
// Manually execute this test case: ./code/dist/sum_dock.bin "   26 7 168  7  9 19   45  82 1   4   46 29   5   3 23  93   74 4  5  367 3 18   "
START_TEST(test_n07_easiest_puzzle_1)
{
    // LOCAL VARIABLES
    int exp_return = ENOERR;  // Expected return value for this test case
    // Expected results for this test case
    char *exp_result = "435269781682571493197834562826195347374682915951743628519326874248957136763418259";
    // The sudoku puzzle for this test case
    char test_input[81] = { "   26 7 1"
                            "68  7  9 "
                            "19   45  "
                            "82 1   4 "
                            "  46 29  "
                            " 5   3 23"
                            "  93   74"
                            " 4  5  36"
                            "7 3 18   " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


// Taken from: https://sandiway.arizona.edu/sudoku/examples.html
// Manually execute this test case: ./code/dist/sum_dock.bin "1  489  673     4      1295  712 6  5  7 3  8  6 957  9146      2     378  512  4"
START_TEST(test_n08_easiest_puzzle_2)
{
    // LOCAL VARIABLES
    int exp_return = ENOERR;  // Expected return value for this test case
    // Expected results for this test case
    char *exp_result = "152489376739256841468371295387124659591763428246895713914637582625948137873512964";
    // The sudoku puzzle for this test case
    char test_input[81] = { "1  489  6"
                            "73     4 "
                            "     1295"
                            "  712 6  "
                            "5  7 3  8"
                            "  6 957  "
                            "9146     "
                            " 2     37"
                            "8  512  4" };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


// Taken from: https://sandiway.arizona.edu/sudoku/examples.html
// Manually execute this test case: ./code/dist/sum_dock.bin " 2 6 8   58   97      4    37    5  6       4  8    13    2      98   36   3 6 9 "
START_TEST(test_n09_intermediate_puzzle)
{
    // LOCAL VARIABLES
    int exp_return = ENOERR;  // Expected return value for this test case
    // Expected results for this test case
    char *exp_result = "123678945584239761967145328372461589691583274458792613836924157219857436745316892";
    // The sudoku puzzle for this test case
    char test_input[81] = { " 2 6 8   "
                            "58   97  "
                            "    4    "
                            "37    5  "
                            "6       4"
                            "  8    13"
                            "    2    "
                            "  98   36"
                            "   3 6 9 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


// Taken from: https://sandiway.arizona.edu/sudoku/examples.html
// Manually execute this test case: ./code/dist/sum_dock.bin "   6  4  7    36      91 8           5 18   3   3 6 45 4 2   6 9 3       2    1  "
START_TEST(test_n10_difficult_puzzle_1)
{
    // LOCAL VARIABLES
    int exp_return = ENOERR;  // Expected return value for this test case
    // Expected results for this test case
    char *exp_result = "581572439792843651364591782438957216256184973179326845845219367913768524627435198";
    // The sudoku puzzle for this test case
    char test_input[81] = { " 2 6 8   "
                            "58   97  "
                            "    4    "
                            "37    5  "
                            "6       4"
                            "  8    13"
                            "    2    "
                            "  98   36"
                            "   3 6 9 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


// Taken from: https://sandiway.arizona.edu/sudoku/examples.html
// Manually execute this test case: ./code/dist/sum_dock.bin "2  3     8 4 62  3 138  2      2 39 5 7   621 32  6    2   914 6 125 8 9     1  2"
START_TEST(test_n11_difficult_puzzle_2)
{
    // LOCAL VARIABLES
    int exp_return = ENOERR;  // Expected return value for this test case
    // Expected results for this test case
    char *exp_result = "276314958854962713913875264468127395597438621132596487325789146641253879789641532";
    // The sudoku puzzle for this test case
    char test_input[81] = { "2  3     "
                            "8 4 62  3"
                            " 138  2  "
                            "    2 39 "
                            "5 7   621"
                            " 32  6   "
                            " 2   914 "
                            "6 125 8 9"
                            "     1  2" };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


// Taken from: https://sandiway.arizona.edu/sudoku/examples.html
// Manually execute this test case: ./code/dist/sum_dock.bin " 2          6    3 74 8         3  2 8  4  1 6  5         1 78 5    9          4 "
START_TEST(test_n12_not_fun_puzzle)
{
    // LOCAL VARIABLES
    int exp_return = ENOERR;  // Expected return value for this test case
    // Expected results for this test case
    char *exp_result = "126437958895621473374985126457193862983246517612578394269314785548769231731852649";
    // The sudoku puzzle for this test case
    char test_input[81] = { " 2       "
                            "   6    3"
                            " 74 8    "
                            "     3  2"
                            " 8  4  1 "
                            "6  5     "
                            "    1 78 "
                            "5    9   "
                            "       4 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


/**************************************************************************************************/
/**************************************** ERROR TEST CASES ****************************************/
/**************************************************************************************************/


START_TEST(test_e01_invalid_char_nul)
{
    // LOCAL VARIABLES
    int exp_return = EINVAL;  // Expected return value for this test case
    char *exp_result = NULL;  // Expected results for this test case
    // The sudoku puzzle for this test case
    char test_input[81] = { "        5"
                            "5 93   2 "
                            "  \0  739 "
                            "  325814 "
                            "6 5173  2"
                            "182 9  37"
                            "8 4    7 "
                            "2 7  5461"
                            "31 7   5 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


START_TEST(test_e02_invalid_char_zero)
{
    // LOCAL VARIABLES
    int exp_return = EINVAL;  // Expected return value for this test case
    char *exp_result = NULL;  // Expected results for this test case
    // The sudoku puzzle for this test case
    char test_input[81] = { "        5"
                            "5 93   2 "
                            "  0  739 "
                            "  325814 "
                            "6 5173  2"
                            "182 9  37"
                            "8 4    7 "
                            "2 7  5461"
                            "31 7   5 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


START_TEST(test_e03_invalid_char_letter)
{
    // LOCAL VARIABLES
    int exp_return = EINVAL;  // Expected return value for this test case
    char *exp_result = NULL;  // Expected results for this test case
    // The sudoku puzzle for this test case
    char test_input[81] = { "        5"
                            "5 93   2 "
                            "  A  739 "
                            "  325814 "
                            "6 5173  2"
                            "182 9  37"
                            "8 4    7 "
                            "2 7  5461"
                            "31 7   5 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


START_TEST(test_e04_invalid_char_tab)
{
    // LOCAL VARIABLES
    int exp_return = EINVAL;  // Expected return value for this test case
    char *exp_result = NULL;  // Expected results for this test case
    // The sudoku puzzle for this test case
    char test_input[81] = { "        5"
                            "5 93   2 "
                            "  \t  739 "
                            "  325814 "
                            "6 5173  2"
                            "182 9  37"
                            "8 4    7 "
                            "2 7  5461"
                            "31 7   5 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


START_TEST(test_e05_invalid_char_newline)
{
    // LOCAL VARIABLES
    int exp_return = EINVAL;  // Expected return value for this test case
    char *exp_result = NULL;  // Expected results for this test case
    // The sudoku puzzle for this test case
    char test_input[81] = { "        5"
                            "5 93   2 "
                            "  \n  739 "
                            "  325814 "
                            "6 5173  2"
                            "182 9  37"
                            "8 4    7 "
                            "2 7  5461"
                            "31 7   5 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


START_TEST(test_e06_bad_pointer)
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


START_TEST(test_e07_invalid_board_row_dupe)
{
    // LOCAL VARIABLES
    int exp_return = EINVAL;  // Expected return value for this test case
    char *exp_result = NULL;  // Expected results for this test case
    // The sudoku puzzle for this test case
    char test_input[81] = { " 683 9 7 "
                            " 42     1"
                            "1 7 5 6  "
                            "  5 7 12 "
                            "7    158 "
                            "    3 34 "  // <-- HERE
                            "   19 2 5"
                            "8 162 39 "
                            "9  543 1 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


START_TEST(test_e08_invalid_board_col_dupe)
{
    // LOCAL VARIABLES
    int exp_return = EINVAL;  // Expected return value for this test case
    char *exp_result = NULL;  // Expected results for this test case
    // The sudoku puzzle for this test case
    char test_input[81] = { " 683 9 7 "
                            " 42     1"
                            "1 7 5 6  "
                            "  5 7 12 "
                            "7    158 "  // <-- COL 7 HERE...
                            "    3 74 "
                            "   19 2 5"
                            "8 162 59 "  // <-- ...AND COL 7 HERE
                            "9  543 1 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


START_TEST(test_e09_invalid_board_grid_dupe)
{
    // LOCAL VARIABLES
    int exp_return = EINVAL;  // Expected return value for this test case
    char *exp_result = NULL;  // Expected results for this test case
    // The sudoku puzzle for this test case
    char test_input[81] = { " 5   3   "
                            "9 6    78"
                            "3   28  1"
                            "        7"
                            " 136 298 "
                            "8 2    1 "
                            "  91   6 "     // <-- COL 8 HERE...
                            "28       "
                            "7 52   96" };  // <-- ...AND COL 9 HERE

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


/**************************************************************************************************/
/************************************** BOUNDARY TEST CASES ***************************************/
/**************************************************************************************************/


START_TEST(test_b01_solved)
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
                            "426853791"
                            "713924856"
                            "961537284"
                            "287419635"
                            "345286179" };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


START_TEST(test_b02_almost_solved)
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


START_TEST(test_b03_almost_empty)
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


START_TEST(test_b04_empty)
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


START_TEST(test_s01_multiple_solutions)
{
    // LOCAL VARIABLES
    int exp_return = ENOERR;  // Expected return value for this test case
    char *exp_result = NULL;  // Expected results for this test case
    // The sudoku puzzle for this test case
    char test_input[81] = { "        5"
                            "5 93   2 "
                            "     739 "
                            "  325814 "
                            "6 5173  2"
                            "182 9  37"
                            "8 4    7 "
                            "2 7  5461"
                            "31 7   5 " };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


START_TEST(test_s02_array_too_long)
{
    // LOCAL VARIABLES
    int exp_return = ENOERR;  // Expected return value for this test case
    // Expected results for this test case
    char *exp_result = "534678912672195348198342567859761423426853791713924856961537284287419635345286179";
    // The sudoku puzzle for this test case
    char test_input[] = { "53467    "
                          "6  195   "
                          " 98    6 "
                          "8   6   3"
                          "4  8 3  1"
                          "7   2   6"
                          " 6    28 "
                          "   419  5"
                          "    8  79"
                          "abcdefghi" };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


START_TEST(test_s03_long_array_truncated)
{
    // LOCAL VARIABLES
    int exp_return = ENOERR;  // Expected return value for this test case
    // Expected results for this test case
    char *exp_result = "534678912672195348198342567859761423426853791713924856961537284287419635345286179";
    // The sudoku puzzle for this test case
    char test_input[] = { "53467    "
                          "6  195   "
                          " 98    6 "
                          "8   6   3"
                          "4  8 3  1"
                          "7   2   6"
                          " 6    28 "
                          "   419  5"
                          "    8  79"
                          "\0abcdefg" };

    // RUN TEST
    run_test_case(test_input, exp_result, exp_return);
}
END_TEST


/**************************************************************************************************/
/************************************* HELPER CODE DEFINITION *************************************/
/**************************************************************************************************/


Suite *create_test_suite(void)
{
    // LOCAL VARIABLES
    Suite *suite = suite_create("SUDO_Logic-Solve_Board");  // Test suite
    TCase *tc_normal = tcase_create("Normal");              // Normal test cases
    TCase *tc_error = tcase_create("Error");                // Error test cases
    TCase *tc_boundary = tcase_create("Boundary");          // Error test cases
    TCase *tc_special = tcase_create("Special");            // Special test cases

    // SETUP TEST CASES
    tcase_add_test(tc_normal, test_n01_easy_sudoku);
    tcase_add_test(tc_normal, test_n02_medium_sudoku);
    tcase_add_test(tc_normal, test_n03_hard_sudoku);
    tcase_add_test(tc_normal, test_n04_expert_sudoku);
    tcase_add_test(tc_normal, test_n05_master_sudoku);
    tcase_add_test(tc_normal, test_n06_extreme_sudoku);
    tcase_add_test(tc_normal, test_n07_easiest_puzzle_1);
    tcase_add_test(tc_normal, test_n08_easiest_puzzle_2);
    tcase_add_test(tc_normal, test_n09_intermediate_puzzle);
    tcase_add_test(tc_normal, test_n10_difficult_puzzle_1);
    tcase_add_test(tc_normal, test_n11_difficult_puzzle_2);
    tcase_add_test(tc_normal, test_n12_not_fun_puzzle);
    tcase_add_test(tc_error, test_e01_invalid_char_nul);
    tcase_add_test(tc_error, test_e02_invalid_char_zero);
    tcase_add_test(tc_error, test_e03_invalid_char_letter);
    tcase_add_test(tc_error, test_e04_invalid_char_tab);
    tcase_add_test(tc_error, test_e05_invalid_char_newline);
    tcase_add_test(tc_error, test_e06_bad_pointer);
    tcase_add_test(tc_error, test_e07_invalid_board_row_dupe);
    tcase_add_test(tc_error, test_e08_invalid_board_col_dupe);
    tcase_add_test(tc_error, test_e09_invalid_board_grid_dupe);
    tcase_add_test(tc_boundary, test_b01_solved);
    tcase_add_test(tc_boundary, test_b02_almost_solved);
    tcase_add_test(tc_boundary, test_b03_almost_empty);
    tcase_add_test(tc_boundary, test_b04_empty);
    tcase_add_test(tc_special, test_s01_multiple_solutions);
    tcase_add_test(tc_special, test_s02_array_too_long);
    tcase_add_test(tc_special, test_s03_long_array_truncated);
    suite_add_tcase(suite, tc_normal);
    suite_add_tcase(suite, tc_error);
    suite_add_tcase(suite, tc_boundary);
    suite_add_tcase(suite, tc_special);

    // DONE
    return suite;
}


void run_test_case(char test_input[81], char exp_solution[81], int exp_return)
{
    // LOCAL VARIABLES
    int actual_ret = CANARY_INT;  // Return value of the tested function

    // RUN IT
    // Call the function
    actual_ret = solve_board(test_input);
    // Compare actual return value to expected return value
    ck_assert_msg(exp_return == actual_ret, "solve_board() returned [%d] '%s' "
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
    char log_rel_path[] = { "./code/test/test_output/check_sudo_logic_solve_board.log" };
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
