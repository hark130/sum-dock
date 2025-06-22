/*
 *  This library defines game logic functionality to solve a game board.
 */

// #define SUDO_DEBUG                          // Enable DEBUG logging

#include <errno.h>                          // EINVAL, ENODATA
#include <stdbool.h>                        // bool, false, true
#include <stdio.h>                          // DEBUGGING
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
 *      miss_col: An array to fill with missing characters.
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
 *      miss_grid: An array to fill with missing characters.
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
 *      miss_row: An array to fill with missing characters.
 *      row: The row index.
 *
 *  Returns:
 *      ENOERR on success, errno value on error.
 */
int fill_missing_row(char board[9][9], char miss_row[9], int row);

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
    // PRINT_ERROR(HERE);  // DEBUGGING
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
        // FPRINTF_ERR("MISSING ROW CONTENTS BEFORE: %s\n", miss_row);  // DEBUGGING
        results = fill_missing_row(board, miss_row, row);
        // FPRINTF_ERR("MISSING ROW CONTENTS AFTER:  %s\n", miss_row);  // DEBUGGING
    }
    // Get missing col characters
    if (ENOERR == results)
    {
        // FPRINTF_ERR("MISSING COL %d CONTENTS BEFORE: %s\n", col, miss_col);  // DEBUGGING
        results = fill_missing_col(board, miss_col, col);
        // FPRINTF_ERR("MISSING COL %d CONTENTS AFTER:  %s\n", col, miss_col);  // DEBUGGING
    }
    // Get missing grid characters
    if (ENOERR == results)
    {
        // FPRINTF_ERR("MISSING ROW %d COL %d CONTENTS BEFORE: %s\n", row, col, miss_grd);  // DEBUGGING
        results = fill_missing_grid(board, miss_grd, row, col);
        // FPRINTF_ERR("MISSING ROW %d COL %d CONTENTS AFTER:  %s\n", row, col, miss_grd);  // DEBUGGING
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
                        // FPRINTF_ERR("FOUND A MATCH BETWEEN ROW %d AND COL %d: %c!\n", row, col, miss_grd[grid_index]);  // DEBUGGING
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
    /* DEBUGGING */
    // if (ENOERR == results)
    // {
    //     FPRINTF_ERR("MISSING ROW %d CONTENTS AFTER:  %s\n", row, miss_row);  // DEBUGGING
    //     FPRINTF_ERR("MISSING COL %d CONTENTS AFTER:  %s\n", col, miss_col);  // DEBUGGING
    //     FPRINTF_ERR("MISSING ROW %d COL %d CONTENTS AFTER:  %s\n", row, col, miss_grd);  // DEBUGGING
    // }
    /* DEBUGGING */
    return overlap;
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
        memset(miss_col, '\0', 9 * sizeof(char));
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
        // FPRINTF_ERR("ROW %d AND COL %d IS IN GRID %d\n", row, col, grid_num);  // DEBUGGING
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
        // FPRINTF_ERR("ROW %d COL %d is %c\n", row, col, board[row][col]);  // DEBUGGING
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
    // PRINT_ERROR(HERE);  // DEBUGGING
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            if (SUDO_EMPTY_GRID == board[row][col])
            {
                // FPRINTF_ERR("FOUND AN EMPTY GRID AT ROW %d COL %d\n", row, col);  // DEBUGGING
                results = ENODATA;  // There's an empty grid
                goto done;  // No need to search for anymore empty grids
            }
        }
    }

    // DONE
done:
    // FPRINTF_ERR("ABOUT TO RETURN [%d] '%s' from %s\n", results, strerror(results), __FUNCTION_NAME__);  // DEBUGGING
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
    // PRINT_ERROR(IS IT EMPTY?);  // DEBUGGING
    if (true == is_empty)
    {
        // PRINT_ERROR(IT IS EMPTY!);  // DEBUGGING
        match = check_for_match(board, row, col, &results);
        if ('\0' != match && ENOERR == results)
        {
            // FPRINTF_ERR("FOUND SOLUTION AT ROW %d COL %d: %c\n", row, col, match);  // DEBUGGING
            board[row][col] = match;
            /* DEBUGGING */
            // for (int row_index = 0; row_index < 9; row_index++)
            // {
            //     for (int col_index = 0; col_index < 9; col_index++)
            //     {
            //         if (0 == col_index)
            //         {
            //             printf("\n");
            //         }
            //         printf("%c", board[row_index][col_index]);
            //     }
            // }
            // printf("\n");
            /* DEBUGGING */
        }
    }
    else if (ENOERR == results)
    {
        results = ENODATA;  // No error and intersection wasn't empty
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
                // FPRINTF_ERR("EVALUATING ROW %d COL %d\n", row, col);  // DEBUGGING
                results = make_a_match(game, row, col);
                // FPRINTF_ERR("make_a_match() returned [%d] %s\n", results, strerror(results));  // DEBUGGING
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
