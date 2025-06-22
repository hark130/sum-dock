/*
 *  This library contains non-releasable, common-use Check unit test code.
 */


#include <check.h>                          // Check assert messages
#include <errno.h>                          // errno
#include <limits.h>                         // PATH_MAX
#include <stdlib.h>                         // calloc()
#include <string.h>                         // strerror()
#include <sys/stat.h>                       // stat(), struct stat
#include <unistd.h>                         // getcwd()
// Local includes
#include "sudo_debug.h"                     // PRINT_ERRNO(), PRINT_ERROR(), FPRINTF_ERR()
#include "sudo_macros.h"                    // ENOERR
#include "unit_test_code.h"                 // CANARY_INT, SUDO_REPO_NAME, resolve_to_repo()

/**************************************************************************************************/
/********************************* PRIVATE FUNCTION DECLARATIONS **********************************/
/**************************************************************************************************/

/*
 *    Description:
 *        Allocate heap-memory and concatenate dirname/pathname, adding a delimiter if necessary.
 *        The caller is responsible for using free_devops_mem() to free the memory address returned
 *        by this function.
 *
 *    Args:
 *        dirname: The absolute or relative directory name which contains pathname.
 *        pathname: Optional; Absolute or relative pathname to join to dirname.  This function
 *            will add a delimiter between dirname/pathname if pathname is defined.  Ignored if NULL
 *            or empty.
 *        must_exist: If true, dirname must exist.
 *      errnum: [Out] Storage location for errno values encountered.
 *
 *    Returns:
 *        Heap-allocated memory address containing dirname/pathname, on success.
 *        Returns NULL on error.  Check errnum for errno value.
 */
char *join_dir_to_path(const char *dirname, const char *pathname, bool must_exist, int *errnum);

/*
 *    Description:
 *        Answers the question, "Does pathname exist?".  Any invalid input is treated as a "no".
 *        The following errno values are also treated as a "no":
 *            ENOENT is obvious... file flat out doesn't exist.
 *            ENAMETOOLONG means pathname is too long to it *can't* exist.
 *            ENOTDIR means part of the path prefix of pathname is not a dir so it *can't* exist.
 *        EACCES is tricky because it could be inconclusive.  Permission could be denied for one
 *        of the directories in the path prefix of pathname.  Still, this function treats EACCES
 *        as a "yes" because this is devops code (but prints a DEBUG warning).
 *
 *    Args:
 *        pathname: Absolute or relative pathname to check.
 *
 *    Returns:
 *        True if pathname exists.  False otherwise.
 */
bool is_path_there(const char *pathname);

/*
 *  Description:
 *      Find needle in haystack.  Truncate the rest of hastack with a trailing "/\0".
 *
 *  Args:
 *      haystack: The buffer, holding an absolute directory, to search for needle in and then
 *          modify.
 *      needle: The directory name to look for in haystack.
 *      hay_len: The number of elements in haystack, to avoid buffer overruns.
 *
 *  Returns:
 *      0 on success, ENOKEY if needle is not found in haystack, ENOBUFS if haystack is not
 *      big enough to one more character, errno on error.
 */
int truncate_dir(char *haystack, const char *needle, size_t hay_len);

/*
 *  Description:
 *      Validate standard errno [Out] args on behalf of the library.  No values are changed.
 *
 *  Args:
 *      err: This must be a valid pointer.
 *
 *  Returns:
 *      0 on success, EINVAL for bad input.
 */
int validate_devops_err(int *err);

/*
 *  Description:
 *      Validate standard path names on behalf of the library.  No values are changed.
 *
 *  Args:
 *      name: This must be a valid pointer to a string that is not empty.
 *
 *  Returns:
 *      0 on success, EINVAL for bad input.
 */
int validate_devops_name(const char *name);

/*
 *  Description:
 *      Validate standard arguments on behalf of the library.  No values are changed.
 *
 *  Args:
 *      name: This must be a valid pointer to a string that is not empty.
 *      err: This must be a valid pointer.
 *
 *  Returns:
 *      0 on success, EINVAL for bad input.
 */
int validate_devops_args(const char *name, int *err);


/**************************************************************************************************/
/********************************** PUBLIC FUNCTION DEFINITIONS ***********************************/
/**************************************************************************************************/


