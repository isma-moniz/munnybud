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
public:
    void storeExpense(float amount, const std::string& category, const std::string& description, const std::string& date);
    int retrieveExpensesByDate(const std::string&date, int range, json& result);
};

#endif