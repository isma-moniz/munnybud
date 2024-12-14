/**
* @file 
* @brief Utils header file
*
* Contains utilities such as date manipulation, parsing, etc...
*/

#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <iostream>
#include <optional>
#include <string>

bool isDateInRange(const std::string& baseDate, const std::string& testDate, int range);
std::optional<std::chrono::system_clock::time_point> parseDate(const std::string& dateString);

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