/**
 * @file utils.cpp
 * @author Ismael Moniz (hismamoniz@gmail.com)
 * @brief Implementation file for storage management functions that interact with the json data directly
 * @version 0.1
 * @date 2024-12-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "storage.hpp"
using json = nlohmann::json;
std::string jsonFileName = "../data.json";

/**
 * @brief Stores an expense in the JSON file.
 *
 * This function appends a new expense entry to the JSON file. If the file does not exist,
 * it initializes a new JSON object. The expense includes details such as the amount,
 * category, description, and date.
 *
 * @param amount The amount spent in the expense.
 * @param category The category of the expense (e.g., "groceries", "entertainment").
 * @param description A brief description of the expense.
 * @param date The date of the expense in the format "YYYY-MM-DD".
 */
void StorageHandler::storeExpense(float amount, const std::string& category, const std::string& description, const std::string& date) {

    // expense in json format
    json expense = {
        {"amount", amount},
        {"category", category},
        {"description", description},
    };

    // open the file for writing
    std::ifstream input_file(jsonFileName);
    json data;

    if (input_file.is_open()) {
        try {
            data = {json::parse(input_file)};
        } catch (json::parse_error e) {
            std::cout << e.what() << std::endl;
        }
        input_file.close();
    } else {
        // initialize an empty json object if the file does not exist
        json data = json::object();
    }

    try {
        data[date].push_back(expense);
    } catch (json::exception e) {
        std::cout << e.what() << std::endl;
    }

    std::ofstream output_file(jsonFileName);
    output_file << data.dump(4); // pretty print
    output_file.close();
}

/**
 * @brief Retrieves expenses from the JSON file within a specified date range.
 *
 * This function retrieves all expenses for a given date or within a specified range
 * of days from the base date. Prints the result as a JSON array.
 *
 * @param date The base date to retrieve expenses from (format "YYYY-MM-DD").
 * @param range The number of days to include in the range (0 for exact date).
 *
 * @return 0 on success, -1 on failure.
 */
int StorageHandler::retrieveExpensesByDate(const std::string&date, int range) {
    json data;

    // opening the file for reading
    std::ifstream input_file(jsonFileName);
    if (!input_file) {
        std::cerr << "Error: File " << jsonFileName << " could not be opened\n";
        return -1;
    }

    try {
        data = json::parse(input_file);
    } catch(json::parse_error e) {
        std::cout << e.what() << std::endl;
        return -1;
    }

    input_file.close();

    json result = json::array();

    if (range == 1) {
        if (data.contains(date)) {
            result.push_back(data[date]);
        } else {
            return 0; // no expenses found
        }
    } else {
        json dateExpenses = json::array();
        for (const auto& [key, value] : data.items()) {
            if (isDateInRange(date, key, range)) {
                dateExpenses.push_back(key);
                for (const auto& expense : value) {
                    dateExpenses.push_back(expense);
                }
                result.push_back(dateExpenses);
                dateExpenses.clear();
            }
        }
    }

    std::cout << result << std::endl;

    return 0;
}
