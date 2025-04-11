#include "indexmanager.hpp"

/**
 * @brief fallback function
 * 
 * @param transactions 
 */
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

/**
 * @brief fallback function
 * 
 * @param transactions 
 */
void IndexManager::populateWalletIdx(json& transactions) {
    transactionsById.clear();
    transactionsByWallet.clear();
    for (const auto& [date, txList] : transactions["data"].items()) {
        for (const auto& tx : txList) {
            Transaction txObj(tx);
            txObj.date = date;
            transactionsById.try_emplace(txObj.id, txObj);
            transactionsByWallet[txObj.wallet].insert(txObj.id);
        }
    }
    isIdIdxPopulated = true;
    isWalletIdxPopulated = true;
}

/**
 * @brief fallback function
 * 
 * @param transactions 
 */
void IndexManager::populateCategoryIndex(json& transactions) {
    transactionsById.clear();
    transactionsByCategory.clear();
    for (const auto& [date, txList] : transactions["data"].items()) {
        for (const auto& tx : txList) {
            Transaction txObj(tx);
            txObj.date = date;
            transactionsById.try_emplace(txObj.id, txObj);
            transactionsByCategory[txObj.category].insert(txObj.id);
        }
    }
    isIdIdxPopulated = true;
    isCategoryIdxPopulated = true;
}

/**
 * @brief fallback function
 * 
 * @param transactions 
 */
void IndexManager::populateDateHash(json& transactions) {
    transactionsById.clear();
    transactionsByDateHashed.clear();
    for (const auto& [date, txList] : transactions["data"].items()) {
        for (const auto& tx : txList) {
            Transaction txObj(tx);
            txObj.date = date;
            transactionsById.try_emplace(txObj.id, txObj);
            transactionsByDateHashed[txObj.date].insert(txObj.id);
        }
    }
    isIdIdxPopulated = true;
    isDateHashPopulated = true;
}

/**
 * @brief fallback function
 * 
 * @param transactions 
 */
void IndexManager::populateDateMap(json& transactions) {
    transactionsById.clear();
    transactionsByDateMap.clear();
    for (const auto& [date, txList] : transactions["data"].items()) {
        for (const auto& tx : txList) {
            Transaction txObj(tx);
            txObj.date = date;
            transactionsById.try_emplace(txObj.id, txObj);
            transactionsByDateMap[txObj.date].insert(txObj.id);
        }
    }
    isIdIdxPopulated = true;
    isDateMapPopulated = true;
}

/**
 * @brief populates ALL indexes
 * 
 * @param transactions 
 */
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
            transactionsByCategory[txObj.category].insert(txObj.id);
            transactionsByWallet[txObj.wallet].insert(txObj.id);
            transactionsByDateHashed[txObj.date].insert(txObj.id);
            transactionsByDateMap[txObj.date].insert(txObj.id);
        }
    }
    isIdIdxPopulated = true;
    isWalletIdxPopulated = true;
    isCategoryIdxPopulated = true;
    isDateHashPopulated = true;
    isDateMapPopulated = true;
}

/**
 * @brief Computes the intersection of two unordered sets.
 *
 * This function compares two sets and returns a new unordered set
 * containing only the elements present in both.
 * It optimizes performance by iterating over the smaller set.
 *
 * @param a The first unordered set.
 * @param b The second unordered set.
 * @return A new unordered set containing the intersection of sets a and b.
 */
std::unordered_set<int> IndexManager::twoSetIntersection(const std::unordered_set<int>& a, const std::unordered_set<int>& b) {
    std::unordered_set<int> result;

    const auto& smaller = a.size() < b.size() ? a : b;
    const auto& larger = a.size() > b.size() ? a : b;

    for (int val : smaller) {
        if (larger.count(val)) {
            result.insert(val);
        }
    }

    return result;
}

/**
 * @brief Computes the intersection of multiple unordered sets.
 *
 * Given a vector of unordered sets, this function returns a new set
 * containing only the elements common to all input sets.
 * It performs pairwise intersections starting from the first set.
 * It is NOT optimal. ordering the sets from smallest to biggest would be optimal !!!!!
 *
 * @param sets A vector containing the unordered sets to intersect.
 * @return A new unordered set representing the common elements among all sets.
 */
std::unordered_set<int> IndexManager::setIntersection(const std::vector<std::unordered_set<int>>& sets) {
    if (sets.empty()) return {};
    size_t nr_sets = sets.size();

    std::unordered_set<int> result = sets[0];
    if (nr_sets == 1) return result;

    for (size_t i = 1; i < nr_sets; i++) {
        result = twoSetIntersection(result, sets[i]);
    }

    return result;
}
