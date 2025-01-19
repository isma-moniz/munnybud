/**
 * @file utils.cpp
 * @author Ismael Moniz (hismamoniz@gmail.com)
 * @brief Header file for storage management functions that interact with the json data directly
 * @version 0.1
 * @date 2024-12-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STORAGE_HPP
#define STORAGE_HPP

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
private:
    json data;
    std::string jsonFileName;

    int loadData();
    int storeData();
public:
    StorageHandler(const std::string& jsonFileName);
    int storeTransaction(float amount, const std::string& category, const std::string& description, const std::string& date, const std::string& wallet);
    
    int retrieveExpenses(const std::string& base_date, int range, json::array_t& result);
    int retrieveDailyExpenses(const std::string& date, json::array_t& result);
    int retrieveWeeklyExpenses(const std::string& date, json::array_t& result);
    int retrieveMonthlyExpenses(const std::string& date, json::array_t& result);

    float retrieveBalance(const std::string& wallet);
    int updateBalance(const std::string& wallet, int amount);
};

#endif
