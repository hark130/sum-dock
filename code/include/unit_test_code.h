/*
 *    This library contains non-releasable, common-use Check unit test code.
 */

#ifndef __SUDO_UNIT_TEST_CODE__
#define __SUDO_UNIT_TEST_CODE__

#include <stdbool.h>						// bool
// Local includes

// Use this with printf("%s", BOOL_STR_LIT(bool)); to print human readable results
#define BOOL_STR_LIT(boolean) (boolean ? "true" : "false")
#define CANARY_INT (int)0xBADC0DE  // Actually, a reverse canary value
#define SUDO_REPO_NAME (const char *)"sum-dock"  // The name of this repo


/*
 *  Description:
 *      Allocate zeroized array in heap memory.
 *
 *  Args:
 *      num_elem: The number of elements in the array.
 *        size_elem: The size of each element in the array.
 *        errnum: [Out] Storage location for errno values encountered.
 *
 *  Returns:
 *      Heap-allocated array of total size num_elem * size_elem that has been zeroized, on success.
 *        Caller is respsonsible for freeing the return value with free_devops_mem().
 *        NULL on error (check errnum).
 */
void *alloc_devops_mem(size_t num_elem, size_t size_elem, int *errnum);

/*
 *  Description:
 *      Free a devops-allocated array from heap memory and set the original pointer to NULL.
 *
 *  Args:
 *      old_array: Pointer to the heap-allocated array to free storage location.
 *
 *  Returns:
 *      0 on success, errno on error.
 */
int free_devops_mem(void **old_array);

/*
 *  Resolve paththame to SUDO_REPO_NAME in a standardized way.  Use free_devops_mem() to free
 *  the return value.
 */
/*
 *  Description:
 *      Resolve paththame to SUDO_REPO_NAME in a standardized way.  Use free_devops_mem() to free
 *  	the return value.
 *
 *  Args:
 *      pathname: Relative pathname to resolve.
 *
 *  Returns:
 *      Heap-allocated pointer on success, NULL on failure.
 */
char *resolve_test_input(const char *pathname);

/*
 *  Description:
 *      Translate rel_filename into an absolute filename resolved to the repo_name, as extracted
 *        from the current working directory.  Caller is responsible for calling devops_free().
 *
 *  Args:
 *      repo_name: Root-level directory name to find in the current working directory
 *            (e.g., SKID_REPO_NAME).
 *      rel_filename: Optional; The relative filename to resolve to repo_name's absolute path.
 *            If this argument is NULL, only the repo_dir will be returned.
 *        must_exist: If true, the resulting repo_dir/rel_filename must exist or errnum will
 *            be updated with ENOENT.  If false, it doesn't matter (e.g., "missing file" error input).
 *        errnum: [Out] Storage location for errno values encountered.
 *
 *  Returns:
 *      Heap-allocated array representing repo_dir/rel_filename on success.  Caller is
 *        respsonsible for freeing the return value with free_devops_mem().
 *        NULL on error (check errnum).
 */
char *resolve_to_repo(const char *repo_name, const char *rel_filename, bool must_exist,
                      int *errnum);


#endif  /* __SUDO_UNIT_TEST_CODE__ */
