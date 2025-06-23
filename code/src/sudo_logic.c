/*
 *  This library defines game logic functionality to solve a game board.
 */

// #define SUDO_DEBUG                          // Enable DEBUG logging

#include <errno.h>                          // EINVAL, ENODATA
#include <stdbool.h>                        // bool, false, true
#include <string.h>                         // memset()
#include "sudo_debug.h"                     // MODULE_*LOAD(), PRINT_ERRNO(), PRINT_ERROR()
#include "sudo_macros.h"                    // ENOERR
#include "sudo_validation.h"                // validate_board()


MODULE_LOAD();  // Print the module name being loaded using the gcc constructor attribute
MODULE_UNLOAD();  // Print the module name being unloaded using the gcc destructor attribute


/**************************************************************************************************/
/********************************* PRIVATE FUNCTION DECLARATIONS **********************************/
/**************************************************************************************************/

/*
 *  Description:
 *      STRATEGY 1: If ~[ROW SET] && ~[COL SET] && ~[GRID SET] is one value?
 *      This function (almost) does *NOT* validate anything!
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      row: The row index.
 *      col: The column index.
 *      errnum: [Out] Provide feedback on execution.
 *
 *  Returns:
 *      The available play on success (errnum is set to ENOERR).  Nul character if no solution was
 *      found (errnum is set to ENODATA).  Nul character on error (errnum is set with errno).
 */
char check_for_match(char board[9][9], int row, int col, int *errnum);

/*
 *  Description:
 *      Count the number of non-nul characters in array.  Replaces strlen() for fixed-width arrays.
 *
 *  Args:
 *      array: A fixed-width array.
 *
 *  Returns:
 *      The number of characteres in array: a number between 0 and 9 (inclusive).
 *      0 if array is NULL.
 */
int count_array_entries(char array[9]);

/*
 *  Description:
 *      Determine the grid containing the intersection of row and col.
 *      1 2 3
 *      4 5 6
 *      7 8 9
 *
 *  Args:
 *      row: The row index.
 *      col: The column index.
 *      errnum: [Out] Provide feedback on execution.
 *
 *  Returns:
 *      A grid number 1-9 on success (errnum is set to ENOERR), 0 on error (errnum is set
 *      accordingly).
 */
int determine_grid(int row, int col, int *errnum);

/*
 *  Description:
 *      Fill the miss_col array with all the characters missing from board*[col].
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      miss_col: [Out] An array to fill with missing characters.
 *      col: The col index.
 *
 *  Returns:
 *      ENOERR on success, errno value on error.
 */
int fill_missing_col(char board[9][9], char miss_col[9], int col);

/*
 *  Description:
 *      Fill the miss_grid array with all the characters missing from the 3x3 grid containing
 *      board[row][col].
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      miss_grid: [Out] An array to fill with missing characters.
 *      row: The row index.
 *      col: The col index.
 *
 *  Returns:
 *      ENOERR on success, errno value on error.
 */
int fill_missing_grid(char board[9][9], char miss_grid[9], int row, int col);

/*
 *  Description:
 *      Fill the miss_row array with all the characters missing from board[row]*.
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      miss_row: [Out] An array to fill with missing characters.
 *      row: The row index.
 *
 *  Returns:
 *      ENOERR on success, errno value on error.
 */
int fill_missing_row(char board[9][9], char miss_row[9], int row);

/*
 *  Description:
 *      Fill col_entries with all of the board values from the col_index, skipping
 *      SUDO_EMPTY_GRIDs.  The col_entries array will be zeroized first.
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      col_entries: [Out] An array to fill with the col characters.
 *      col_index: The col index to fetch entries from.
 *
 *  Returns:
 *      ENOERR on success, errno value on error.
 */
int get_col_entries(char board[9][9], char col_entries[9], int col_index);

/*
 *  Description:
 *      Fill grid_entries with all of the board values from grid number grid_num, skipping
 *      SUDO_EMPTY_GRIDs.  The grid_entries array will be zeroized first.
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      grid_entries: [Out] An array to fill with found characters.
 *      grid_num: The grid to fetch entries from.
 *
 *  Returns:
 *      ENOERR on success, errno value on error.
 */
int get_grid_entries(char board[9][9], char grid_entries[9], int grid_num);

/*
 *  Description:
 *      Fill intersection with all of the board values from line1 that are also in line2,
 *      skipping SUDO_EMPTY_GRIDs.  The intersection array will be zeroized first.
 *
 *  Args:
 *      line1: The first line to consider.
 *      line2: The second line to consider.
 *      intersection: [Out] An array to fill with characters in both line1 and line2.
 *
 *  Returns:
 *      ENOERR on success, errno value on error.
 */
int get_line_intersection(char line1[9], char line2[9], char intersection[9]);

