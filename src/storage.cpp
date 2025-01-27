/**
 * @file utils.cpp
 *
 * @brief Implementation file for storage management functions that interact with the json data directly
 *
 */

#include "storage.hpp"
#include <chrono>
#include <regex>
#include <iterator>

std::string transactionFilePath = "../transactions.json";
std::string walletsFilePath = "../wallets.json";

int StorageHandler::setupTransactions(const std::string& transactionFile) {
    std::ofstream file(transactionFile);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << transactionFile << " ." << std::endl;
        return -1;
    }

    json data = {
        {"metadata", { { "currentID", 0 } }},
        {"data", json::object()}
    };
    
    file << data.dump(4);
    if (!file) {
        std::cerr << "Error writing to file: " << transactionFile << std::endl;
        return -1;
    }
    
    file.close();
    return 0;
}

int StorageHandler::setupWallets(const std::string& walletFile) {
    std::ofstream file(walletFile);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << walletFile << " ." << std::endl;
        return -1;
    }

    json data = json::object();
    float walletMoney;
    std::string walletName;
    
    while (true) {
        std::cout << "Enter name of default wallet (leave empty for 'default'): ";
        std::getline(std::cin, walletName);
        if (walletName.empty()) {
            walletName = "default";
            break;
        }

        std::regex alphaNum("^[a-zA-Z0-9]+$");
        if (std::regex_match(walletName, alphaNum)) {
            break;
        } else {
            std::cout << "Invalid wallet name. Must only contain alphanumeric characters.\n";
        }
    }

    std::cout << "Enter amount of money in default wallet: ";
    while (!(std::cin >> walletMoney)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max());
        std::cout << "Invalid amount. Please enter a number: ";
    }

    data["default_wallet"] = walletName; 
    data["wallets"] = json::object();
    data["wallets"][walletName] = static_cast<int>(std::round(walletMoney * 100));

    file << data.dump(4);
    if (!file) {
        std::cerr << "Error writing to file: " << walletFile << std::endl;
        return -1;
    }

    file.close();
    return 0;
}

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

    if (!wallets.contains("default_wallet")) throw std::runtime_error("Could not find default wallet.");
    StorageHandler::default_wallet = wallets["default_wallet"];

    transactionsById.clear();
    for (const auto& [date, txList] : transactions.items()) {
        for (const auto& tx : txList) {
            Transaction txObj(tx);
            txObj.date = date;
            transactionsById[txObj.id] = txObj;
        }
    }
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
int StorageHandler::storeTransaction(Transaction& transaction) {
    std::string wlt;
    if (transaction.wallet == "default")
        wlt = StorageHandler::default_wallet;
    else wlt = transaction.wallet;
    
    // expense in json format
    json jsonTransaction = transaction.toJson(); //TODO figure out id issue

    if (updateBalance(wlt, jsonTransaction["amount"]) != 0) {
        Transaction::currentID--;
        return -1;
    }

    if (!transactions.contains("data") || !transactions["data"].is_object()) {
        Transaction::currentID--;
        std::cerr << "Invalid file structure: couldn't find 'data' object.\n";
        return -1;
    }

    if(!transactions["data"].contains(transaction.date) || !transactions["data"][transaction.date].is_array())
        transactions["data"][transaction.date] = json::array();
    transactions["data"][transaction.date].push_back(jsonTransaction);
    transactions["metadata"]["currentID"] = Transaction::currentID;
    return storeData();
}

int StorageHandler::retrieveDailyExpenses(const std::string& base_date, std::vector<Transaction>& result) {
    if (transactions["data"].contains(base_date)) {
        for (auto it : transactions["data"][base_date]) {
            Transaction transaction(it);
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
                Transaction transaction(expense);
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
                    Transaction transaction(expense);
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
                if (updateBalance((*i)["wallet"], -1 * static_cast<int>((*i)["amount"])) != 0) {
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
   if (!wallets["wallets"].contains(wallet)) {
       std::cerr << "Error: Wallet " << wallet << " not found.\n";
       return -1;
   }

   if (!wallets["wallets"][wallet].is_number()) {
       std::cerr << "Error: Invalid wallet balance.\n";
       return -1;
   }

   return wallets["wallets"][wallet].get<float>() / 100;
}

int StorageHandler::updateBalance(const std::string& wallet, int amount) {
    if (!wallets["wallets"].contains(wallet)) {
        std::cerr << "Error: Wallet '" << wallet << "' does not exist. Unable to update wallet balance." << std::endl;
        return -1;

    } 
    std::cout << wallets["wallets"][wallet] << std::endl;
    wallets["wallets"][wallet] = wallets["wallets"][wallet].get<int>() + amount;
    std::cout << wallets["wallets"][wallet].get<int>() << std::endl;
    return 0;
}
