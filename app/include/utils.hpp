#pragma once

#include <iostream>
#include <string>

using namespace std;

struct Utils {
    static string zeroPadding(uint16_t value, std::size_t width) {
        std::string s = std::to_string(value);
        if (s.length() < width) s = std::string(width - s.length(), '0') + s;
        return s;
    };  
};