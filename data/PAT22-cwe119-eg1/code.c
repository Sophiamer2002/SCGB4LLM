#include <stdlib.h>

extern void deal_with_argument(char *arg);

/**
 * @brief Entry point of the program that processes a command-line argument.
 * 
 * This function takes an index from the first command-line argument, validates it,
 * and passes the corresponding argument to the `deal_with_argument` function.
 * 
 * @param argc The number of command-line arguments.
 * @param argv An array of strings representing the command-line arguments.
 *             The first element (argv[0]) is the program name.
 * 
 * @return int Returns 0 on successful execution.
 *             Returns 1 if the index is invalid.
 * 
 * @example
 * Command-line usage:
 * ./program 2 arg1 arg2 arg3
 * 
 * In this example:
 * - argv[1] is "2", which is converted to the integer index 2.
 * - argv[1 + 2] ("arg2") is passed to the `deal_with_argument` function.
 */
int main(int argc, char *argv[]) {
// BEGIN SOLUTION
    if (argc < 2) {
        return 1; // Not enough arguments
    }

    int index = atoi(argv[1]); // Convert the first argument to an integer
    if (index <= 0 || index >= argc - 1) {
        return 1;
    }

    char *arg = argv[index + 1];
    deal_with_argument(arg);
    return 0;
// END SOLUTION
}