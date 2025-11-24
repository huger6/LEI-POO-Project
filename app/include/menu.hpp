#pragma once

#include <string>
#include <vector>

/**
 * @brief Handle menu output and option chosen
 * 
 */
class Menu {
    public:
        Menu(const std::string& title, const std::vector<std::string>& options);

        static bool askYesNo(const std::string& question, bool clearTerminal = false);

        int show(bool clearTerminal = true);
    private:
        std::string title;
        std::vector<std::string> options;
};

