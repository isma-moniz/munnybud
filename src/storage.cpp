/**
* @file utils.cpp
* @author Ismael Moniz (hismamoniz@gmail.com)
* @brief Implementation file for storage management and organization functions
* that interact with the json data directly
*
*/

#include "storage.hpp"
#include "utils.hpp"
#include <chrono>
#include <regex>
#include <stdexcept>
#include <fstream>
#include <iostream>

/**
* @brief Sets up a new transaction json file with the proper structure.
If, say, the wallet index returns almost every transaction because most transactions belong to the same wallet, then the benefit of intersecting that index is minimal. In that case, you might end up with a performance similar to filtering, or even a slight overhead if you perform multiple intersections.*
* @param transactionFile the path to the transaction file
* @return int -1 for error, 0 for success
*/
int StorageHandler::setupTransactions(const std::string &transactionFile) {
std::ofstream file(transactionFile);
if (!file.is_open()) {
    std::cerr << "Error opening file for writing: " << transactionFile << " ."
            << std::endl;
    return -1;
}

json data = {{"metadata", {{"currentID", 0}}}, {"data", json::object()}};

file << data.dump(4);
if (!file) {
    std::cerr << "Error writing to file: " << transactionFile << std::endl;
    return -1;
}

file.close();
return 0;
}

