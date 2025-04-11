#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include "json.hpp"

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
};
#endif
