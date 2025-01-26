/**
 * @file utils.cpp
 *
 * @brief Implementation file for storage management functions that interact with the json data directly
 *
 */

#include "storage.hpp"
#include <chrono>
#include <iterator>

std::string transactionFilePath = "../transactions.json";
std::string walletsFilePath = "../wallets.json";

json StorageHandler::loadFile(const std::string& filePath) {
    std::ifstream file(filePath);
    json data;
    if (file.is_open()) {
        try {
            data = json::parse(file);
        } catch (json::parse_error& e) {
            std::cout << "JSON parse error: " << e.what() << std::endl;
            throw std::runtime_error("JSON Parse Error");
        }
        file.close();
    } else {
        json data = json::object();
    }

    return data;
}

void StorageHandler::loadData() { 
    wallets = loadFile(walletFile);
    transactions = loadFile(transactionFile);
    if (!transactions.contains("metadata") || !transactions["metadata"].contains("currentID")) throw std::runtime_error("Transaction metadata invalid.");
    Transaction::currentID = transactions["metadata"]["currentID"];
}

int StorageHandler::storeFile(const std::string& filePath, json& data) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return -1;
    }
    file << std::setprecision(2) << data.dump(4);
    file.close();
    return 0;
}

int StorageHandler::storeData() {
    return (storeFile(walletFile, wallets) + storeFile(transactionFile, transactions)) == 1;
}

StorageHandler::StorageHandler(const std::string& _walletFile, const std::string& _transactionFile) : walletFile(_walletFile), transactionFile(_transactionFile) {
    loadData(); 
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
        {"id", ++Transaction::currentID},
        {"amount", (int)amountInCents}, // convert to integer cent amount
        {"category", category},
        {"description", description},
        {"wallet", wallet}
    };

    if (updateBalance(wallet, (int)amountInCents) != 0) {
        Transaction::currentID--;
        return -1;
    }

    if (!transactions.contains("data") || !transactions["data"].is_object()) {
        transactions["data"] = json::object();
    }

    if(!transactions["data"].contains(date) || !transactions["data"][date].is_array())
        transactions["data"][date] = json::array();
    transactions["data"][date].push_back(transaction);
    transactions["metadata"]["currentID"] = Transaction::currentID;
    return storeData();
}

int StorageHandler::retrieveDailyExpenses(const std::string& base_date, std::vector<Transaction>& result) {
    if (transactions["data"].contains(base_date)) {
        for (auto it : transactions["data"][base_date]) {
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

    for (const auto& [key, value]: transactions["data"].items()) {
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
    for (const auto& [key, value] : transactions["data"].items()) {
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

int StorageHandler::deleteTransaction(int id) {
    for (auto& [key, values] : transactions["data"].items()) {
        for (json::iterator i = values.begin(); i != values.end(); i++) {
            if ((*i)["id"] == id) {
                if (updateBalance((*i)["wallet"], -1 * (int)(*i)["amount"]) != 0) {
                    std::cerr << "Error in transaction deletion: Could not update balance." << std::endl;
                    return -1;
                }
                values.erase(i);
                storeData();
                return 0;
            }
        }
    }
    std::cerr << "Error in transaction deletion: Transaction not found." << std::endl;
    return -1;
}

/**
 * @brief Retrieves the current balance from the JSON file
 *
 * @return The balance as a float
 */
float StorageHandler::retrieveBalance(const std::string& wallet) {
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
    if (!wallets.contains(wallet)) {
        std::cerr << "Error: Wallet '" << wallet << "' does not exist. Unable to update wallet balance." << std::endl;
        return -1;

    } 
    std::cout << wallets[wallet] << std::endl;
    wallets[wallet] = wallets[wallet].get<int>() + amount;
    std::cout << wallets[wallet].get<int>() << std::endl;
    return 0;
}
