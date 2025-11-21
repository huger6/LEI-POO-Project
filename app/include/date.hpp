#pragma once

#include <iostream>
#include <string>
#include <cstdint>
#include <filesystem>

class Date {
    public:
        Date();
        Date(std::uint16_t day, std::uint16_t month, std::uint16_t year);
        Date(const std::string &date);
        ~Date();

        static Date convertFileTime(const std::filesystem::file_time_type &ftime);
        static Date now();

        std::string getFormattedDate() const;
        std::uint16_t getDay() const;
        std::uint16_t getMonth() const;
        std::uint16_t getYear() const;
    private:
        std::uint16_t day, month, year;

        void parse(const std::string &dateStr);
};