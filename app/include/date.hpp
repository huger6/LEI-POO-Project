#pragma once

#include <iostream>
#include <string>
#include <cstdint>

class Date {
    public:
        Date();
        Date(uint16_t day, uint16_t month, uint16_t year);
        ~Date();

        std::string getFormattedDate() const;
        uint16_t getDay() const;
        uint16_t getMonth() const;
        uint16_t getYear() const;
    private:
        uint16_t day, month, year;
};