/**
* @brief Sets up a new wallet file with the proper structure, creating a user
* provided default wallet.
*
* @param walletFile the path to the wallet file
* @return int -1 for error, 0 for success
*/
int StorageHandler::setupWallets(const std::string &walletFile) {
std::ofstream file(walletFile);
if (!file.is_open()) {
    std::cerr << "Error opening file for writing: " << walletFile << " ."
            << std::endl;
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
    std::cout << "Invalid wallet name. Must only contain alphanumeric "
                "characters.\n";
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

/**
* @brief Loads the file contents of the provided file (path) into a
* nlohmann::basicjson<> object and returns it.
*
* @param filePath the path to the file
* @return json - nlohmann::basicjson<> object with the loaded data
*/
json StorageHandler::loadFile(const std::string &filePath) {
std::ifstream file(filePath);
json data;
if (file.is_open()) {
    try {
    data = json::parse(file);
    } catch (json::parse_error &e) {
    std::cout << "JSON parse error: " << e.what() << std::endl;
    throw std::runtime_error("JSON Parse Error");
    }
    file.close();
} else {
    json data = json::object();
}

return data;
}

/**
* @brief This function calls loadFile for both the wallet file and the
* transaction file and loads them into the appropriate member variables in the
* StorageHandler class, as well as the default wallet.
*
*/
void StorageHandler::loadData() {
wallets = loadFile(walletFile);
transactions = loadFile(transactionFile);

if (!transactions.contains("metadata") ||
    !transactions["metadata"].contains("currentID"))
    throw std::runtime_error("Transaction metadata invalid.");
Transaction::currentID = transactions["metadata"]["currentID"];

if (!wallets.contains("default_wallet"))
    throw std::runtime_error("Could not find default wallet.");

StorageHandler::default_wallet = wallets["default_wallet"];
}

/**
* @brief Stores the contents of json data in the given filePath, effectively
* updating user storage.
*
* @param filePath the path to the file
* @param data nlohmann::basic_json<> containing user data
* @return int -1 on error, 0 for success
*/
int StorageHandler::storeFile(const std::string &filePath, json &data) {
std::ofstream file(filePath);
if (!file.is_open()) {
    std::cerr << "Error: Could not open file for writing." << std::endl;
    return -1;
}
file << std::setprecision(2) << data.dump(4);
file.close();
return 0;
}

/**
* @brief Wrapper function that calls storeFile for the json data structures in
* the StorageHandler class.
*
* @return int -1 on error, 0 on success
*/
int StorageHandler::storeData() {
return (storeFile(walletFile, wallets) +
        storeFile(transactionFile, transactions)) == 1;
}

/**
* @brief Construct a new Storage Handler:: Storage Handler object, initializing
* wallet and transaction file paths.
*
* @param _walletFile wallet file path
* @param _transactionFile transaction file path
*/
StorageHandler::StorageHandler(const std::string &_walletFile,
                            const std::string &_transactionFile)
    : walletFile(_walletFile), transactionFile(_transactionFile) {
loadData();
}

/**
* @brief Stores the specified transaction in the transactions json file.
*
* @param transaction a Transaction oject to be converted to json
* @return int -1 on error, 0 on success
*/
int StorageHandler::storeTransaction(Transaction &transaction) {
std::string wlt;
if (transaction.wallet == "default")
    wlt = StorageHandler::default_wallet;
else
    wlt = transaction.wallet;

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

if (!transactions["data"].contains(transaction.date) ||
    !transactions["data"][transaction.date].is_array())
    transactions["data"][transaction.date] = json::array();
transactions["data"][transaction.date].push_back(jsonTransaction);
transactions["metadata"]["currentID"] = Transaction::currentID;
return storeData();
}

/**
* @brief populates the Id index.
* This performs a simple check to see if it is already loaded,
* and leaves the actual loading up to the IndexManager class
*/
void StorageHandler::populateIdIdx() {
if (!idxManager.isIdIdxPopulated)
    idxManager.populateIdIdx(transactions);
}

/**
* @brief populates the Wallet index.
* This performs a simple check to see if it is already loaded,
* and leaves the actual loading up to the IndexManager class
*/
void StorageHandler::populateWalletIdx() {
if (!idxManager.isWalletIdxPopulated)
    idxManager.populateWalletIdx(transactions);
}

/**
* @brief populates the Category index.
* This performs a simple check to see if it is already loaded,
* and leaves the actual loading up to the IndexManager class
*/
void StorageHandler::populateCategoryIdx() {
if (!idxManager.isCategoryIdxPopulated)
    idxManager.populateCategoryIndex(transactions);
}

/**
* @brief populates the Date unordered map index.
* This performs a simple check to see if it is already loaded,
* and leaves the actual loading up to the IndexManager class
*/
void StorageHandler::populateDateHash() {
if (!idxManager.isDateHashPopulated)
    idxManager.populateDateHash(transactions);
}

/**
* @brief populates the Date map index.
* This performs a simple check to see if it is already loaded,
* and leaves the actual loading up to the IndexManager class
*/
void StorageHandler::populateDateMap() {
if (!idxManager.isDateMapPopulated)
    idxManager.populateDateMap(transactions);
}

/**
* @brief Makes use of the id index to find a Transaction with the provided id.
* Index is loaded on demand if not loaded already by calling populateIdIdx()
* @param id
* @return Transaction& oject reference with the provided id
*/
Transaction& StorageHandler::getTransactionById(int id) {
    populateIdIdx();
    auto it = idxManager.transactionsById.find(id);
    if (it == idxManager.transactionsById.end())
        throw std::runtime_error("Transaction not found.");
    return it->second;
}

/**
* @brief Finds transactions with a certain wallet and puts them in the result
* unordered_set
*
* @param wallet - wallet to query
* @param result Transaction vector to be filled
* @return -1 on empty or no wallet, 0 on success
*/
int StorageHandler::getTransactionsByWallet(const std::string &wallet,
                                            std::unordered_set<int> &result) {
    populateWalletIdx();
    auto it = idxManager.transactionsByWallet.find(wallet);
    if (it == idxManager.transactionsByWallet.end()) {
        std::cout << "Wallet not found\n";
        return -1;
    }
    result = it->second;
    if (result.empty())
        return -1;
    return 0;
}

/**
* @brief Finds transactions with a certain category and puts them in the result
* vector
*
* @param category - category to query
* @param result Transaction vector to be filled
* @return int -1 on empty or no category, 0 on success
*/
int StorageHandler::getTransactionsByCategory(
    const std::string &category, std::unordered_set<int> &result) {
    populateCategoryIdx();
    auto it = idxManager.transactionsByCategory.find(category);
    if (it == idxManager.transactionsByCategory.end()) {
        std::cout << "Category not found\n";
        return -1;
    }
    result = it->second;
    if (result.empty())
        return -1;
    return 0;
}

/**
* @brief Finds transactions with a certain date and puts them in the result
* vector
*
* @param base_date - date to query
* @param result Transaction vector to be filled
* @return int -1 on empty, 0 on success
*/
int StorageHandler::retrieveDailyTransactions(const std::string &base_date,
                                        std::unordered_set<int> &result) {
    populateDateHash();
    auto it = idxManager.transactionsByDateHashed.find(base_date);
    if (it == idxManager.transactionsByDateHashed.end())
        return -1;
    result = it->second;
    if (result.empty())
        return -1;
    return 0;
}

/**
* @brief Finds transactions in the week of the base date
* 
* @param base_date - base_date to query
* @param result Transaction vector to be filled
* @return int -1 on empty, 0 on success
*/
int StorageHandler::retrieveWeeklyTransactions(const std::string &base_date,
                                        std::unordered_set<int> &result) {
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
        for (int transactionID : it->second) {
            result.insert(transactionID);
        }
    }
    if (result.empty())
        return -1;
    return 0;
}

/**
* @brief Finds transactions in the month of the base date
* 
* @param base_date - base_date to query
* @param result Transaction vector to be filled
* @return int -1 on empty, 0 on success
*/
int StorageHandler::retrieveMonthlyTransactions(const std::string &base_date,
                                            std::unordered_set<int> &result) {
    populateDateMap();
    std::chrono::year_month_day base_ymd = parseYMD(base_date);
    std::chrono::year_month_day startOfMonth =
        base_ymd.year() / base_ymd.month() / std::chrono::day(1);
    std::chrono::year_month_day endOfMonth =
        base_ymd.year() / base_ymd.month() / std::chrono::last;
    std::string start = formatYMD(startOfMonth);
    std::string end = formatYMD(endOfMonth);

    auto lowBound = idxManager.transactionsByDateMap.lower_bound(start);
    auto highBound = idxManager.transactionsByDateMap.upper_bound(end);

    if (lowBound == idxManager.transactionsByDateMap.end()) {
        return -1;
    }

    for (auto it = lowBound; it != highBound; ++it) {
        for (int transactionID : it->second) {
            result.insert(transactionID);
        }
    }
    if (result.empty())
        return -1;
    return 0;
}

/**
* @brief Wrapper function that retrieves the expenses based on a combined query 
* @param base_date 
* @param range 
* @param wallet
* @param category
* @param result 
* @return int 
*/
int StorageHandler::retrieveTransactions(const std::string &base_date, int range, const std::string& wallet, const std::string& category,
                                    std::unordered_map<std::string, std::vector<Transaction>> &result, const std::string& groupBy) {
    
    idxManager.populateAllIdxs(transactions); // the overhead from this is pretty much negligible at this point, but I'll fix it later

    std::unordered_set<int> walletTransactions;
    std::unordered_set<int> categoryTransactions;
    std::unordered_set<int> dateTransactions;
    std::vector<std::unordered_set<int>> setVec;
    std::string date;

    std::function<std::string(const Transaction&)> extractor;

    if (groupBy == "date")
        extractor = [](const Transaction& t) { return t.date; };
    else if (groupBy == "category")
        extractor = [](const Transaction& t) { return t.category; };
    else if (groupBy == "wallet")
        extractor = [](const Transaction& t) { return t.wallet; };
    else
        throw std::invalid_argument("Invalid groupBy parameter.");

    if (base_date.empty() && wallet.empty() && category.empty()) {
        date = getCurrentDate();
        switch (range) {
            case 1:
                retrieveDailyTransactions(date, dateTransactions);
                break;
            case 2:
                retrieveWeeklyTransactions(date, dateTransactions);
                break;
            case 3:
                retrieveMonthlyTransactions(date, dateTransactions);
                break;
            default:
                break;
        }
        for (int val : dateTransactions) {
            Transaction& transaction = getTransactionById(val);
            result[extractor(transaction)].push_back(transaction);
        }
        return 0;
    }

    if (!wallet.empty()) {
        getTransactionsByWallet(wallet, walletTransactions);
        setVec.push_back(walletTransactions);
    }
    if (!category.empty()) {
        getTransactionsByCategory(category, categoryTransactions);
        setVec.push_back(categoryTransactions);
    }
    if (!base_date.empty()) {
        switch (range) {
            case 1:
                retrieveDailyTransactions(base_date, dateTransactions);
                break;
            case 2:
                retrieveWeeklyTransactions(base_date, dateTransactions);
                break;
            case 3:
                retrieveMonthlyTransactions(base_date, dateTransactions);
                break;
            default:
                break;
        }
        setVec.push_back(dateTransactions);
    }
    std::unordered_set<int> final = idxManager.setIntersection(setVec);
    for (int val : final) {
        Transaction& transaction = getTransactionById(val);
        result[extractor(transaction)].push_back(transaction);
    }
    return 0;
}

// TODO: this function has to be redone with the indexing system. MAYBE NOT!
int StorageHandler::deleteTransaction(int id) {
for (auto &[key, values] : transactions["data"].items()) {
    for (json::iterator i = values.begin(); i != values.end(); i++) {
    if ((*i)["id"] == id) {
        if (updateBalance((*i)["wallet"],
                        -1 * static_cast<int>((*i)["amount"])) != 0) {
        std::cerr
            << "Error in transaction deletion: Could not update balance."
            << std::endl;
        return -1;
        }
        values.erase(i);
        storeData();
        return 0;
    }
    }
}
std::cerr << "Error in transaction deletion: Transaction not found."
            << std::endl;
return -1;
}

/**
* @brief Returns the balance of the provided wallet as a float
* 
* @param wallet the wallet to be queried
* @return float the balance as a float
*/
float StorageHandler::retrieveBalance(const std::string &wallet) {
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

/**
* @brief Updates the value of the provided wallet by incrementing it with 'amount'
* 
* @param wallet the wallet to be updated
* @param amount value in cents
* @return int -1 on error, 0 on success
*/
int StorageHandler::updateBalance(const std::string &wallet, int amount) {
std::string wlt;
if (wallet == "default")
    wlt = StorageHandler::default_wallet;
else
    wlt = wallet;
if (!wallets["wallets"].contains(wlt)) {
    std::cerr << "Error: Wallet '" << wlt
            << "' does not exist. Unable to update wallet balance."
            << std::endl;
    return -1;
}
std::cout << wallets["wallets"][wlt] << std::endl;
wallets["wallets"][wlt] = wallets["wallets"][wlt].get<int>() + amount;
std::cout << wallets["wallets"][wlt].get<int>() << std::endl;
return 0;
}
