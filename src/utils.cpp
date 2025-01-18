/**
 * @file utils.cpp
 *
 * @brief Implementation file for utils such as date manipulation, date parsing, etc.
 * 
 */

#include "utils.hpp"
#include <chrono>
#include <ctime>

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

void getWeek(const std::chrono::year_month_day& baseDate, std::chrono::year_month_day& firstDay, std::chrono::year_month_day& lastDay) {
    using namespace std::chrono;

    sys_days baseDays = sys_days{baseDate};
    weekday baseWd = weekday{baseDays};
    
    sys_days startOfWeek = baseDays - days(baseWd.iso_encoding() -1);
    sys_days endOfWeek = startOfWeek + days(6);

    firstDay = year_month_day(startOfWeek);
    lastDay = year_month_day(endOfWeek);
}