/*
 *  Description:
 *      Fill row_entries with all of the board values from the row_index, skipping SUDO_EMPTY_GRIDs.
 *      The row_entries array will be zeroized first.
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      row_entries: [Out] An array to fill with the row characters.
 *      row_index: The row index to fetch entries from.
 *
 *  Returns:
 *      ENOERR on success, errno value on error.
 */
int get_row_entries(char board[9][9], char row_entries[9], int row_index);

/*
 *  Description:
 *      Determine if the intersection of row and col is empty.  This function does not validate
 *      the game board but does validate row and col.
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      row: The row index.
 *      col: The column index.
 *      errnum: [Out] Provide feedback on execution.
 *
 *  Returns:
 *      Returns true if the intersection is empty, false otherwise.  On error, returns false and
 *      sets errnum with an errno value.
 */
bool is_empty_intersection(char board[9][9], int row, int col, int *errnum);

/*
 *  Description:
 *      Is the game really over?  This function does not validate the game board.
 *
 *  Args:
 *      board: A two-dimenstional array.
 *
 *  Returns:
 *      ENOERR on game over, ENODATA for an unsolved game board, or errno on error.
 */
int is_game_really_over(char board[9][9]);

/*
 *  Description:
 *      STRATEGY 2: If the intersection of [LINE1 SET] and [LINE2 SET] contains one value and
 *      there is only one available play left in the grid not covered by LINE1 or LINE2, solve it.
 *      LINE1 and LINE2 must cross the grid in question.  The "available play" can not be
 *      in LINE1 or LINE2 (of course).
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      grid_num: The grid number to investigate: 1-9.
 *
 *  Returns:
 *      ENOERR for a match, ENODATA if no match was made, or errno on error.
 */
int make_a_grid_match(char board[9][9], int grid_num);

/*
 *  Description:
 *      STRATEGY 2: Check all of the colss passing through the grid.
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      grid_num: The grid number to investigate: 1-9.
 *
 *  Returns:
 *      ENOERR for a match, ENODATA if no match was made, or errno on error.
 */
int make_a_grid_match_cols(char board[9][9], int grid_num);

/*
 *  Description:
 *      STRATEGY 2: Check all of the perpendicular lines passing through the grid.
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      grid_num: The grid number to investigate: 1-9.
 *
 *  Returns:
 *      ENOERR for a match, ENODATA if no match was made, or errno on error.
 */
int make_a_grid_match_perp(char board[9][9], int grid_num);

/*
 *  Description:
 *      STRATEGY 2: Check all of the rows passing through the grid.
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      grid_num: The grid number to investigate: 1-9.
 *
 *  Returns:
 *      ENOERR for a match, ENODATA if no match was made, or errno on error.
 */
int make_a_grid_match_rows(char board[9][9], int grid_num);

/*
 *  Description:
 *      STRATEGY 1: If ~[ROW SET] && ~[COL SET] && ~[GRID SET] is one value, solve it.
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      row: The row index.
 *      col: The column index.
 *      errnum: [Out] Provide feedback on execution.
 *
 *  Returns:
 *      ENOERR for a match, ENODATA if no match was made, or errno on error.
 */
int make_a_match(char board[9][9], int row, int col);

/*
 *  Description:
 *      Fill the last empty spot in the column.
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      col_index: The column index to fill.
 *
 *  Returns:
 *      ENOERR on success, ENODATA for an unsolved col (e.g., it was full, it had more than one
 *      empty slot), errno value on error.
 */
int solve_final_col(char board[9][9], int col_index);

/*
 *  Description:
 *      Fill the last empty spot in the grid.
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      grid_num: The grid to fetch entries from.
 *
 *  Returns:
 *      ENOERR on success, ENODATA for an unsolved grid (e.g., it was full, it had more than one
 *      empty slot), errno value on error.
 */
int solve_final_grid(char board[9][9], int grid_num);

/*
 *  Description:
 *      Fill the last empty spot in the row.
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      row_index: The row index to fill.
 *
 *  Returns:
 *      ENOERR on success, ENODATA for an unsolved row (e.g., it was full, it had more than one
 *      empty slot), errno value on error.
 */
int solve_final_row(char board[9][9], int row_index);

/*
 *  Description:
 *      Grid number grid_num has one empty spot in row_index.  Put number there.
 *      This function does not validate arg values but does validate the status of the grid.
 *
 *  Args:
 *      board: A two-dimenstional array.
 *      grid_num: The grid to fill.
 *      row_index: The row index of grid_num to fill.
 *      number: The value to fill it with.
 *
 *  Returns:
 *      ENOERR on success, ENODATA for an unsolved grid (e.g., it was full, it had more than one
 *      empty slot), errno value on error.
 */
int solve_final_grid_row(char board[9][9], int grid_num, int row_index, char number);

