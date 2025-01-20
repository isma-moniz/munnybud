/**
 * @file utils.cpp
 * 
 * @brief Implementation file for storage management functions that interact with the json data directly
 * 
 */

#include "storage.hpp"
#include <chrono>

using json = nlohmann::json;
std::string jsonFileName = "../data.json";

int StorageHandler::loadData() {
    std::ifstream input_file(jsonFileName);
    if (input_file.is_open()) {
        try {
            data = json::parse(input_file);
        } catch (json::parse_error& e) {
            std::cout << "JSON parse error: " << e.what() << std::endl;
            return -1;
        }
        input_file.close();
    } else {
        // initialize an empty json object if the file does not exist
        json data = json::object();
    }

    if (!data.is_object()) {
        std::cerr << "Error: Root JSON structure is not an object. Resetting to empty object." << std::endl;
        data = json::object();
    }

    return 0;
}

int StorageHandler::storeData() {
    std::ofstream output_file(jsonFileName);
    if (!output_file.is_open()) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return -1;
    }
    output_file << std::setprecision(2) << data.dump(4);
    output_file.close();
    return 0;
}

StorageHandler::StorageHandler(const std::string& fileName) : jsonFileName(fileName) {
    if (loadData() != 0) {
        std::cerr << "Failed to load JSON data from '" << fileName << "'.\n";
        data = json::object();
    }
}

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
 *
 *@return 0 on success, -1 on failure
 */
int StorageHandler::storeTransaction(float amount, const std::string& category, const std::string& description, const std::string& date, const std::string& wallet) {
    float amountInCents = std::round(amount * 100); 
    // expense in json format
    json transaction = {
        {"amount", (int)amountInCents}, // convert to integer cent amount 
        {"category", category},
        {"description", description},
        {"wallet", wallet}
    };

        
    if (updateBalance(wallet, (int)amountInCents) != 0) {
        return -1;
    }
    
    if (!data.contains("transactions") || !data["transactions"].is_object()) {
        data["transactions"] = json::object();
    }

    if(!data["transactions"].contains(date) || !data["transactions"][date].is_array())
        data["transactions"][date] = json::array();
    data["transactions"][date].push_back(transaction);

    return storeData();
}

int StorageHandler::retrieveDailyExpenses(const std::string& base_date, std::vector<Transaction>& result) {
    if (data["transactions"].contains(base_date)) {
        for (auto it : data["transactions"][base_date]) {
            Transaction transaction = Transaction::fromJson(it);
            transaction.date = base_date;
            result.push_back(transaction);
        }
        return 0;
    } else {
        return -1;
    }
}

int StorageHandler::retrieveWeeklyExpenses(const std::string& base_date, std::vector<Transaction>& result) { 
    std::chrono::year_month_day baseDate = parseYMD(base_date);
    std::chrono::year_month_day startOfWeek, endOfWeek;

    getWeek(baseDate, startOfWeek, endOfWeek);
    
    for (const auto& [key, value]: data["transactions"].items()) {
        std::chrono::year_month_day currentDate = parseYMD(key);
        if (currentDate >= startOfWeek && currentDate <= endOfWeek) {
            for (const auto& expense : value) {
                Transaction transaction = Transaction::fromJson(expense);
                transaction.date = key;
                result.push_back(transaction);
            }
        }
    }
    if (empty(result)) return -1;
    return 0;
}

int StorageHandler::retrieveMonthlyExpenses(const std::string& base_date, std::vector<Transaction>& result) {
    std::chrono::year_month_day base_ymd = parseYMD(base_date);
    for (const auto& [key, value] : data["transactions"].items()) {
            std::chrono::year_month_day currentDate = parseYMD(key);
            if (same_month(base_ymd, currentDate)) {
                for (const auto& expense : value) {
                    Transaction transaction = Transaction::fromJson(expense);
                    transaction.date = key;
                    result.push_back(transaction);
                }
            }
        }
    if (empty(result)) return -1;
    return 0;
}

int StorageHandler::retrieveExpenses(const std::string& base_date, int range, std::vector<Transaction>& result) { 
    switch(range) {
        case 1:
            return retrieveDailyExpenses(base_date, result);
            break;
        case 2:
            return retrieveWeeklyExpenses(base_date, result);
            break;
        case 3:
            return retrieveMonthlyExpenses(base_date, result);
            break;
        default:
            return -1;
    }
}

/**
 * @brief Retrieves the current balance from the JSON file
 *
 * @return The balance as a float
 */
float StorageHandler::retrieveBalance(const std::string& wallet) {
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

   if (!data.contains("wallets")) {
       std::cerr << "Error: No valid wallets found in file.\n";
       return -1;
   }

   json wallets = data["wallets"];

   if (!wallets.contains(wallet)) {
       std::cerr << "Error: Wallet " << wallet << " not found.\n";
       return -1;
   }

   if (!wallets[wallet].is_number()) {
       std::cerr << "Error: Invalid wallet balance.\n";
       return -1;
   }

   return wallets[wallet].get<float>() / 100;
}

int StorageHandler::updateBalance(const std::string& wallet, int amount) {
    if (!data.contains("wallets") || !data["wallets"].is_object()) {
        data["wallets"] = json::object();
    }

    if (!data["wallets"].contains(wallet)) {
        std::cerr << "Error: Wallet '" << wallet << "' does not exist. Unable to update wallet balance." << std::endl;
        return -1;
    }
    std::cout << data["wallets"][wallet] << std::endl;
    data["wallets"][wallet] = data["wallets"][wallet].get<int>() + amount;
    std::cout << data["wallets"][wallet].get<int>() << std::endl;
    return 0;
}
