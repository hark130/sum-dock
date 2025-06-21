/*
 *    Defines common use macros for use with SUM DOCK (SUDO).
 */

#ifndef __SUDO_MACROS__
#define __SUDO_MACROS__

/* GENERAL MACROS */
// ENOERR
#ifndef ENOERR
    #define ENOERR ((int)0)  // Success value for errno
#endif  /* ENOERR */
// NULL
#ifndef NULL
    #define NULL ((void *)0)  // Just in case it's not already defined
#endif  /* NULL */
// SUDO_MAX_SZ
#define SUDO_MAX_SZ (~(size_t)0)    // Library's value for the maximum size_t value

#endif  /* __SUDO_MACROS__ */