/*
 *  Description:
 *      Use strategy one to solve the board.  This function does not validate the game board.
 *
 *  Notes:
 *      For each row and column that isn't solved at the intersection...
 *      If ~[ROW SET] && ~[COL SET] && ~[GRID SET] is one value, solve it.  Declare win or continue.
 *      If the loop ends, check for a win and return accordingly.
 *
 *  Args:
 *      board: A fixed-size array of 81 characters.  Each character must be a SUDO_EMPTY_GRID
 *          or number ranging from 1-9, inclusive.
 *
 *  Returns:
 *      ENOERR on success, ENODATA for an unsolved game board, or errno on error.
 */
int solve_strategy_one(char board[81]);

/*
 *  Description:
 *      Use strategy two to solve the board.  This function does not validate the game board.
 *
 *  Notes:
 *      Process of elimination...
 *      If Line* 1 and Line 2 cross a grid, and both Lines contain a number not already in
 *      the grid, *and* there's only one empty spot left in Line 3, solve it.
 *      If the loop ends, check for a win and return accordingly.
 *
 *  Args:
 *      board: A fixed-size array of 81 characters.  Each character must be a SUDO_EMPTY_GRID
 *          or number ranging from 1-9, inclusive.
 *
 *  Returns:
 *      ENOERR on success, ENODATA for an unsolved game board, or errno on error.
 */
int solve_strategy_two(char board[81]);

/*
 *  Description:
 *      Swap the integer values.
 *
 *  Args:
 *      num1: Pointer to number one.
 *      num2: Pointer to number two.
 */
void swap_ints(int *num1, int *num2);

/*
 *  Description:
 *      Validate row and col arguments for this module.
 *
 *  Args:
 *      row: The row index.
 *      col: The column index.
 *
 *  Returns:
 *      ENOERR on success, ERANGE on error.
 */
int validate_row_and_col(int row, int col);

/**************************************************************************************************/
/********************************** PUBLIC FUNCTION DEFINITIONS ***********************************/
/**************************************************************************************************/


int is_game_over(char board[81])
{
    // LOCAL VARIABLES
    int results = validate_board(board);   // Input validation
    char (*game)[9] = (char (*)[9])board;  // Cast it to a two-dimensional array

    // IS IT OVER?
    if (ENOERR == results)
    {
        results = is_game_really_over(game);
    }

    // DONE
    return results;
}


int solve_board(char board[81])
{
    // LOCAL VARIABLES
    int results = ENOERR;  // Results of execution

    // INPUT VALIDATION
    results = is_game_over(board);  // Full validation
    // Respond to results
    if (ENOERR == results)
    {
        goto done;  // Game is already solved
    }
    else if (ENODATA == results)
    {
        results = ENOERR;  // Not solved but the board is valid so let's continue
    }

    // SOLVE IT
    if (ENOERR == results)
    {
        // Strategy #1
        results = solve_strategy_one(board);
        if (ENODATA == results)
        {
            PRINT_ERROR(Strategy number one failed to solve the game);
            // Strategy #2
            results = solve_strategy_two(board);
            if (ENODATA == results)
            {
                PRINT_ERROR(Strategy number two failed to solve the game);
            }
        }
    }

    // VERIFY
    if (ENOERR == results)
    {
        results = is_game_over(board);  // Full validation
        if (ENOERR != results)
        {
            PRINT_ERROR(Game logic reported success but the game board failed validation);
        }
    }

    // DONE
done:
    return results;
}


/**************************************************************************************************/
/********************************** PRIVATE FUNCTION DEFINITIONS **********************************/
/**************************************************************************************************/


char check_for_match(char board[9][9], int row, int col, int *errnum)
{
    // LOCAL VARIABLES
    int results = ENOERR;         // Results of execution
    char miss_row[9] = { '\0' };  // Characters missing from the row
    char miss_col[9] = { '\0' };  // Characters missing from the col
    char miss_grd[9] = { '\0' };  // Characters missing from the grid
    char overlap = '\0';          // Overlap character between all three arrays

    // INPUT VALIDATION
    if (NULL == board)
    {
        results = EINVAL;  // We shall not abide NULL pointers
    }

    // CHECK IT
    // Get missing row characters
    if (ENOERR == results)
    {
        results = fill_missing_row(board, miss_row, row);
    }
    // Get missing col characters
    if (ENOERR == results)
    {
        results = fill_missing_col(board, miss_col, col);
    }
    // Get missing grid characters
    if (ENOERR == results)
    {
        results = fill_missing_grid(board, miss_grd, row, col);
    }
    // Find overlap
    if (ENOERR == results)
    {
        results = ENODATA;  // Now using this variable to keep track of multiple solutions
        for (int row_index = 0; row_index < 9; row_index++)
        {
            if ('\0' == miss_row[row_index])
            {
                break;  // End of line
            }
            for (int col_index = 0; col_index < 9; col_index++)
            {
                if ('\0' == miss_col[col_index])
                {
                    break;  // End of line
                }
                else if (miss_row[row_index] != miss_col[col_index])
                {
                    continue;  // Not a match... keep looking
                }
                for (int grid_index = 0; grid_index < 9; grid_index++)
                {
                    if ('\0' == miss_grd[grid_index])
                    {
                        break;  // End of line
                    }
                    else if (miss_col[col_index] != miss_grd[grid_index])
                    {
                        continue;  // Not a match... keep looking
                    }
                    else
                    {
                        if (ENOERR == results)
                        {
                            results = ENODATA;  // A second solution was found so...
                            overlap = '\0';     // ...this is not the answer.
                            goto done;          // Time to stop looking.
                        }
                        else
                        {
                            overlap = miss_grd[grid_index];  // Found one!
                            results = ENOERR;  // Success
                        }
                    }
                }
            }
        }
    }

    // DONE
done:
    if (NULL != errnum)
    {
        *errnum = results;
    }
    return overlap;
}


