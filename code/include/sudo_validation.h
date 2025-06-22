/*
 *  This library defines functionality to validate arguments on behalf of SUDO.
 */

#ifndef __SUDO_VALIDATION__
#define __SUDO_VALIDATION__

/*
 *  Description:
 *      Validate the sudoku game board.
 *
 *  Args:
 *      board: A fixed-size char array of 81 characters.  Each character must be a SUDO_EMPTY_GRID,
 *          or number ranging from 1-9, inclusive.
 *
 *  Returns:
 *      ENOERR on successful validation, EINVAL otherwise.
 */
int validate_board(const char board[81]);

/*
 *  Description:
 *      Validate the user-provided board string.
 *
 *  Args:
 *      board_string: A nul-terminated char array of 81 characters.  Each character must be a
 *			SUDO_EMPTY_GRID, or number ranging from 1-9, inclusive.
 *
 *  Returns:
 *      ENOERR on successful validation, EINVAL otherwise.
 */
int validate_board_string(const char *board_string);

/*
 *  Description:
 *      Validates the errnum arguments.
 *
 *  Args:
 *      err: A non-NULL pointer to an integer.
 *
 *  Returns:
 *      An errno value indicating the results of validation.  ENOERR on successful validation.
 */
int validate_err(int *err);

#endif  /* __SUDO_VALIDATION__ */
