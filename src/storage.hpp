/**
* @file
* @brief Storage handler header file
*
* Interacts with the JSON storage file, performing the low level operations such as storing and retrieving data
*/
#include "utils.hpp"

#include <string>
#include <stdio.h>
#include "json.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <ctime>

using json = nlohmann::json;

/**
* @class
* @brief Storage Handler class
* 
*/
class StorageHandler {
public:
    void storeExpense(float amount, const std::string& category, const std::string& description, const std::string& date);
    int retrieveExpensesByDate(const std::string&date, int range, json& result);
};
