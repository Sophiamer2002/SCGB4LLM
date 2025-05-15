#include <ctype.h>
#include <string.h>

/**
 * @brief Removes trailing whitespace characters from a given string.
 * 
 * This function modifies the input string in place by removing any
 * whitespace characters (e.g., spaces, tabs, newlines) from the end
 * of the string.
 * 
 * @param strMessage A pointer to the input string to be trimmed.
 *                   The string is a valid null-terminated string and
 *                   is not NULL.
 */
void trimTrailingWhitespace(char *strMessage) {
// BEGIN SOLUTION
    size_t len = strlen(strMessage);
    while (len > 0 && isspace(strMessage[len - 1])) {
        strMessage[--len] = '\0';
    }
// END SOLUTION
}