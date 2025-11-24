#pragma once

#include <string>
#include <iostream>
#include <algorithm>


/**
 * @brief Handle input from the user
 * 
 */
class Input {
    public:
        static std::string getString(const std::string& prompt, bool allowEmpty = false);

        static void wait();

    private:
        static std::string trim(const std::string& str);
};

