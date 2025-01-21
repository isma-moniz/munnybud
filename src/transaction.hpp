#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include <iostream>
#include "json.hpp"
#include <unordered_map>

using json = nlohmann::json;

class Transaction {
public:
    int amount;
    std::string category;
    std::string description;
    std::string wallet;
    std::string date;
    // consider adding a date as well

    Transaction(int amt, const std::string& cat, const std::string& desc, const std::string& wlt);

    static Transaction fromJson(const json& transactionObject);
    json toJson() const;

    static std::vector<Transaction> filter(const std::vector<Transaction>& transactions, const std::function<bool(const Transaction&)>& predicate);
    static std::unordered_map<std::string, std::vector<Transaction>> groupBy(const std::vector<Transaction>& transactions, const std::string& property);

};
#endif
