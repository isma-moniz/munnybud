/**
* @file 
* @brief Utils implementation file
*
* Contains utilities such as date manipulation, parsing, etc...
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
bool isDateInRange(const std::string &baseDate, const std::string &testDate, int range) {
    auto baseTime = parseDate(baseDate);
    auto testTime = parseDate(testDate);

    if ((!baseTime) || (!testTime)) {
        std::cout << "isDateInRange error: invalid dates.\n";
        return false;
    }

    auto rangeDuration = std::chrono::hours(range * 24);
    return (*testTime >= *baseTime && *testTime < (*baseTime + rangeDuration));
}

/**
 * @brief Parses a date string into a `std::chrono::system_clock::time_point`.
 *
 * This function takes a date string in the format "YYYY-MM-DD" and converts it
 * into a `std::chrono::system_clock::time_point`. If the parsing fails, it returns
 * `std::nullopt`.
 *
 * @param dateString The date string to parse in the format "YYYY-MM-DD".
 *
 * @return An optional containing the parsed time_point if successful; std::nullopt otherwise.
 */
std::optional<std::chrono::system_clock::time_point> parseDate(const std::string& dateString) {
    struct tm dateTm;
    std::istringstream ss(dateString);
    ss >> std::get_time(&dateTm, "%Y-%m-%d");
    if (ss.fail()) {
        return std::nullopt;
    }

    // Treat the date as UTC with no time offset
    dateTm.tm_hour = 0;
    dateTm.tm_min = 0;
    dateTm.tm_sec = 0;

    return std::chrono::system_clock::from_time_t(mktime(&dateTm));
}