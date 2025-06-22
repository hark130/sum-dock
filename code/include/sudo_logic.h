/*
 *  This library defines game logic functionality to solve a game board.
 */

#ifndef __SUDO_LOGIC__
#define __SUDO_LOGIC__

/*
 *  Description:
 *      Is the game over?
 *
 *  Args:
 *      board: A fixed-size array of 81 characters.  Each character must be a SUDO_EMPTY_GRID
 *          or number ranging from 1-9, inclusive.
 *
 *  Returns:
 *      ENOERR on game over, ENODATA for an unsolved game board, or errno on error.
 */
int is_game_over(char board[81]);

/*
 *  Description:
 *      Solve a game board.
 *
 *  Args:
 *      board: A fixed-size array of 81 characters.  Each character must be a SUDO_EMPTY_GRID
 *          or number ranging from 1-9, inclusive.
 *
 *  Returns:
 *      ENOERR on success, ENODATA for an unsolved game board, or errno on error.
 */
int solve_board(char board[81]);

#endif  /* __SUDO_LOGIC__ */
