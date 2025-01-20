#include "transaction.hpp"
#include "json.hpp"

Transaction::Transaction(int amt, const std::string& cat, const std::string& desc, const std::string& wlt) 
    : amount(amt), category(cat), description(desc), wallet(wlt) {}

Transaction Transaction::fromJson(const json& transactionObject) {
    return Transaction(
        transactionObject.at("amount").get<int>(),
        transactionObject.at("category").get<std::string>(),
        transactionObject.at("description").get<std::string>(),
        transactionObject.at("wallet").get<std::string>()
    );
}

json Transaction::toJson() const {
    return {
        {"amount", amount},
        {"category", category},
        {"description", description},
        {"wallet", wallet}
    };
}
