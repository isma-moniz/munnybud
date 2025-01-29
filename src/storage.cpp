/**
 * @file utils.cpp
 *
 * @brief Implementation file for storage management functions that interact with the json data directly
 *
 */

#include "storage.hpp"
#include <chrono>
#include <regex>
#include <stdexcept>

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
    // loadAllIndexes, if running in gui mode
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
    json jsonTransaction = transaction.toJson();

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

void StorageHandler::populateIdIdx() {
    if (!idxManager.isIdIdxPopulated)
        idxManager.populateIdIdx(transactions);
}

void StorageHandler::populateWalletIdx() {
    if (!idxManager.isWalletIdxPopulated)
        idxManager.populateWalletIdx(transactions);
}

void StorageHandler::populateCategoryIdx() {
    if (!idxManager.isCategoryIdxPopulated)
        idxManager.populateCategoryIndex(transactions);
}

void StorageHandler::populateDateHash() {
    if (!idxManager.isDateHashPopulated)
        idxManager.populateDateHash(transactions);
}

void StorageHandler::populateDateMap() {
    if (!idxManager.isDateMapPopulated)
        idxManager.populateDateMap(transactions);
}

Transaction& StorageHandler::getTransactionById(int id) {
    populateIdIdx(); 
    auto it = idxManager.transactionsById.find(id);
    if (it == idxManager.transactionsById.end())
        throw std::runtime_error("Transaction not found.");
    return it->second;
}

int StorageHandler::getTransactionsByWallet(const std::string& wallet, std::vector<Transaction>& result) {
    populateWalletIdx();
    auto it = idxManager.transactionsByWallet.find(wallet);
    if (it == idxManager.transactionsByWallet.end())
        throw std::runtime_error("Wallet not found.");
    for (auto& transactionID : it->second) {
        try {
            result.push_back(getTransactionById(transactionID));
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << '\n';
            return -1;
        }
    }
    if (result.empty()) return -1;
    return 0;
}

int StorageHandler::getTransactionsByCategory(const std::string& category, std::vector<Transaction>& result) {
    populateCategoryIdx();
    auto it = idxManager.transactionsByCategory.find(category);
    if (it == idxManager.transactionsByCategory.end())
        throw std::runtime_error("Transaction not found");
    for (auto& transactionID : it->second) {
        try {
            result.push_back(getTransactionById(transactionID));
        } catch(std::runtime_error& e) {
            std::cerr << e.what() << '\n';
            return -1;
        }
    }
    if (result.empty()) return -1;
    return 0;
}

int StorageHandler::retrieveDailyExpenses(const std::string& base_date, std::vector<Transaction>& result) {
    populateDateHash();
    auto it = idxManager.transactionsByDateHashed.find(base_date);
    if (it == idxManager.transactionsByDateHashed.end())
        return -1;
    else {
        try {
            for (int id : it->second) {
                result.push_back(getTransactionById(id));
            }
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << '\n';
            return -1;
        }
    }
    return 0;
}

int StorageHandler::retrieveWeeklyExpenses(const std::string& base_date, std::vector<Transaction>& result) {
    populateDateMap();
    std::chrono::year_month_day baseDate = parseYMD(base_date);
    std::chrono::year_month_day startOfWeek, endOfWeek;

    getWeek(baseDate, startOfWeek, endOfWeek);
    std::string start = formatYMD(startOfWeek);
    std::string end = formatYMD(endOfWeek);
    auto lowBound = idxManager.transactionsByDateMap.lower_bound(start);
    auto highBound = idxManager.transactionsByDateMap.upper_bound(end);

    if (lowBound == idxManager.transactionsByDateMap.end()) {
        return -1; // No transactions in the given range
    }

    for (auto it = lowBound; it != highBound; ++it) {
        try {
            for (int transactionID : it->second) {
                result.push_back(getTransactionById(transactionID));
            }
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << '\n';
            return -1;
        } 
    }
    if (result.empty()) return -1;
    return 0;
}

int StorageHandler::retrieveMonthlyExpenses(const std::string& base_date, std::vector<Transaction>& result) {
    populateDateMap();
    std::chrono::year_month_day base_ymd = parseYMD(base_date);
    std::chrono::year_month_day startOfMonth = base_ymd.year() / base_ymd.month() / std::chrono::day(1);
    std::chrono::year_month_day endOfMonth = base_ymd.year() / base_ymd.month() / std::chrono::last;
    std::string start = formatYMD(startOfMonth);
    std::string end = formatYMD(endOfMonth);

    auto lowBound = idxManager.transactionsByDateMap.lower_bound(start);
    auto highBound = idxManager.transactionsByDateMap.upper_bound(end);

    if (lowBound == idxManager.transactionsByDateMap.end()) {
        return -1;
    }
    
    for (auto it = lowBound; it != highBound; ++it) {
        try{
            for (int transactionID : it->second) {
                result.push_back(getTransactionById(transactionID));
            }        
        } catch (std::runtime_error& e) {
                std::cerr << e.what() << '\n';
                return -1;
        }
    }
    if (result.empty()) return -1;
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
    std::string wlt;
    if  (wallet == "default")
        wlt = StorageHandler::default_wallet;
    else wlt = wallet;
    if (!wallets["wallets"].contains(wlt)) {
        std::cerr << "Error: Wallet '" << wlt << "' does not exist. Unable to update wallet balance." << std::endl;
        return -1;

    } 
    std::cout << wallets["wallets"][wlt] << std::endl;
    wallets["wallets"][wlt] = wallets["wallets"][wlt].get<int>() + amount;
    std::cout << wallets["wallets"][wlt].get<int>() << std::endl;
    return 0;
}
