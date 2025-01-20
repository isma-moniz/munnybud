#include "interface.hpp"

void printResults(std::vector<Transaction>& results) { 
    for (Transaction& transaction : results) {
        std::cout << "Date: " << transaction.date << std::endl;
        std::cout << "Amount: " << std::fixed << std::setprecision(2) << transaction.amount / 100.0 << std::endl;
        std::cout << "Category: " << transaction.category << std::endl;
        std::cout << "Description: " << transaction.description << std::endl;
        std::cout << "Wallet: " << transaction.wallet << std::endl;
    }
}
