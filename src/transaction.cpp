#include "transaction.hpp"
#include "json.hpp"

Transaction::Transaction(int amt, const std::string& cat, const std::string& desc, const std::string& wlt) 
    : amount(amt), category(cat), description(desc), wallet(wlt) {
        id = ++Transaction::currentID;
    }

Transaction::Transaction(const json& transactionObject) {
    id = transactionObject.at("id").get<int>();
    amount = transactionObject.at("amount").get<int>();
    category = transactionObject.at("category").get<std::string>();
    description = transactionObject.at("description").get<std::string>();
    wallet = transactionObject.at("wallet").get<std::string>();
}

json Transaction::toJson() const {
    return {
        {"id", id},
        {"amount", amount},
        {"category", category},
        {"description", description},
        {"wallet", wallet}
    };
}
