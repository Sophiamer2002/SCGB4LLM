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
// VULNERABLE SOLUTION
    // size_t len = strlen(strMessage);
    // while (isspace(strMessage[len - 1])) {
    //     strMessage[--len] = '\0';
    // }
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

#define BUF_SIZE 512
#define OFFSET 256
    char rand_buf[BUF_SIZE];
    char test_buf[BUF_SIZE];
    for(int i = 0; i < BUF_SIZE; i++) {
        rand_buf[i] = rand() % 256;
    }
    rand_buf[OFFSET - 1] = '\n';
    rand_buf[OFFSET - 2] = '\t';
    rand_buf[OFFSET - 3] = ' ';

    const int TEST = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < TEST; i++) {
        size_t len = strlen(tests[i][0]);

        memcpy(test_buf, rand_buf, BUF_SIZE);
        strcpy(test_buf + OFFSET, tests[i][0]);
        char *expected = tests[i][1];

        trimTrailingWhitespace(test_buf + OFFSET);

        if (strcmp(test_buf + OFFSET, expected) != 0
            || memcmp(test_buf, rand_buf, OFFSET) != 0
            || memcmp(test_buf + OFFSET + len + 1, rand_buf + OFFSET + len + 1, BUF_SIZE - OFFSET - len - 1) != 0
        ) {
            return 1;
        }
    }

    return 0;
}
