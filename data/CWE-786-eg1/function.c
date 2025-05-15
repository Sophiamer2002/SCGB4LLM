#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void trimTrailingWhitespace(char *strMessage) {
// BEGIN SOLUTION
    size_t len = strlen(strMessage);
    while (len > 0 && isspace(strMessage[len - 1])) {
        strMessage[--len] = '\0';
    }
// END SOLUTION
}

int main() {
    char *tests[][2] = {
        {"afjaldkf", "afjaldkf"},
        {"      ", ""},
        {"afoi    \n", "afoi"},
        {"uuu\t\t  \n", "uuu"},
        {"10abfa   \t", "10abfa"}
    };

    char test_buf[512];

    const int TEST = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < TEST; i++) {
        size_t len = strlen(tests[i][0]);

        strcpy(test_buf, tests[i][0]);
        char *expected = tests[i][1];

        trimTrailingWhitespace(test_buf);

        if (strcmp(test_buf, expected) != 0) {
            return 1;
        }
    }

    return 0;
}