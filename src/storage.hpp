#include <string>
#include <stdio.h>
#include "json.hpp"
#include <fstream>
#include <vector>

using json = nlohmann::json;

class StorageHandler {
public:
    void storeExpense(float amount, const std::string& category, const std::string& description, const std::string& date);
};
