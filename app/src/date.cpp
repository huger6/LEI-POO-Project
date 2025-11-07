#include "date.hpp"
#include "utils.hpp"

using namespace std;

Date::Date(uint16_t day, uint16_t month, uint16_t year) : day(day), month(month), year(year) {}

Date::~Date() {}

std::string Date::getFormattedDate() const {
    return Utils::zeroPadding(day, 2) + "/" + Utils::zeroPadding(month, 2) + "/" + Utils::zeroPadding(year, 4);
}

uint16_t Date::getDay() const { return day; }

uint16_t Date::getMonth() const { return month; }

uint16_t Date::getYear() const { return year; }