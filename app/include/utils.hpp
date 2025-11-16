#pragma once

#include <iostream>
#include <string>

struct Utils {
    static std::string zeroPadding(uint16_t value, std::size_t width) {
        std::string s = std::to_string(value);
        if (s.length() < width) s = std::string(width - s.length(), '0') + s;
        return s;
    };  

    static std::string extractAttribute(const std::string& line, const std::string& attr) {
        std::string key = attr + "=\"";
        size_t start = line.find(key);

        if (start == std::string::npos) return "";

        start += key.size();
        size_t end = line.find('"', start);

        return line.substr(start, end - start);
    }

    static bool hasPattern(const std::string &name, const std::string &pattern) {
        return name.find(pattern) != std::string::npos;
    }
};