int count_array_entries(char array[9])
{
    // LOCAL VARIABLES
    int count = 0;  // Number of non-nul characters in array

    // INPUT VALIDATION
    if (NULL != array)
    {
        for (int i = 0; i < 9; i++)
        {
            if ('\0' != array[i])
            {
                count++;
            }
        }
    }

    // DONE
    return count;
}


int determine_grid(int row, int col, int *errnum)
{
    // LOCAL VARIABLES
    int results = ENOERR;  // Results of execution
    int grid_num = 0;      // 1-9 on success

    // INPUT VALIDATION
    results = validate_row_and_col(row, col);

    // DETERMINE IT
    if (ENOERR == results)
    {
        switch (row)
        {
            case 0:
            case 1:
            case 2:
                grid_num = (col + 3) / 3;
                break;
            case 3:
            case 4:
            case 5:
                grid_num = ((col + 3) / 3) + 3;
                break;
            case 6:
            case 7:
            case 8:
                grid_num = ((col + 3) / 3) + 6;
                break;
            default:
                results = EINVAL;  // How did we get here?!
        }
    }

    // DONE
    if (NULL != errnum)
    {
        *errnum = results;
    }
    return grid_num;
}


int fill_missing_col(char board[9][9], char miss_col[9], int col)
{
    // LOCAL VARIABLES
    int results = ENOERR;           // Results of execution
    int found[10] = { 0 };          // Keep track of chars found by index (index 1 represents '1')
    char *tmp_miss_col = miss_col;  // Iterating pointer into miss_col

    // INPUT VALIDATION
    if (NULL == board || NULL == miss_col)
    {
        results = EINVAL;  // We shall not abide NULL pointers
    }

    // SETUP
    if (ENOERR == results)
    {
        memset(miss_col, '\0', 9 * sizeof(miss_col[0]));
    }

    // FILL IT
    if (ENOERR == results)
    {
        for (int i = 0; i < 9; i++)
        {
            if (SUDO_EMPTY_GRID != board[i][col])
            {
                found[board[i][col] - '0'] = 1;  // Found one
            }
        }
        for (int i = 1; i <= 9; i++)
        {
            if (0 == found[i])
            {
                *tmp_miss_col = i + '0';
                tmp_miss_col++;
            }
        }
    }

    // DONE
    return results;
}


int fill_missing_grid(char board[9][9], char miss_grid[9], int row, int col)
{
    // LOCAL VARIABLES
    int results = ENOERR;             // Results of execution
    int found[10] = { 0 };            // Keep track of chars found by index (index 1 represents '1')
    char *tmp_miss_grid = miss_grid;  // Iterating pointer into miss_col
    int grid_num = 0;                 // Grid number containing row and col
    int start_row = 0;                // Iterate over the grid's rows
    int start_col = 0;                // Iterate over the grid's cols

    // INPUT VALIDATION
    if (NULL == board || NULL == miss_grid)
    {
        results = EINVAL;  // We shall not abide NULL pointers
    }

    // FILL IT
    // Determine which grid row and col is in
    if (ENOERR == results)
    {
        grid_num = determine_grid(row, col, &results);
    }
    // Get the values from that grid
    if (ENOERR == results)
    {
        start_row = 3 * ((grid_num - 1) / 3);
        start_col = 3 * ((grid_num - 1) % 3);
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (SUDO_EMPTY_GRID != board[start_row + i][start_col + j])
                {
                    found[board[start_row + i][start_col + j] - '0'] = 1;  // Found one
                }
            }
        }
        for (int i = 1; i <= 9; i++)
        {
            if (0 == found[i])
            {
                *tmp_miss_grid = i + '0';
                tmp_miss_grid++;
            }
        }
    }

    // DONE
    return results;
}