void *alloc_devops_mem(size_t num_elem, size_t size_elem, int *errnum)
{
    // LOCAL VARIABLES
    int result = ENOERR;   // Errno value
    void *new_buf = NULL;  // Pointer to the newly allocated buffer

    // INPUT VALIDATION
    if (num_elem <= 0 || size_elem <= 0 || !errnum)
    {
        result = EINVAL;  // Invalid argument
    }

    // ALLOCATE!
    if (ENOERR == result)
    {
        new_buf = calloc(num_elem, size_elem);
        if (!new_buf)
        {
            result = errno;
            PRINT_ERROR(The call to calloc() failed);
            PRINT_ERRNO(result);
        }
    }

    // DONE
    if (errnum)
    {
        *errnum = result;
    }
    return new_buf;
}


int free_devops_mem(void **old_array)
{
    // LOCAL VARIABLES
    int result = ENOERR;   // Errno value
    void *old_buf = NULL;  // Pointer to the old buffer to free

    // INPUT VALIDATION
    if (old_array && *old_array)
    {
        old_buf = *old_array;
        free(old_buf);
        old_buf = NULL;
        *old_array = NULL;
    }
    else
    {
        result = EINVAL;  // NULL pointer
    }

    // DONE
    return result;
}


char *resolve_test_input(const char *pathname)
{
    // LOCAL VARIABLES
    int errnum = CANARY_INT;                 // Errno values
    const char *repo_name = SUDO_REPO_NAME;  // Name of the repo
    char *resolved_name = NULL;              // pathname resolved to repo_name

    // RESOLVE IT
    resolved_name = resolve_to_repo(repo_name, pathname, false, &errnum);
    ck_assert_msg(0 == errnum, "resolve_to_repo(%s, %s) failed with [%d] %s\n", repo_name,
                  pathname, errnum, strerror(errnum));
    ck_assert_msg(NULL != resolved_name, "resolve_to_repo(%s, %s) failed to resolve the path\n",
                  repo_name, pathname);

    // DONE
    if (0 != errnum && resolved_name)
    {
        free_devops_mem((void **)&resolved_name);  // Best effort
    }
    return resolved_name;
}


char *resolve_to_repo(const char *repo_name, const char *rel_filename, bool must_exist, int *errnum)
{
    // LOCAL VARIABLES
    int result = ENOERR;                   // Errno value
    char cwd[PATH_MAX + 1] = { 0 };        // Current working directory
    const char *tmp_file = rel_filename;   // Temp pointer to rel_filename
    char *abs_file = NULL;                 // repo_dir/rel_filename

    // INPUT VALIDATION
    result = validate_devops_args(repo_name, errnum);

    // 1. Get current working directory
    if (ENOERR == result)
    {
        if (cwd != getcwd(cwd, sizeof(cwd)))
        {
            result = errno;
            PRINT_ERROR(The call to getcwd() failed);
            PRINT_ERRNO(result);
        }
    }
    // 2. Find repo_name within the cwd, creating repo_dir
    if (ENOERR == result)
    {
        result = truncate_dir(cwd, repo_name, sizeof(cwd));
    }
    // Allocate and check
    if (ENOERR == result)
    {
        // 3. Join repo_dir to rel_filename (Optional)
        if (tmp_file && *tmp_file)
        {
            // First, advance past any leading delimiters or periods
            while (*tmp_file == '/' || *tmp_file == '.')
            {
                tmp_file++;
            }
        }
        // 4. Check must_exist
        abs_file = join_dir_to_path(cwd, tmp_file, must_exist, &result);
        if (result)
        {
            PRINT_ERROR(The call to join_dir_to_path() failed);
            PRINT_ERRNO(result);
        }
    }

    // CLEANUP
    if (ENOERR != result && abs_file)
    {
        free_devops_mem((void**)&abs_file);
    }

    // DONE
    if (errnum)
    {
        *errnum = result;
    }
    return abs_file;
}


/**************************************************************************************************/
/********************************** PRIVATE FUNCTION DEFINITIONS **********************************/
/**************************************************************************************************/


