#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include "json.hpp"
#include <unordered_map>

using json = nlohmann::json;

class Transaction {
public:
    static int currentID;
    int id;
    int amount;
    std::string category;
    std::string description;
    std::string wallet;
    std::string date;

    Transaction(int amt, const std::string& cat, const std::string& desc, const std::string& wlt);
    Transaction(const json& transactionObject);
    json toJson() const;

    static std::unordered_map<std::string, std::vector<Transaction>> groupBy(const std::vector<Transaction>& transactions, const std::string& property);
};
#endif