int fill_missing_row(char board[9][9], char miss_row[9], int row)
{
    // LOCAL VARIABLES
    int results = ENOERR;           // Results of execution
    int found[10] = { 0 };          // Keep track of chars found by index (index 1 represents '1')
    char *tmp_miss_row = miss_row;  // Iterating pointer into miss_row

    // INPUT VALIDATION
    if (NULL == board || NULL == miss_row)
    {
        results = EINVAL;  // We shall not abide NULL pointers
    }

    // SETUP
    if (ENOERR == results)
    {
        memset(miss_row, '\0', 9 * sizeof(char));
    }

    // FILL IT
    if (ENOERR == results)
    {
        for (int i = 0; i < 9; i++)
        {
            if (SUDO_EMPTY_GRID != board[row][i])
            {
                found[board[row][i] - '0'] = 1;  // Found one
            }
        }
        for (int i = 1; i <= 9; i++)
        {
            if (0 == found[i])
            {
                *tmp_miss_row = i + '0';
                tmp_miss_row++;
            }
        }
    }

    // DONE
    return results;
}


int get_col_entries(char board[9][9], char col_entries[9], int col_index)
{
    // LOCAL VARIABLES
    int results = ENOERR;         // Results of execution
    char *col_ptr = col_entries;  // Iterating pointer into gride_entries

    // INPUT VALIDATION
    if (NULL == board || NULL == col_entries || col_index < 0 || col_index > 8)
    {
        results = EINVAL;
    }
    else
    {
        memset(col_entries, '\0', 9 * sizeof(col_entries[0]));
    }

    // GET IT
    if (ENOERR == results)
    {
        for (int row_index = 0; row_index < 9; row_index++)
        {
            if (SUDO_EMPTY_GRID != board[row_index][col_index])
            {
                *col_ptr = board[row_index][col_index];
                col_ptr++;
            }
        }
    }

    // DONE
    return results;
}


int get_grid_entries(char board[9][9], char grid_entries[9], int grid_num)
{
    // LOCAL VARIABLES
    int results = ENOERR;           // Results of execution
    int start_row = 0;              // Iterate over the grid's rows
    int start_col = 0;              // Iterate over the grid's cols
    char *grid_ptr = grid_entries;  // Iterating pointer into gride_entries

    // INPUT VALIDATION
    if (NULL == board || NULL == grid_entries || grid_num < 1 || grid_num > 9)
    {
        results = EINVAL;
    }
    else
    {
        memset(grid_entries, '\0', 9 * sizeof(grid_entries[0]));
    }

    // GET IT
    if (ENOERR == results)
    {
        start_row = 3 * ((grid_num - 1) / 3);
        start_col = 3 * ((grid_num - 1) % 3);
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (SUDO_EMPTY_GRID != board[start_row + i][start_col + j])
                {
                    *grid_ptr = board[start_row + i][start_col + j];  // Found one
                    grid_ptr++;
                }
            }
        }
    }

    // DONE
    return results;
}


int get_line_intersection(char line1[9], char line2[9], char intersection[9])
{
    // LOCAL VARIABLES
    int results = ENOERR;            // Results of execution
    char *inter_ptr = intersection;  // Iterating variable into intersection

    // INPUT VALIDATION
    if (NULL == line1 || NULL == line2 || NULL == intersection)
    {
        results = EINVAL;
    }
    // SETUP
    else
    {
        memset(intersection, '\0', 9 * sizeof(intersection[0]));
    }

    // GET IT
    if (ENOERR == results)
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (line1[i] == line2[j])
                {
                    *inter_ptr = line1[i];
                    inter_ptr++;
                    break;  // Found a match, stop looking.
                }
            }
        }
    }

    // DONE
    return results;
}


int get_row_entries(char board[9][9], char row_entries[9], int row_index)
{
    // LOCAL VARIABLES
    int results = ENOERR;         // Results of execution
    char *row_ptr = row_entries;  // Iterating pointer into gride_entries

    // INPUT VALIDATION
    if (NULL == board || NULL == row_entries || row_index < 0 || row_index > 8)
    {
        results = EINVAL;
    }
    // SETUP
    else
    {
        memset(row_entries, '\0', 9 * sizeof(row_entries[0]));
    }

    // GET IT
    if (ENOERR == results)
    {
        for (int col_index = 0; col_index < 9; col_index++)
        {
            if (SUDO_EMPTY_GRID != board[row_index][col_index])
            {
                *row_ptr = board[row_index][col_index];
                row_ptr++;
            }
        }
    }

    // DONE
    return results;
}


