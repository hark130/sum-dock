/*
 *  This library defines functionality to manage the game board on behalf of SUDO.
 */

#ifndef __SUDO_BOARD__
#define __SUDO_BOARD__

/*
 *  Description:
 *      Copy the board_string into heap-allocated memory.
 *
 *  Args:
 *      board_string: A nul-terminated char array of 81 characters.  Each character must be a
 *			SUDO_EMPTY_GRID or number ranging from 1-9, inclusive.
 *      errnum: [Out] Provide feedback on execution.
 *
 *  Returns:
 *      Heap-allocated pointer to 81 characters, no more no less, on success.  NULL on error
 *      (see errnum for details).
 */
char *create_board(const char *board_string, int *errnum);

/*
 *  Description:
 *      Print a human-readable version of the board.
 *
 *  Args:
 *      board: A fixed-size array of 81 characters.  Each character must be a SUDO_EMPTY_GRID
 *          or number ranging from 1-9, inclusive.
 *
 *  Returns:
 *      ENOERR on successful validation, errno on error.
 */
int print_board(const char board[81]);

#endif  /* __SUDO_BOARD__ */
