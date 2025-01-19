#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include <iostream>
#include "json.hpp"
using json = nlohmann::json;

class Transaction {
public:
    int amount;
    std::string category;
    std::string label;
    std::string wallet;
    // consider adding a date as well

    Transaction(int amt, const std::string& cat, const std::string& lbl, const std::string& wlt);
    static Transaction fromJson(const json& transactionObject);
    json toJson() const;
};
#endif
