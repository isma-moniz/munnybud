#include "interface.hpp"

void printResults(nlohmann::json::array_t &results) { 
    for (int d = 0; d < results.size(); d++) {
        std::string date = results[d][0];
        std::cout << "Expenses made in " << date << ":\n";
        for (int i = 1; i < results[d].size(); i++) {
            std::cout << results[d][i] << std::endl;
        }
    }
}
