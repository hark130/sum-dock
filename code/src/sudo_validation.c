/*
 *  This library defines functionality to validate arguments on behalf of SUDO.
 */

#define SKID_DEBUG                          // Enable DEBUG logging

#include <errno.h>                          // EINVAL
#include <stddef.h>                         // size_t
#include <stdio.h>                          // fprintf()
#include <string.h>                         // strlen()
#include "sudo_debug.h"                     // MODULE_*LOAD(), PRINT_ERRNO(), PRINT_ERROR()
#include "sudo_macros.h"                    // ENOERR, SUDO_BOARD_LEN

MODULE_LOAD();  // Print the module name being loaded using the gcc constructor attribute
MODULE_UNLOAD();  // Print the module name being unloaded using the gcc destructor attribute


/**************************************************************************************************/
/********************************* PRIVATE FUNCTION DECLARATIONS **********************************/
/**************************************************************************************************/

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
    if (ENOERR == results)
    {
        for (size_t i = 0; i < SUDO_BOARD_LEN; i++)
        {
            results = validate_board_entry(board[i]);
            if (ENOERR != results)
            {
                FPRINTF_ERR("%s %s found an invalid character at index %zu: %c\n",
                            DEBUG_WARNG_STR, __FUNCTION_NAME__, i, board[i]);
            }
        }
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


int validate_board_entry(char board_entry)
{
    // LOCAL VARIABLES
    int results = EINVAL;  // The results of validation
    char valid_entries[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ' };

    // VALIDATE IT
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
