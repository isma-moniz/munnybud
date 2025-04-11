/**
 * @file utils.cpp
 * @author Ismael Moniz (hismamoniz@gmail.com)
 * @brief Header file for utils such as date manipulation, date parsing, etc.
 * @version 0.1
 * @date 2024-12-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <iostream>
#include <optional>
#include <string>

std::chrono::year_month_day parseYMD(const std::string& dateString);
std::string formatYMD(const std::chrono::year_month_day& dateYMD);
std::string getCurrentDate();
bool same_month(const std::chrono::year_month_day& d1, const std::chrono::year_month_day& d2);
void getWeek(const std::chrono::year_month_day& baseDate, std::chrono::year_month_day& firstDay, std::chrono::year_month_day& lastDay);

enum daysByMonth {
    jan = 31,
    feb = 28, 
    mar = 31,
    apr = 30,
    may = 31,
    jun = 30,
    jul = 31,
    aug = 31,
    sep = 30,
    oct = 31,
    nov = 30,
    feb_bi = 29
};

#endif