char *join_dir_to_path(const char *dirname, const char *pathname, bool must_exist, int *errnum)
{
    // LOCAL VARIABLES
    int result = validate_devops_args(dirname, errnum);  // Results of execution
    char *joined_path = NULL;                              // dirname/pathname
    size_t total_len = 0;                                  // Total length of dirname/pathanme
    bool add_delim = false;                                // Add a / if necessary
    bool append_path = false;                              // Pathname is optional

    // INPUT VALIDATION
    // pathname
    if (ENOERR == result && pathname && *pathname)
    {
        append_path = true;  // It's there so let's append it
    }
    // dirname + must_exist
    if (ENOERR == result && true == must_exist)
    {
        if (false == is_path_there(dirname))
        {
            FPRINTF_ERR("%s - Unable to locate %s\n", DEBUG_ERROR_STR, dirname);
            result = ENOENT;  // Missing dirname
        }
    }

    // JOIN IT
    // Determine length
    if (ENOERR == result)
    {
        total_len = strlen(dirname);
        // Only add a delimiter if we need to
        if (true == append_path)
        {
            if ('/' != dirname[total_len - 1])
            {
                total_len++;  // Leave a space for the delimiter
                add_delim = true;  // Add a delimiter later
            }
            total_len += strlen(pathname);
        }
    }
    // Allocate
    if (ENOERR == result)
    {
        joined_path = alloc_devops_mem(total_len + 1, sizeof(char), &result);
        if (result)
        {
            PRINT_ERROR(The call to alloc_devops_mem() failed);
        }
    }
    // Concatenate
    if (ENOERR == result)
    {
        // dirname
        strncpy(joined_path, dirname, strlen(dirname));
        // pathname
        if (true == append_path)
        {
            // delimiter
            if (true == add_delim)
            {
                joined_path[strlen(joined_path)] = '/';
            }
            strncat(joined_path, pathname, total_len + 1);
        }
    }

    // CLEANUP
    if (result && joined_path)
    {
        free_devops_mem((void **)&joined_path);  // There was an error so let's cleanup
    }

    // DONE
    if (errnum)
    {
        *errnum = result;
    }
    return joined_path;
}


bool is_path_there(const char *pathname)
{
    // LOCAL VARIABLES
    bool is_there = false;  // Does pathname exist?
    int errnum = ENOERR;    // Errno value
    struct stat statbuf;    // Used in the call to stat()

    // INPUT VALIDATION
    if (pathname && *pathname)
    {
        if(stat(pathname, &statbuf))
        {
            errnum = errno;
            // ENOENT is obvious... file flat out doesn't exist.
            // ENAMETOOLONG means pathname is too long to it *can't* exist.
            // ENOTDIR means part of the path prefix of pathname is not a dir so it *can't* exist.
            if (ENOENT != errnum && ENAMETOOLONG != errnum && ENOTDIR != errnum)
            {
                if (EACCES == errnum)
                {
                    // Permission denied might refer to a directory in pathname's path prefix
                    PRINT_WARNG(The errno value of EACCESS is inconclusive);  // Just a warning
                }
                is_there = true;  // Other errors means it's there, even if there's a problem
            }
        }
        else
        {
            is_there = true;  // Found it
        }
    }

    // DONE
    return is_there;
}


int truncate_dir(char *haystack, const char *needle, size_t hay_len)
{
    // LOCAL VARIABLES
    int result = ENOERR;  // Errno value
    char *temp = NULL;    // Temp pointer

    // INPUT VALIDATION
    result = validate_devops_name(haystack);
    if (ENOERR == result)
    {
        result = validate_devops_name(needle);
    }
    if (ENOERR == result && hay_len <= 0)
    {
        result = EINVAL;  // Invalid hay_len
    }

    // TRUNCATE IT
    // 1. Find needle in haystack
    if (ENOERR == result)
    {
        temp = strstr(haystack, needle);
        if (!temp)
        {
            if (errno)
            {
                result = errno;
            }
            else
            {
                result = ENOKEY;  // Needle not found in haystack
            }
            PRINT_ERROR(The call to strstr() did not succeed);
            PRINT_ERRNO(result);
            FPRINTF_ERR("Failed to find needle '%s' in haystack '%s'\n", needle, haystack);
        }
    }
    // 2. Verify haystack has room
    if (ENOERR == result)
    {
        temp += strlen(needle);  // Points to index after the needle
        if (hay_len < (temp - haystack + 2))
        {
            result = ENOBUFS;  // Not enough room for a trailing slash and nul char
            PRINT_WARNG(Not enough space to truncate haystack);
        }
    }
    // 3. Truncate haystack
    if (ENOERR == result)
    {
        *temp = '/';
        temp++;
        *temp = '\0';
    }

    // DONE
    return result;
}


int validate_devops_err(int *err)
{
    // LOCAL VARIABLES
    int result = ENOERR;  // Errno value

    // INPUT VALIDATION
    if (!err)
    {
        result = EINVAL;  // Bad input
    }

    // DONE
    return result;
}


int validate_devops_name(const char *name)
{
    // LOCAL VARIABLES
    int result = ENOERR;  // Errno value

    // INPUT VALIDATION
    if (!name || !(*name))
    {
        result = EINVAL;  // Bad input
    }

    // DONE
    return result;
}


int validate_devops_args(const char *name, int *err)
{
    // LOCAL VARIABLES
    int result = validate_devops_name(name);  // Errno value

    // INPUT VALIDATION
    if (ENOERR == result)
    {
        result = validate_devops_err(err);
    }

    // DONE
    return result;
}