bool is_empty_intersection(char board[9][9], int row, int col, int *errnum)
{
    // LOCAL VARIABLES
    int results = ENOERR;   // Results of execution
    bool is_empty = false;  // Is row && col empty?

    // INPUT VALIDATION
    results = validate_row_and_col(row, col);

    // IS IT EMPTY?
    if (ENOERR == results)
    {
        if (SUDO_EMPTY_GRID == board[row][col])
        {
            is_empty = true;
        }
    }

    // DONE
    if (NULL != errnum)
    {
        *errnum = results;
    }
    return is_empty;
}


int is_game_really_over(char board[9][9])
{
    // LOCAL VARIABLES
    int results = ENOERR;  // Results of execution

    // IS IT REALLY OVER?
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            if (SUDO_EMPTY_GRID == board[row][col])
            {
                results = ENODATA;  // There's an empty grid
                goto done;  // No need to search for anymore empty grids
            }
        }
    }

    // DONE
done:
    return results;
}


int make_a_grid_match(char board[9][9], int grid_num)
{
    // LOCAL VARIABLES
    int results = ENOERR;                  // Results of execution
    char grd_entries[9] = { '\0' };        // Characters in the grid
    // char line1_entries[9] = { '\0' };      // Characters in LINE1
    // char line2_entries[9] = { '\0' };      // Characters in LINE2
    // char line_intersection[9] = { '\0' };  // The interesection of LINE1 and LINE2

    // VALIDATION
    // Determine current entries
    results = get_grid_entries(board, grd_entries, grid_num);
    if (ENOERR != results)
    {
        PRINT_ERROR(The get_grid_entries functions detected and error);
        PRINT_ERRNO(results);
    }
    // Is there room?
    else
    {
        if (9 == count_array_entries(grd_entries))
        {
            results = ENODATA;  // Grid is full.  Move on.
        }
        else if (8 == count_array_entries(grd_entries))
        {
            results = solve_final_grid(board, grid_num);
        }
    }

    // MAKE A MATCH
    if (ENOERR == results)
    {
        // Grid rows as lines
        results = make_a_grid_match_rows(board, grid_num);
        // if (ENODATA == results)
        // {
        //     // Grid columns as lines
        //     results = make_a_grid_match_cols(board, grid_num);
        //     if (ENODATA == results)
        //     {
        //         // Perpendicular lines
        //         results = make_a_grid_match_perp(board, grid_num);
        //     }
        // }
    }

    // DONE
    return results;
}


int make_a_grid_match_cols(char board[9][9], int grid_num)
{
    return ENOSYS;  // TO DO: DON'T DO NOW
}


int make_a_grid_match_perp(char board[9][9], int grid_num)
{
    return ENOSYS;  // TO DO: DON'T DO NOW
}


int make_a_grid_match_rows(char board[9][9], int grid_num)
{
    // LOCAL VARIABLES
    int results = ENOERR;                   // Results of execution
    char grd_entries[9] = { '\0' };         // Characters in the grid
    char line1_entries[9] = { '\0' };       // Characters in LINE1
    char line2_entries[9] = { '\0' };       // Characters in LINE2
    char line3_entries[9] = { '\0' };       // Characters in LINE2
    char lines_intersection[9] = { '\0' };  // The interesection of LINE1 and LINE2
    char grid_line3[9] = { '\0' };          // The intersection of LINE3 and the grid
    int line1_index = 0;                    // Temp variable to iterate over all line combos
    int line2_index = 0;                    // Temp variable to iterate over all line combos
    int line3_index = 0;                    // Temp variable to iterate over all line combos

    // INPUT VALIDATION
    if (NULL == board || grid_num < 1 || grid_num > 9)
    {
        results = EINVAL;
    }
    // SETUP
    else
    {
        results = get_grid_entries(board, grd_entries, grid_num);
    }

    // CHECK THE ROWS
    if (ENOERR == results)
    {
        results = ENODATA;  // Prove this wrong
        line1_index = 3 * ((grid_num - 1) / 3);
        line2_index = line1_index + 1;
        line3_index = line2_index + 1;
        for (int i = 0; i < 3; i++)  // Iterate over all combinations of LINE1/2/3
        {
            get_row_entries(board, line3_entries, line3_index);  // Best effort
            get_line_intersection(line3_entries, grd_entries, grid_line3);  // L3 & the grid
            if (2 == count_array_entries(grid_line3))  // Won't work otherwise
            {
                get_row_entries(board, line1_entries, line1_index);  // Best effort
                get_row_entries(board, line2_entries, line2_index);  // Best effort
                get_line_intersection(line1_entries, line2_entries, lines_intersection);  // L1 & L2
                if (count_array_entries(lines_intersection) > 0)
                {
                    for (int j = 0; j < count_array_entries(lines_intersection); j++)
                    {
                        results = ENOERR;  // Reset the variable
                        for (int k = 0; k < count_array_entries(grd_entries); k++)
                        {
                            if (lines_intersection[j] == grd_entries[k])
                            {
                                results = ENODATA;  // Not it.
                                break;  // LINE1, LINE2, and the grid all have the value.
                            }
                        }
                        // Respond to results
                        if (ENOERR == results)
                        {
                            FPRINTF_ERR("THIS IS IT!  PLACE A '%c' ON LINE INDEX %d IN GRID %d!\n", lines_intersection[j], line3_index, grid_num);  // DEBUGGING
                            results = solve_final_grid_row(board, grid_num, line3_index,
                                                           lines_intersection[j]);
                            goto done;  // Found it, solve it, get out
                        }
                    }
                }
            }
            // Advance the line indices (AKA rotate them down once)
            swap_ints(&line1_index, &line2_index);  // Line1 is two and Line2 is one
            swap_ints(&line1_index, &line3_index);  // Line3 is two and Line1 is three
        }
    }

    // DONE
done:
    return results;
}


