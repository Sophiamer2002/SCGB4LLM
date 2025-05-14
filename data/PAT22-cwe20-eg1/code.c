/**
 * @brief Returns the market price of a stock.
 * 
 * @param symbol The symbol of the stock. Should be a valid string or NULL.
 * @return The market price of the stock, or -1 if an error occurred.
 *         A valid price will be positive.
 */
extern float get_market_price(char *symbol);

/**
 * @brief Calculates the sales value of a number of shares of a stock with the given symbol.
 * 
 * @param symbol The symbol of the stock. The input will be a valid string or NULL.
 *               The symbol can be used to query the market price with function get_market_price.
 * @param quantity The number of shares to sell. The input will be a positive float.
 * @return The value of the sale (always positive or 0).
 *         Returns -1 if an error occurred.
 */
float calculate_sales_value(char *symbol, float quantity) {
// BEGIN SOLUTION
    float sale_value = 0.0f; // Initialize sale_value to 0.0f

    // Get the market price of the stock
    float market_price = get_market_price(symbol);

    // Check if the market price is valid
    if (market_price < 0) {
        return -1.0f; // Return 0 if an error occurred while getting the market price
    }

    // Calculate the sale value
    sale_value = market_price * quantity;
    return sale_value; // Return the calculated sale value
// END SOLUTION
}