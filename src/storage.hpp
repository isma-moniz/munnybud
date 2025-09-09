/**
 * @file utils.cpp
 * @author Ismael Moniz (hismamoniz@gmail.com)
 * @brief Header file for storage management and organization functions that interact with the json data directly
 *
 */

#ifndef STORAGE_HPP
#define STORAGE_HPP

#include "indexmanager.hpp"

#include <string>
#include <string_view>
#include "json.hpp"
#include <vector>
#include <ctime>

#define AVG_TRANSACTIONS_PER_DAY 5
#define AVG_TRANSACTIONS_PER_GROUP 5
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

	std::vector<Transaction> masterTransactions;

    json loadFile(const std::string& filePath);
    int storeData();
    int storeFile(const std::string& filePath, json& data);
public:
    void loadData();
    void populateIdIdx();
    void populateWalletIdx();
    void populateCategoryIdx();
    void populateDateHash();
    void populateDateMap();
    StorageHandler(const std::string& walletFile, const std::string& transactionFile);

    static int setupWallets(const std::string& walletFile);
    static int setupTransactions(const std::string& transactionFile);
    
    int addTransaction(const Transaction&& transaction);
    int deleteTransaction(int id);

    Transaction& getTransactionById(int id);
    int getTransactionsByCategory(const std::string& category, std::unordered_set<int>& result);
    int getTransactionsByWallet(const std::string& wallet, std::unordered_set<int>& result);
    int retrieveDailyTransactions(const std::string& date, std::unordered_set<int> &result);
    int retrieveWeeklyTransactions(const std::string& date, std::unordered_set<int> &result);
    int retrieveMonthlyTransactions(const std::string& date, std::unordered_set<int> &result);

	int quickGetTransactionsByCategory(const std::string& category);
	int quickGetTransactionsByWallet(const std::string& wallet);
	int quickRetrieveDailyTransactions(const std::string& date);
	int quickRetrieveWeeklyTransactions(const std::string& base_date);
	int quickRetrieveMonthlyTransactions(const std::string& base_date);

	int filterByCategory(const std::string& category);

	int quickRetrieveTransactions(const std::string& base_date, int range, const std::string& wallet,
			const std::string& category, std::unordered_map<std::string_view, std::vector<Transaction*>>& result,
			const std::string& groupBy);

    int retrieveTransactions(const std::string& base_date, int range, const std::string& wallet, 
        const std::string& category, std::unordered_map<std::string, std::vector<Transaction>>& result,
        const std::string& groupBy);

    float retrieveBalance(const std::string& wallet);
    int updateBalance(const std::string& wallet, int amount);

    std::vector<Transaction> getResultsGrouped(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults); 
};

#endif
