/*
 *  This library defines functionality to validate arguments on behalf of SUDO.
 */

// #define SUDO_DEBUG                          // Enable DEBUG logging

#include <errno.h>                          // EINVAL
#include <stddef.h>                         // size_t
#include <stdio.h>                          // fprintf()
#include <string.h>                         // strlen()
#include "sudo_debug.h"                     // MODULE_*LOAD(), PRINT_ERRNO(), PRINT_ERROR()
#include "sudo_macros.h"                    // ENOERR, SUDO_BOARD_LEN, SUDO_EMPTY_GRID

MODULE_LOAD();  // Print the module name being loaded using the gcc constructor attribute
MODULE_UNLOAD();  // Print the module name being unloaded using the gcc destructor attribute


/**************************************************************************************************/
/********************************* PRIVATE FUNCTION DECLARATIONS **********************************/
/**************************************************************************************************/

/*
 *  Description:
 *      Validate an array holding the contents of a board element: row, col, grid.
 *
 *  Args:
 *      array: A fixed-size char array of 9 characters.  Each character must be a SUDO_EMPTY_GRID,
 *          or number ranging from 1-9, inclusive and the numbers may not be duplicated.
 *
 *  Returns:
 *      ENOERR on successful validation, EINVAL otherwise.
 */
int validate_board_array(const char array[9]);

/*
 *  Description:
 *      Validate a column.
 *
 *  Args:
 *      col: A fixed-size char array of 9 characters.  Each character must be a SUDO_EMPTY_GRID,
 *          or number ranging from 1-9, inclusive and the numbers may not be duplicated.
 *
 *  Returns:
 *      ENOERR on successful validation, EINVAL otherwise.
 */
int validate_board_col(const char col[9]);

/*
 *  Description:
 *      Validate board columns.
 *
 *  Args:
 *      board: A fixed-size char array of 81 characters.  Each character must be a SUDO_EMPTY_GRID,
 *          or number ranging from 1-9, inclusive.
 *
 *  Returns:
 *      ENOERR on successful validation, EINVAL otherwise.
 */
int validate_board_cols(const char board[81]);

/*
 *  Description:
 *      Validate board elements: rows, cols, and grids.
 *
 *  Args:
 *      board: A fixed-size char array of 81 characters.  Each character must be a SUDO_EMPTY_GRID,
 *          or number ranging from 1-9, inclusive.
 *
 *  Returns:
 *      ENOERR on successful validation, EINVAL otherwise.
 */
int validate_board_elements(const char board[81]);

/*
 *  Description:
 *      Validate every entry in a single board.
 *
 *  Args:
 *      board: A fixed-size char array of 81 characters.  Each character must be a SUDO_EMPTY_GRID,
 *          or number ranging from 1-9, inclusive.
 *
 *  Returns:
 *      ENOERR on successful validation, EINVAL otherwise.
 */
int validate_board_entries(const char board[81]);

/*
 *  Description:
 *      Validate a single board entry.
 *
 *  Args:
 *      board_entry: One character from the board.
 *
 *  Returns:
 *      ENOERR on successful validation, EINVAL otherwise.
 */
int validate_board_entry(char board_entry);

/*
 *  Description:
 *      Validate a grid.
 *
 *  Args:
 *      grid: A fixed-size char array of 9 characters.  Each character must be a SUDO_EMPTY_GRID,
 *          or number ranging from 1-9, inclusive and the numbers may not be duplicated.
 *
 *  Returns:
 *      ENOERR on successful validation, EINVAL otherwise.
 */
int validate_board_grid(const char grid[9]);

/*
 *  Description:
 *      Validate all board grids.
 *
 *  Args:
 *      board: A fixed-size char array of 81 characters.  Each character must be a SUDO_EMPTY_GRID,
 *          or number ranging from 1-9, inclusive.
 *
 *  Returns:
 *      ENOERR on successful validation, EINVAL otherwise.
 */
int validate_board_grids(const char board[81]);

/*
 *  Description:
 *      Validate a row.
 *
 *  Args:
 *      row: A fixed-size char array of 9 characters.  Each character must be a SUDO_EMPTY_GRID,
 *          or number ranging from 1-9, inclusive and the numbers may not be duplicated.
 *
 *  Returns:
 *      ENOERR on successful validation, EINVAL otherwise.
 */
int validate_board_row(const char row[9]);

/*
 *  Description:
 *      Validate board rows.
 *
 *  Args:
 *      board: A fixed-size char array of 81 characters.  Each character must be a SUDO_EMPTY_GRID,
 *          or number ranging from 1-9, inclusive.
 *
 *  Returns:
 *      ENOERR on successful validation, EINVAL otherwise.
 */
