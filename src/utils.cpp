/**
 * @file utils.cpp
 * @author Ismael Moniz (hismamoniz@gmail.com)
 * @brief Implementation file for utils such as date manipulation, date parsing, etc.
 * @version 0.1
 * @date 2024-12-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "utils.hpp"
#include <chrono>
#include <ctime>

/**
 * @brief Checks if a given test date falls within a specified range from a base date.
 *
 * This function parses both the base and test dates, calculates the range, and determines
 * if the test date falls within the range (inclusive of the base date and exclusive of the upper bound).
 *
 * @param baseDate The base date as a string in the format "YYYY-MM-DD".
 * @param testDate The test date as a string in the format "YYYY-MM-DD".
 * @param range The number of days for the range.
 *
 * @return True if the test date is within the range; false otherwise.
 */
/*bool isDateInRange(const std::string &baseDate, const std::string &testDate, int range) {
    auto baseTime = parseDate(baseDate);
    auto testTime = parseDate(testDate);

    if ((!baseTime) || (!testTime)) {
        std::cout << "isDateInRange error: invalid dates.\n";
        return false;
    }

    auto rangeDuration = std::chrono::hours(range * 24);
    return (*testTime >= *baseTime && *testTime < (*baseTime + rangeDuration));
} */

/**
 * @brief Parses a date string into a 'std::chrono::year_month_day'.
 *
 * This function takes a date string in the format "YYYY-MM-DD" and converts it
 * into a 'std::chrono::year_month_day'. If the parsing fails, it throws a runtime error.
 * 
 * @param dateString The date string to parse in "YYYY-MM-DD" format.
 * @return std::chrono::year_month_day of the provided date string.
 */
std::chrono::year_month_day parseYMD(const std::string& dateString) {
    std::istringstream ss(dateString);
    std::chrono::year_month_day ymd;
    ss >> std::chrono::parse("%Y-%m-%d", ymd);
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse date: " + dateString);
    }
    return ymd;
}

bool same_month(const std::chrono::year_month_day& d1, const std::chrono::year_month_day& d2) {
    return d1.year() == d2.year() && d1.month() == d2.month();
}

/**
 * @brief Gets the current date in a string format
 * 
 * @return std::string with current date in YYYY-mm-dd format
 */
std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();

    time_t time_now = std::chrono::system_clock::to_time_t(now);

    struct tm time_tm = *std::localtime(&time_now);

    std::stringstream ss;
    ss << std::put_time(&time_tm, "%Y-%m-%d");

    return ss.str();
}