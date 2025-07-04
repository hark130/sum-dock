/*
 *  Defines main() for the sum_dock binary.
 */

// #define SUDO_DEBUG                          // Enable DEBUG logging

#include <errno.h>                          // EINVAL
#include <stdio.h>                          // printf()
#include <string.h>                         // strerror()
#include "sudo_board.h"                     // create_board(), print_board()
#include "sudo_debug.h"                     // MODULE_LOAD(), MODULE_UNLOAD()
#include "sudo_logic.h"                     // solve_board()
#include "sudo_macros.h"                    // ENOERR
#include "sudo_memory.h"                    // free_sudo_mem()


MODULE_LOAD();  // Print the module name being loaded using the gcc constructor attribute
MODULE_UNLOAD();  // Print the module name being unloaded using the gcc destructor attribute


/**************************************************************************************************/
/********************************* PRIVATE FUNCTION DECLARATIONS **********************************/
/**************************************************************************************************/

/*
 *  Entry point for SUDO.  Returns errno on error, ENOERR on success.
 */
int main(int argc, char *argv[]);

/*
 *  Single point of truth for this manual test code's usage.
 */
void print_usage(const char *prog_name);

/**************************************************************************************************/
/********************************** PRIVATE FUNCTION DEFINITIONS **********************************/
/**************************************************************************************************/


int main(int argc, char *argv[])
{
    // LOCAL VARIABLES
    int results = ENOERR;     // Errno value from execution
    char *game_board = NULL;  // Heap-allocated copy of the argv string provided

    // INPUT VALIDATION
    if (2 != argc)
    {
        results = EINVAL;  // Invalid number of arguments
    }

    // SUDO IT!
    // Create the game board
    if (ENOERR == results)
    {
        game_board = create_board(argv[1], &results);
    }
    // Print the starting board
    if (ENOERR == results)
    {
        printf("Starting board:\n");
        results = print_board(game_board);
    }
    // Solve the game board
    if (ENOERR == results)
    {
        results = solve_board(game_board);
        // Determine results
        if (ENOERR == results)
        {
            printf("Board solved!\n");
        }
        else if (ENODATA == results)
        {
            printf("Failed to solve the board.\n");
        }
        else
        {
            printf("The game logic encountered an error: %s?!\n", strerror(results));
        }
        print_board(game_board);
    }

    // CLEANUP
    if (NULL != game_board)
    {
        free_sudo_mem((void**)&game_board);  // Best effort
    }

    // DONE
    if (ENOERR != results && ENODATA != results)
    {
        print_usage(argv[0]);
    }
    return results;
}


void print_usage(const char *prog_name)
{
    fprintf(stderr, "Usage: %s <SUDOKO BOARD STRING>\n", prog_name);
}
