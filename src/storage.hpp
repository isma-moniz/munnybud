/**
 * @file utils.cpp
 * @author Ismael Moniz (hismamoniz@gmail.com)
 * @brief Header file for storage management functions that interact with the json data directly
 * @version 0.1
 * @date 2024-12-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef STORAGE_HPP
#define STORAGE_HPP

#include "utils.hpp"
#include "transaction.hpp"

#include <string>
#include <stdio.h>
#include "json.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <ctime>

using json = nlohmann::json;

/**
* @class
* @brief Storage Handler class
*
*/
class StorageHandler {
private:
    json transactions;
    json wallets;
    std::string walletFile;
    std::string transactionFile; 
    static std::string default_wallet;
    
    // indexes computed at runtime
    std::unordered_map<int, Transaction> transactionsById;
    std::unordered_map<std::string, std::vector<int>> transactionsByWallet;
    std::unordered_map<std::string, std::vector<int>> transactionsByCategory;

    void loadData();
    json loadFile(const std::string& filePath);
    int storeData();
    int storeFile(const std::string& filePath, json& data);
public:
    StorageHandler(const std::string& walletFile, const std::string& transactionFile);

    static int setupWallets(const std::string& walletFile);
    static int setupTransactions(const std::string& transactionFile);
    
    int storeTransaction(Transaction& transaction);
    int deleteTransaction(int id);

    Transaction& getTransactionById(int id);
    const std::vector<int>& getTransactionsByCategory(const std::string& category) const;
    const std::vector<int>& getTransactionsByWallet(const std::string& wallet) const;

    int retrieveExpenses(const std::string& base_date, int range, std::vector<Transaction>& result);
    int retrieveDailyExpenses(const std::string& date, std::vector<Transaction>& result);
    int retrieveWeeklyExpenses(const std::string& date, std::vector<Transaction>& result);
    int retrieveMonthlyExpenses(const std::string& date, std::vector<Transaction>& result);

    float retrieveBalance(const std::string& wallet);
    int updateBalance(const std::string& wallet, int amount);
};

#endif
