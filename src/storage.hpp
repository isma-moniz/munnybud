/**
 * @file utils.cpp
 * @author Ismael Moniz (hismamoniz@gmail.com)
 * @brief Header file for storage management and organization functions that interact with the json data directly
 *
 */

#ifndef STORAGE_HPP
#define STORAGE_HPP

#include "utils.hpp"
#include "indexmanager.hpp"

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
    IndexManager idxManager; 

    void loadData();
    json loadFile(const std::string& filePath);
    int storeData();
    int storeFile(const std::string& filePath, json& data);
public:
    void populateIdIdx();
    void populateWalletIdx();
    void populateCategoryIdx();
    void populateDateHash();
    void populateDateMap();
    StorageHandler(const std::string& walletFile, const std::string& transactionFile);

    static int setupWallets(const std::string& walletFile);
    static int setupTransactions(const std::string& transactionFile);
    
    int storeTransaction(Transaction& transaction);
    int deleteTransaction(int id);

    Transaction& getTransactionById(int id);
    int getTransactionsByCategory(const std::string& category, std::unordered_set<int>& result);
    int getTransactionsByWallet(const std::string& wallet, std::unordered_set<int>& result);
    int retrieveDailyTransactions(const std::string& date, std::unordered_set<int> &result);
    int retrieveWeeklyTransactions(const std::string& date, std::unordered_set<int> &result);
    int retrieveMonthlyTransactions(const std::string& date, std::unordered_set<int> &result);

    int retrieveTransactions(const std::string& base_date, int range, const std::string& wallet, const std::string& category, std::vector<Transaction>& result);

    float retrieveBalance(const std::string& wallet);
    int updateBalance(const std::string& wallet, int amount);
};

#endif
