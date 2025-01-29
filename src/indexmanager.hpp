#ifndef INDEXMANAGER_HPP
#define INDEXMANAGER_HPP

#include <unordered_map>
#include <map>
#include "transaction.hpp"
#include "json.hpp"

using json = nlohmann::json;

class IndexManager {
public:
    std::unordered_map<int, Transaction> transactionsById;
    std::unordered_map<std::string, std::vector<int>> transactionsByWallet;
    std::unordered_map<std::string, std::vector<int>> transactionsByCategory;
    std::unordered_map<std::string, std::vector<int>> transactionsByDateHashed;
    std::map<std::string, std::vector<int>> transactionsByDateMap;

    bool isIdIdxPopulated = false;
    bool isWalletIdxPopulated = false;
    bool isCategoryIdxPopulated = false;
    bool isDateHashPopulated = false;
    bool isDateMapPopulated = false;

    void populateIdIdx(json& transactions);
    void populateWalletIdx(json& transactions);
    void populateCategoryIndex(json& transactions);
    void populateDateHash(json& transactions);
    void populateDateMap(json& transactions);
    void populateAllIdxs(json& transactions);
};

#endif