int validate_board_rows(const char board[81]);

/**************************************************************************************************/
/********************************** PUBLIC FUNCTION DEFINITIONS ***********************************/
/**************************************************************************************************/


int validate_board(const char board[81])
{
    // LOCAL VARIABLES
    int results = ENOERR;  // Results of execution

    // INPUT VALIDATION
    if (NULL == board)
    {
        results = EINVAL;  // NULL pointer
    }

    // VALIDATE IT
    // Valid entries
    if (ENOERR == results)
    {
        results = validate_board_entries(board);
    }
    // Valid board elements
    if (ENOERR == results)
    {
        results = validate_board_elements(board);
    }

    // DONE
    return results;
}


int validate_board_string(const char *board_string)
{
    // LOCAL VARIABLES
    int results = ENOERR;  // Results of execution
    size_t board_len = 0;  // Length of the board_string

    // INPUT VALIDATION
    if (NULL == board_string)
    {
        results = EINVAL;  // NULL pointer
    }
    else
    {
        board_len = strlen(board_string);
        if (SUDO_BOARD_LEN != board_len)
        {
            fprintf(stderr, "The board string must be %zu characters long instead of "
                    "the %zu provided!\n", SUDO_BOARD_LEN, board_len);
            results = EINVAL;  // Invalid string length
        }
    }

    // VALIDATE IT
    if (ENOERR == results)
    {
        results = validate_board(board_string);
        if (ENOERR != results)
        {
            fprintf(stderr, "An invalid character was detected in the board string!\n");
        }
    }

    // DONE
    return results;
}


int validate_err(int *err)
{
    // LOCAL VARIABLES
    int results = ENOERR;  // The results of validation

    // INPUT VALIDATION
    if (!err)
    {
        results = EINVAL;  // NULL pointer
    }

    // DONE
    return results;
}


/**************************************************************************************************/
/********************************** PRIVATE FUNCTION DEFINITIONS **********************************/
/**************************************************************************************************/


int validate_board_array(const char array[9])
{
    // LOCAL VARIABLES
    int results = ENOERR;   // Results of execution
    int found[10] = { 0 };  // Keep track of chars found by index (index 1 represents '1')

    // INPUT VALIDATION
    if (NULL == array)
    {
        results = EINVAL;  // NULL pointer
    }

    // VALIDATE ROW
    if (ENOERR == results)
    {
        // Character count
        for (int i = 0; i < 9; i++)
        {
            if (SUDO_EMPTY_GRID != array[i])
            {
                found[array[i] - '0'] += 1;  // Found one
            }
        }
        // Validate count
        for (int i = 1; i <= 9; i++)
        {
            if (found[i] > 1)
            {
                results = EINVAL;  // Invalid entry count found
                FPRINTF_ERR("Redundant entries found for the character '%c'\n", i + '0');
                break;  // Found a violation so stop checking
            }
        }
    }

    // DONE
    return results;
}


int validate_board_col(const char col[9])
{
    // LOCAL VARIABLES
    int results = validate_board_array(col);  // Results of execution

    // VALIDATE COL
    if (ENOERR != results)
    {
        PRINT_ERROR(Detected a duplicate entry in a col);
    }

    // DONE
    return results;
}


int validate_board_cols(const char board[81])
{
    // LOCAL VARIABLES
    int results = ENOERR;                  // Results of execution
    char (*game)[9] = (char (*)[9])board;  // Cast it to a two-dimensional array
    char col_copy[9] = { "\0" };           // Column copy
    char *copy_ptr = col_copy;             // Iterating pointer into col_copy

    // INPUT VALIDATION
    if (NULL == board)
    {
        results = EINVAL;  // NULL pointer
    }

    // VALIDATE ROWS
    if (ENOERR == results)
    {
        for (int col_index = 0; col_index < 9; col_index++)
        {
            copy_ptr = col_copy;  // Reset iterating pointer
            for (int row_index = 0; row_index < 9; row_index++)
            {
                *copy_ptr = game[row_index][col_index];
                copy_ptr++;
            }
            results = validate_board_col(col_copy);
            if (ENOERR != results)
            {
                break;  // Found a violation so stop looking
            }
        }
    }

    // DONE
    return results;
}