int make_a_match(char board[9][9], int row, int col)
{
    // LOCAL VARIABLES
    int results = ENOERR;   // Results of execution
    bool is_empty = false;  // Is the intersection of row and col empty
    char match = '\0';      // Match to be made

    // VALIDATION
    is_empty = is_empty_intersection(board, row, col, &results);
    if (true == is_empty)
    {
        match = check_for_match(board, row, col, &results);
        if ('\0' != match && ENOERR == results)
        {
            board[row][col] = match;
        }
    }
    else if (ENOERR == results)
    {
        results = ENODATA;  // No error and intersection wasn't empty
    }

    // DONE
    return results;
}


int solve_final_col(char board[9][9], int col_index)
{
    // LOCAL VARIABLES
    int results = ENODATA;       // Results of execution
    char entries[9] = { '\0' };  // Found entries
    bool found_it = false;       // Found an entry
    char missing = '\0';         // The character missing from the column
    char valid_entries[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };

    // INPUT VALIDATION
    if (NULL == board || col_index < 0 || col_index > 8)
    {
        results = EINVAL;
    }

    // SOLVE IT
    // What is there?
    if (ENOERR == results)
    {
        results = get_col_entries(board, entries, col_index);
    }
    // Is there one left?
    if (ENOERR == results)
    {
        if (8 != count_array_entries(entries))
        {
            results = ENODATA;  // There should only be one spot left for this to work
        }
    }
    // What is missing?
    if (ENOERR == results)
    {
        results = ENODATA;  // Prove this wrong
        for (int i = 0; i < (sizeof(valid_entries) / sizeof(valid_entries[0])); i++)
        {
            found_it = false;  // Reset the temp variable
            for (int j = 0; j < 8; j++)
            {
                if (valid_entries[i] == entries[j])
                {
                    found_it = true;
                    break;  // Valid entry matches an actual entry.  Keep looking.
                }
            }
            if (false == found_it)
            {
                missing = valid_entries[i];  // This is the one
                results = ENOERR;
                break;
            }
        }
    }
    // Fill it
    if (ENOERR == results)
    {
        for (int row_index = 0; row_index < 9; row_index++)
        {
            if (SUDO_EMPTY_GRID == board[row_index][col_index])
            {
                board[row_index][col_index] = missing;
                break;
            }
        }
    }

    // DONE
    return results;
}


int solve_final_grid(char board[9][9], int grid_num)
{
    return ENOSYS;  // TO DO: DON'T DO NOW...
}


int solve_final_grid_row(char board[9][9], int grid_num, int row_index, char number)
{
    // LOCAL VARIABLES
    int results = ENODATA;            // Results of execution
    char grid_entries[9] = { '\0' };  // Entries from grid_num
    char row_entries[9] = { '\0' };   // Entries from row_index
    char intersection[9] = { '\0' };  // The intersection of grid_entries and row_entries
    int start_col = 0;                // Iterate over the cols to fill in number

    // SOLVE IT
    get_grid_entries(board, grid_entries, grid_num);
    get_row_entries(board, row_entries, row_index);
    get_line_intersection(grid_entries, row_entries, intersection);
    if (2 == count_array_entries(intersection))
    {
        start_col = 3 * ((grid_num - 1) % 3);
        for (int col_index = start_col; col_index < (start_col + 3); col_index++)
        {
            if (SUDO_EMPTY_GRID == board[row_index][col_index])
            {
                board[row_index][col_index] = number;
                results = ENOERR;  // Filled an empty spot
                break;  // Stop looking
            }
        }
    }

    // DONE
    return results;
}


