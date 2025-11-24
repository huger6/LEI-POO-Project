#include "date.hpp"

#include "utils.hpp"


using namespace std;


/**
 * @brief Construct a new Date:: Date object
 * 
 */
Date::Date() = default;

/**
 * @brief Construct a new Date:: Date object
 * 
 * @param day 
 * @param month 
 * @param year 
 */
Date::Date(uint16_t day, uint16_t month, uint16_t year) : day(day), month(month), year(year) {}

/**
 * @brief Construct a new Date object
 * 
 * @param date Date in string format
 */
Date::Date(const string &date) {
    parse(date);
}

/**
 * @brief Converts a filesystem::file_time_type into a Date scrutured in day, month and year
 * 
 * @param ftime filesystem object to convert
 * @return Date Date converted
 */
Date Date::convertFileTime(const std::filesystem::file_time_type &ftime) {
    auto sctp = chrono::time_point_cast<chrono::system_clock::duration>(
        ftime - filesystem::file_time_type::clock::now() + chrono::system_clock::now()
    );

    time_t tt = chrono::system_clock::to_time_t(sctp);

    tm time{};

    #ifdef _WIN32
        localtime_s(&time, &tt);
    #else
        localtime_r(&tt, &time);
    #endif

    return Date(
        time.tm_mday,
        time.tm_mon + 1,
        time.tm_year + 1900
    );
}

/**
 * @brief Return current date
 * 
 * @return Date Today's date
 */
Date Date::now() {
    using namespace std::chrono;

    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);
    std::tm *lt = std::localtime(&t);

    return Date(
        static_cast<std::uint16_t>(lt->tm_mday),
        static_cast<std::uint16_t>(lt->tm_mon + 1),
        static_cast<std::uint16_t>(lt->tm_year + 1900)
    );
}

/**
 * @brief Get the date formatted as a string with "/" separating
 * 
 * @return string Date formatted
 */
string Date::getFormattedDate() const {
    return Utils::zeroPadding(day, 2) + "/" + Utils::zeroPadding(month, 2) + "/" + Utils::zeroPadding(year, 4);
}

/**
 * @brief Get the day 
 * 
 * @return uint16_t day
 */
uint16_t Date::getDay() const { return day; }

/**
 * @brief Get the month
 * 
 * @return uint16_t month
 */
uint16_t Date::getMonth() const { return month; }

/**
 * @brief Get the year
 * 
 * @return uint16_t year
 */
uint16_t Date::getYear() const { return year; }

/**
 * @brief Parse the date string and get 
 * 
 * @param dateStr 
 */
void Date::parse(const string &dateStr) {
    char sep1, sep2;
    istringstream iss(dateStr);

    iss >> day >> sep1 >> month >> sep2 >> year;
}

