#include "transaction.hpp"
#include "json.hpp"

Transaction::Transaction(int amt, const std::string& cat, const std::string& lbl, const std::string& wlt) 
    : amount(amt), category(cat), label(lbl), wallet(wlt) {}

Transaction Transaction::fromJson(const json& transactionObject) {
    return Transaction(
        transactionObject.at("amount").get<int>(),
        transactionObject.at("category").get<std::string>(),
        transactionObject.at("label").get<std::string>(),
        transactionObject.at("wallet").get<std::string>()
    );
}

json Transaction::toJson() const {
    return {
        {"amount", amount},
        {"category", category},
        {"label", label},
        {"wallet", wallet}
    };
}
