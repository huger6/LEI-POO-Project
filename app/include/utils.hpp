#pragma once

#include <iostream>
#include <string>


/**
 * @brief Functions with complementary use
 * 
 */
struct Utils {
    /**
     * @brief Places 0's before value
     * 
     * @param value Value to pad with 0's
     * @param width Width expected
     * @return std::string String padded
     */
    static std::string zeroPadding(uint16_t value, std::size_t width) {
        std::string s = std::to_string(value);
        if (s.length() < width) s = std::string(width - s.length(), '0') + s;
        return s;
    };  

    /**
     * @brief Extract something from a line
     * 
     * @param line Line
     * @param attr What to extract
     * @return std::string Extracted piece
     */
    static std::string extractAttribute(const std::string& line, const std::string& attr) {
        std::string key = attr + "=\"";
        size_t start = line.find(key);

        if (start == std::string::npos) return "";

        start += key.size();
        size_t end = line.find('"', start);

        return line.substr(start, end - start);
    }

    /**
     * @brief Check if a string has a certain pattern
     * 
     * @param str string
     * @param pattern pattern
     * @return true Has pattern
     * @return false Doesn't have pattern
     */
    static bool hasPattern(const std::string &str, const std::string &pattern) {
        return str.find(pattern) != std::string::npos;
    }

    /**
     * @brief Clear terminal
     * 
     */
    static void clear() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
};

