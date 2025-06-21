/*
 *  This library defines functionality to allocate and free memory on behalf of SUDO.
 */

#ifndef __SUDO_MEMORY__
#define __SUDO_MEMORY__

#include <stddef.h>                         // size_t
#include "sudo_macros.h"                    // ENOERR

/*
 *  Description:
 *      Allocate a zeroized array in heap memory.
 *
 *  Args:
 *      num_elem: The number of elements in the array.
 *      size_elem: The size of each element in the array.
 *      errnum: [Out] Storage location for errno values encountered.
 *
 *  Returns:
 *      Heap-allocated memory of total size num_elem * size_elem that has been zeroized, on success.
 *      Caller is responsible for freeing the return value with free_sudo_mem().
 *      NULL on error (check errnum for details).
 */
void *alloc_sudo_mem(size_t num_elem, size_t size_elem, int *errnum);

/*
 *  Description:
 *      Free sudo-allocated heap memory and set the original pointer to NULL.
 *
 *  Args:
 *      old_mem: Pointer to the heap-allocated memory's storage location.
 *
 *  Returns:
 *      0 on success, errno on error.
 */
int free_sudo_mem(void **old_mem);

#endif  /* __SUDO_MEMORY__ */
