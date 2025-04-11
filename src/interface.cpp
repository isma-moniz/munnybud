#include "interface.hpp"
#include <cctype>
#include <stdexcept>

void printResults(std::vector<Transaction>& results, const std::string& groupBy) {
    std::unordered_map<std::string, std::vector<Transaction>> groupedResults;    

    groupedResults = Transaction::groupBy(results, groupBy);
    
    if (groupBy == "date")
        printGroupedByDate(groupedResults);
    else if (groupBy == "category")
        printGroupedByCategory(groupedResults);
    else if (groupBy == "wallet")
        printGroupedByWallet(groupedResults);
    else throw std::invalid_argument("Invalid groupBy argument.");
}

void printGroupedByDate(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults) {
    std::cout << "Expenses grouped by date: " << std::endl << std::endl;
    for (const auto& [key, transactions] : groupedResults) {
        std::cout << "Date: " << key << std::endl << std::endl;
        for (const auto& transaction : transactions) {
            std::cout << "ID: " << transaction.id << std::endl;
            std::cout << "Amount: " << std::fixed << std::setprecision(2) << transaction.amount / 100.0 << std::endl;
            std::cout << "Category: " << transaction.category << std::endl;
            std::cout << "Description: " << transaction.description << std::endl;
            std::cout << "Wallet: " << transaction.wallet << std::endl;
            std::cout << std::endl;
        }
    }
}

void printGroupedByCategory(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults) {
    std::cout << "Expenses grouped by category: " << std::endl;
    for (const auto& [key, transactions] : groupedResults) {
        std::cout << "Category: " << key << std::endl << std::endl;
        for (const auto& transaction : transactions) {
            std::cout << "ID: " << transaction.id << std::endl;
            std::cout << "Date: " << transaction.date << std::endl;
            std::cout << "Amount: " << std::fixed << std::setprecision(2) << transaction.amount / 100.0 << std::endl;
            std::cout << "Description: " << transaction.description << std::endl;
            std::cout << "Wallet: " << transaction.wallet << std::endl;
            std::cout << std::endl;
        }
    }
}

void printGroupedByWallet(const std::unordered_map<std::string, std::vector<Transaction>>& groupedResults) {
    std::cout << "Expenses grouped by wallet: " << std::endl;
    for (const auto& [key, transactions] : groupedResults) {
        std::cout << "Wallet: " << key << std::endl << std::endl;
        for (const auto& transaction : transactions) {
            std::cout << "ID: " << transaction.id << std::endl;
            std::cout << "Date: " << transaction.date << std::endl;
            std::cout << "Amount: " << std::fixed << std::setprecision(2) << transaction.amount / 100.0 << std::endl;
            std::cout << "Description: " << transaction.description << std::endl;
            std::cout << "Category: " << transaction.category << std::endl;
            std::cout << std::endl;
        }
    }
}

