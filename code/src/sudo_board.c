/*
 *  This library defines functionality to manage the game board on behalf of SUDO.
 */

#define SKID_DEBUG                          // Enable DEBUG logging

#include <errno.h>                          // EINVAL
#include <stdio.h>                          // printf()
#include <string.h>                         // memcpy()
#include "sudo_debug.h"                     // MODULE_*LOAD(), PRINT_ERRNO(), PRINT_ERROR()
#include "sudo_macros.h"                    // ENOERR, SUDO_BOARD_LEN
#include "sudo_memory.h"                    // alloc_sudo_mem()
#include "sudo_validation.h"                // validate_err(), validate_board_string()


MODULE_LOAD();  // Print the module name being loaded using the gcc constructor attribute
MODULE_UNLOAD();  // Print the module name being unloaded using the gcc destructor attribute


/**************************************************************************************************/
/********************************* PRIVATE FUNCTION DECLARATIONS **********************************/
/**************************************************************************************************/



/**************************************************************************************************/
/********************************** PUBLIC FUNCTION DEFINITIONS ***********************************/
/**************************************************************************************************/


char *create_board(const char *board_string, int *errnum)
{
    // LOCAL VARIABLES
    int results = ENOERR;    // Results of execution
    char *board_ptr = NULL;  // Heap-allocated board array

    // INPUT VALIDATION
    results = validate_board_string(board_string);
    if (ENOERR == results)
    {
        results = validate_err(errnum);
    }

    // CREATE IT
    if (ENOERR == results)
    {
        board_ptr = (char*)alloc_sudo_mem(SUDO_BOARD_LEN, sizeof(char), &results);
    }
    if (ENOERR == results)
    {
        memcpy(board_ptr, board_string, SUDO_BOARD_LEN * sizeof(char));
    }

    // DONE
    if (NULL != errnum)
    {
        *errnum = results;
    }
    return board_ptr;
}


int print_board(const char board[81])
{
    // LOCAL VARIABLES
    int results = ENOERR;    // Results of execution

    // INPUT VALIDATION
    results = validate_board(board);

    // PRINT IT
    if (ENOERR == results)
    {
        for (size_t i = 0; i < SUDO_BOARD_LEN; i++)
        {
            if (0 == (i % 9))
            {
                printf("\n");  // Start a newline
            }
            printf("%c", board[i]);
        }
        printf("\n\n");
    }

    // DONE
    return results;
}


/**************************************************************************************************/
/********************************** PRIVATE FUNCTION DEFINITIONS **********************************/
/**************************************************************************************************/
