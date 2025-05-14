#include <stdio.h>
#include <stdlib.h>

char *last_deal_with_argument = NULL;
void deal_with_argument(char *arg) {
    last_deal_with_argument = arg;
}

int test_fun(int argc, char *argv[]) {
// BEGIN SOLUTION
    if (argc < 2) {
        return 1;
    }

    int index = atoi(argv[1]);
    if (index <= 0 || index >= argc - 1) {
        return 1;
    }

    char *arg = argv[index + 1];
    deal_with_argument(arg);
    return 0;
// VULNERABLE SOLUTION
    // int index = atoi(argv[1]);
    // char *arg = argv[index + 1];
    // deal_with_argument(arg);
    // return 0;
// END SOLUTION
}

int main() {
    char *tests[] = {
        "150", "-2"
    };

    const int TEST = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < TEST; i++) {
        char *argv[100];
        argv[0] = "program";
        argv[1] = tests[i];
        int argc = rand() % 98 + 2;
        for (int j = 2; j < argc; j++) {
            argv[j] = (char *)(size_t)rand();
        }

        int ret = test_fun(argc, argv);
        int expected_arg = atoi(tests[i]);
        if (ret != 1) {
            return 1;
        }

        if (last_deal_with_argument != NULL) {
            return 1;
        }
    }

    return 0;
}