int solve_final_row(char board[9][9], int row_index)
{
    // LOCAL VARIABLES
    int results = ENODATA;       // Results of execution
    char entries[9] = { '\0' };  // Found entries
    bool found_it = false;       // Found an entry
    char missing = '\0';         // The character missing from the column
    char valid_entries[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };

    // INPUT VALIDATION
    if (NULL == board || row_index < 0 || row_index > 8)
    {
        results = EINVAL;
    }

    // SOLVE IT
    // What is there?
    if (ENOERR == results)
    {
        results = get_col_entries(board, entries, row_index);
    }
    // Is there one left?
    if (ENOERR == results)
    {
        if (8 != count_array_entries(entries))
        {
            results = ENODATA;  // There should only be one spot left for this to work
        }
    }
    // What is missing?
    if (ENOERR == results)
    {
        results = ENODATA;  // Prove this wrong
        for (int i = 0; i < (sizeof(valid_entries) / sizeof(valid_entries[0])); i++)
        {
            found_it = false;  // Reset the temp variable
            for (int j = 0; j < 8; j++)
            {
                if (valid_entries[i] == entries[j])
                {
                    found_it = true;
                    break;  // Valid entry matches an actual entry.  Keep looking.
                }
            }
            if (false == found_it)
            {
                missing = valid_entries[i];  // This is the one
                results = ENOERR;
                break;
            }
        }
    }
    // Fill it
    if (ENOERR == results)
    {
        for (int col_index = 0; col_index < 9; col_index++)
        {
            if (SUDO_EMPTY_GRID == board[row_index][col_index])
            {
                board[row_index][col_index] = missing;
                break;
            }
        }
    }

    // DONE
    return results;
}


int solve_strategy_one(char board[81])
{
    // LOCAL VARIABLES
    int results = ENODATA;                 // Results of execution
    char (*game)[9] = (char (*)[9])board;  // Cast it to a two-dimensional array

    // SOLVE IT
    do
    {
        for (int row = 0; row < 9; row++)
        {
            for (int col = 0; col < 9; col++)
            {
                results = make_a_match(game, row, col);
                if (ENODATA != results)
                {
                    break;  // Break out of the for loops
                }
            }
            if (ENODATA != results)
            {
                break;  // Break out of the for loops
            }
        }
        // Respond to results
        if (ENOERR == results)  // Made a play
        {
            results = is_game_really_over(game);  // Is it over yet?
            if (ENODATA == results)
            {
                continue;  // Game's not over yet
            }
            else if (ENOERR == results)
            {
                break;  // Game over man!
            }
            else
            {
                PRINT_ERROR(is_game_really_over identified an error);
                break;  // Encountered an error
            }
        }
        else if (ENODATA == results)  // Made a full loop without a play
        {
            PRINT_ERROR(Strategy one failed to make a play);
            break;  // No plays to make
        }
        else if (ENOERR != results)
        {
            PRINT_ERROR(Encountered an error in strategy one);
            break;  // Encountered an error
        }
    }
    while (1);

    // DONE
    return results;
}


int solve_strategy_two(char board[81])
{
    // LOCAL VARIABLES
    int results = ENODATA;                 // Results of execution
    char (*game)[9] = (char (*)[9])board;  // Cast it to a two-dimensional array

    // SOLVE IT
    do
    {
        for (int grid_num = 1; grid_num <= 9; grid_num++)
        {
            results = make_a_grid_match(game, grid_num);
        }
        // Respond to results
        if (ENOERR == results)  // Made a play
        {
            results = is_game_really_over(game);  // Is it over yet?
            if (ENODATA == results)
            {
                continue;  // Game's not over yet
            }
            else if (ENOERR == results)
            {
                break;  // Game over man!
            }
            else
            {
                PRINT_ERROR(is_game_really_over identified an error);
                break;  // Encountered an error
            }
        }
        else if (ENODATA == results)  // Made a full loop without a play
        {
            PRINT_ERROR(Strategy two failed to make a play);
            break;  // No plays to make
        }
        else if (ENOERR != results)
        {
            PRINT_ERROR(Encountered an error in strategy one);
            break;  // Encountered an error
        }
    }
    while (1);

    // DONE
    return results;
}


void swap_ints(int *num1, int *num2)
{
    if (NULL != num1 && NULL != num2)
    {
        *num1 = *num1 ^ *num2;
        *num2 = *num1 ^ *num2;
        *num1 = *num1 ^ *num2;
    }
}


int validate_row_and_col(int row, int col)
{
    // LOCAL VARIABLES
    int results = ENOERR;   // Results of execution

    // INPUT VALIDATION
    // row
    if (row > 8 || row < 0)
    {
        results = ERANGE;  // Row is out of range
        FPRINTF_ERR("%s The row %d is out of index\n", DEBUG_ERROR_STR, row);
    }
    // col
    if (col > 8 || col < 0)
    {
        results = ERANGE;  // Column is out of range
        FPRINTF_ERR("%s The column %d is out of index\n", DEBUG_ERROR_STR, col);
    }

    // DONE
    return results;
}
