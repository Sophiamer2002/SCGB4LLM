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
    size_t length = strlen(strMessage);

    // Find the last non-whitespace character
    size_t i;
    for (i = length - 1; i < length; i--) {
        if (!isspace(strMessage[i])) {
            break;
        }
    }

    // Null-terminate the string at the new end position
    strMessage[i + 1] = '\0';
// END SOLUTION
}