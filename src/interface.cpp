#include "interface.hpp"


void printResults(const std::vector<Transaction>& results, const std::string& filter) {
    std::unordered_map<std::string, std::vector<Transaction>> groupedResults;
    groupedResults = Transaction::groupBy(results, "date");
    
    for (const auto& [key, transactions] : groupedResults) {

        std::cout << "Date: " << key << std::endl;

        for (const auto& transaction : transactions) {
            std::cout << "Amount: " << std::fixed << std::setprecision(2) << transaction.amount / 100.0 << std::endl;
            std::cout << "Category: " << transaction.category << std::endl;
            std::cout << "Description: " << transaction.description << std::endl;
            std::cout << "Wallet: " << transaction.wallet << std::endl;

        }
    }
}
