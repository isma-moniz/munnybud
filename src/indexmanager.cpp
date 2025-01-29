#include "indexmanager.hpp"

// for now it is okay to have each index separately populated 
// however, if you find yourself calling more than one populate function,
// then you will be repeating the iteration loop which is unnecessary.
// if that happens, make another function that takes in all the indexes you need.
void IndexManager::populateIdIdx(json& transactions) {
    transactionsById.clear();
    for (const auto& [date, txList] : transactions["data"].items()) {
        for (const auto& tx : txList) {
            Transaction txObj(tx);
            txObj.date = date;
            transactionsById.try_emplace(txObj.id, txObj);
        }
    }
    isIdIdxPopulated = true;
}

void IndexManager::populateWalletIdx(json& transactions) {
    transactionsById.clear();
    transactionsByWallet.clear();
    for (const auto& [date, txList] : transactions["data"].items()) {
        for (const auto& tx : txList) {
            Transaction txObj(tx);
            txObj.date = date;
            transactionsById.try_emplace(txObj.id, txObj);
            transactionsByWallet[txObj.wallet].push_back(txObj.id);
        }
    }
    isIdIdxPopulated = true;
    isWalletIdxPopulated = true;
}

void IndexManager::populateCategoryIndex(json& transactions) {
    transactionsById.clear();
    transactionsByCategory.clear();
    for (const auto& [date, txList] : transactions["data"].items()) {
        for (const auto& tx : txList) {
            Transaction txObj(tx);
            txObj.date = date;
            transactionsById.try_emplace(txObj.id, txObj);
            transactionsByCategory[txObj.category].push_back(txObj.id);
        }
    }
    isIdIdxPopulated = true;
    isCategoryIdxPopulated = true;
}

void IndexManager::populateDateHash(json& transactions) {
    transactionsById.clear();
    transactionsByDateHashed.clear();
    for (const auto& [date, txList] : transactions["data"].items()) {
        for (const auto& tx : txList) {
            Transaction txObj(tx);
            txObj.date = date;
            transactionsById.try_emplace(txObj.id, txObj);
            transactionsByDateHashed[txObj.date].push_back(txObj.id);
        }
    }
    isIdIdxPopulated = true;
    isDateHashPopulated = true;
}

void IndexManager::populateDateMap(json& transactions) {
    transactionsById.clear();
    transactionsByDateMap.clear();
    for (const auto& [date, txList] : transactions["data"].items()) {
        for (const auto& tx : txList) {
            Transaction txObj(tx);
            txObj.date = date;
            transactionsById.try_emplace(txObj.id, txObj);
            transactionsByDateMap[txObj.date].push_back(txObj.id);
        }
    }
    isIdIdxPopulated = true;
    isDateMapPopulated = true;
}

void IndexManager::populateAllIdxs(json& transactions) {
    transactionsById.clear();
    transactionsByWallet.clear();
    transactionsByCategory.clear();
    transactionsByDateHashed.clear();
    transactionsByDateMap.clear();
    for (const auto& [date, txList] : transactions["data"].items()) {
        for (const auto& tx : txList) {
            Transaction txObj(tx);
            txObj.date = date;
            transactionsById.try_emplace(txObj.id, txObj);
            transactionsByCategory[txObj.category].push_back(txObj.id);
            transactionsByWallet[txObj.wallet].push_back(txObj.id);
            transactionsByDateHashed[txObj.date].push_back(txObj.id);
            transactionsByDateMap[txObj.date].push_back(txObj.id);
        }
    }
    isIdIdxPopulated = true;
    isWalletIdxPopulated = true;
    isCategoryIdxPopulated = true;
    isDateHashPopulated = true;
    isDateMapPopulated = true;
}
