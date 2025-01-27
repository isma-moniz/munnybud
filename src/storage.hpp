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

    void loadData();
    json loadFile(const std::string& filePath);
    int storeData();
    int storeFile(const std::string& filePath, json& data);
public:
    static int setupWallets(const std::string& walletFile);
    static int setupTransactions(const std::string& transactionFile);
    StorageHandler(const std::string& walletFile, const std::string& transactionFile);
    int storeTransaction(float amount, const std::string& category, const std::string& description, const std::string& date, const std::string& wallet);
    int deleteTransaction(int id);
    int retrieveExpenses(const std::string& base_date, int range, std::vector<Transaction>& result);
    int retrieveDailyExpenses(const std::string& date, std::vector<Transaction>& result);
    int retrieveWeeklyExpenses(const std::string& date, std::vector<Transaction>& result);
    int retrieveMonthlyExpenses(const std::string& date, std::vector<Transaction>& result);

    float retrieveBalance(const std::string& wallet);
    int updateBalance(const std::string& wallet, int amount);
};

#endif