int validate_board_elements(const char board[81])
{
    // LOCAL VARIABLES
    int results = ENOERR;  // Results of execution

    // INPUT VALIDATION
    if (NULL == board)
    {
        results = EINVAL;  // NULL pointer
    }

    // VALIDATE ELEMENTS
    if (ENOERR == results)
    {
        results = validate_board_rows(board);
    }
    if (ENOERR == results)
    {
        results = validate_board_cols(board);
    }
    if (ENOERR == results)
    {
        results = validate_board_grids(board);
    }

    // DONE
    return results;
}


int validate_board_entries(const char board[81])
{
    // LOCAL VARIABLES
    int results = ENOERR;  // Results of execution

    // INPUT VALIDATION
    if (NULL == board)
    {
        results = EINVAL;  // NULL pointer
    }

    // VALIDATE ENTRIES
    if (ENOERR == results)
    {
        for (size_t i = 0; i < SUDO_BOARD_LEN; i++)
        {
            results = validate_board_entry(board[i]);
            if (ENOERR != results)
            {
                // PRINT_ERRNO(results);  // DEBUGGING
                FPRINTF_ERR("%s %s found an invalid character at index %zu: [%d] '%c'\n",
                            DEBUG_WARNG_STR, __FUNCTION_NAME__, i, board[i], board[i]);
                break;  // Stop looking because we found a board violation
            }
        }
    }

    // DONE
    return results;
}


int validate_board_entry(char board_entry)
{
    // LOCAL VARIABLES
    int results = EINVAL;  // The results of validation
    char valid_entries[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', SUDO_EMPTY_GRID };

    // VALIDATE IT
    // FPRINTF_ERR("VALIDATING BOARD ENTRY: [%d] %c\n", board_entry, board_entry);  // DEBUGGING
    for (int i = 0; i < (sizeof(valid_entries) / sizeof(valid_entries[0])); i++)
    {
        if (board_entry == valid_entries[i])
        {
            results = ENOERR;  // Found a good entry
            break;  // Stop looking
        }
    }

    // DONE
    return results;
}


int validate_board_grid(const char grid[9])
{
    // LOCAL VARIABLES
    int results = validate_board_array(grid);  // Results of execution

    // VALIDATE ROW
    if (ENOERR != results)
    {
        PRINT_ERROR(Detected a duplicate entry in a grid);
    }

    // DONE
    return results;
}


int validate_board_grids(const char board[81])
{
    // LOCAL VARIABLES
    int results = ENOERR;                  // Results of execution
    char (*game)[9] = (char (*)[9])board;  // Cast it to a two-dimensional array
    int start_row = 0;                     // Iterate over the grid's rows
    int start_col = 0;                     // Iterate over the grid's cols
    char grid_copy[9] = { "\0" };          // Copy of the grid contents
    char *grid_ptr = grid_copy;            // Iterating pointer into col_copy

    // INPUT VALIDATION
    if (NULL == board)
    {
        results = EINVAL;  // NULL pointer
    }

    // VALIDATE GRIDS
    if (ENOERR == results)
    {
        for (int grid_num = 1; grid_num <= 9; grid_num++)
        {
            grid_ptr = grid_copy;  // Reset the iterating pointer
            // Get the values from the grid
            start_row = 3 * ((grid_num - 1) / 3);
            start_col = 3 * ((grid_num - 1) % 3);
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    *grid_ptr = game[start_row + i][start_col + j];
                    grid_ptr++;
                }
            }
            results = validate_board_grid(grid_copy);
            if (ENOERR != results)
            {
                FPRINTF_ERR("Detected a duplicate entry in grid number %d\n", grid_num);
                break;  // Found a violation so stop looking
            }
        }
    }

    // DONE
    return results;
}


int validate_board_row(const char row[9])
{
    // LOCAL VARIABLES
    int results = validate_board_array(row);  // Results of execution

    // VALIDATE ROW
    if (ENOERR != results)
    {
        PRINT_ERROR(Detected a duplicate entry in a row);
    }

    // DONE
    return results;
}


int validate_board_rows(const char board[81])
{
    // LOCAL VARIABLES
    int results = ENOERR;                  // Results of execution
    char (*game)[9] = (char (*)[9])board;  // Cast it to a two-dimensional array

    // INPUT VALIDATION
    if (NULL == board)
    {
        results = EINVAL;  // NULL pointer
    }

    // VALIDATE ROWS
    if (ENOERR == results)
    {
        for (int row_index = 0; row_index < 9; row_index++)
        {
            results = validate_board_row(game[row_index]);
            if (ENOERR != results)
            {
                break;  // Found a violation so stop looking
            }
        }
    }

    // DONE
    return results;
}
