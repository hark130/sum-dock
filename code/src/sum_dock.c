/*
 *  Defines main() for the sum_dock binary.
 */

#define SUDO_DEBUG  // Enable DEBUG logging

#include "sudo_debug.h"   // MODULE_LOAD(), MODULE_UNLOAD()
#include "sudo_macros.h"  // ENOERR


MODULE_LOAD();  // Print the module name being loaded using the gcc constructor attribute
MODULE_UNLOAD();  // Print the module name being unloaded using the gcc destructor attribute


int main()
{
    // LOCAL VARIABLES
    int results = ENOERR;  // Errno value from execution


    // DONE
    return results;
}
