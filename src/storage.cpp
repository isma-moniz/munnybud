/**
* @file 
* @brief Storage handler implementation file
*
* Interacts with the JSON storage file, performing the low level operations such as storing and retrieving data
*/

#include "storage.hpp"
using json = nlohmann::json;
std::string jsonFileName = "../data.json";

/**
 * @brief StorageHandler function for storing expenses in the JSON file
 *
 * @param amount The amount spent
 * @param category The category of the expense
 * @param description Optional description of the expense
 * @param date Expense date
 *
 * Low level function to store expenses in the json file. The date is provided here but will default to the current date many times
 * in the higher level implementation
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
* @brief Storage handler function to retrieve expenses from the JSON file
*
* @param date Base date to retrieve expenses from
* @param ranger Retrieve transactions in this span from the base date
* @param result json object to store the result in
*/
int StorageHandler::retrieveExpensesByDate(const std::string&date, int range, json& result) {
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

    result = json::array();

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

    return 0;
}
