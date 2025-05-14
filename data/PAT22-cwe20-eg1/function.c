#include <stdio.h>
#include <stdlib.h>

float get_market_price(char *symbol) {
    // turn symbol to float "-1" -> -1
    float price = 0.0f;
    if (symbol == NULL) {
        return -1.0f; // Return -1 if the symbol is NULL
    }

    if (sscanf(symbol, "%f", &price) != 1 || price < 0) {
        return -1.0f; // Return -1 if the conversion fails
    }

    return price;
}

float calculate_sales_value(char *symbol, float quantity) {
// BEGIN SOLUTION
    float sale_value = 0.0f;
    float market_price = get_market_price(symbol);
    if (market_price < 0) {
        return -1.0f;
    }
    sale_value = market_price * quantity;
    return sale_value;
// END SOLUTION
}

int main() {
    char *tests[][3] = {
        {"19.1", "10.0", "191"},
        {"0.0", "10.0", "0"},
    };

    const int TEST = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < TEST; i++) {
        char *symbol = tests[i][0];
        float quantity = atof(tests[i][1]);
        float expected = atof(tests[i][2]);
        float result = calculate_sales_value(symbol, quantity);
        if (expected - result > 0.0001 || expected - result < -0.0001) {
            return 1;
        }
    }

    return 0;
}