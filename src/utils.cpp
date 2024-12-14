#include "utils.hpp"
#include <chrono>
#include <ctime>

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