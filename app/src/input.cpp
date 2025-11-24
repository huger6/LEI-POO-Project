#include "input.hpp"

#include <limits>

/**
 * @brief Get a string as user input
 * 
 * @param prompt Prompt to show
 * @param allowEmpty true to allow "", false by default
 * @return std::string 
 */
std::string Input::getString(const std::string& prompt, bool allowEmpty) {
    std::string line;

    while (true) {
        std::cout << prompt;
        
        // Read the full line (including spaces)
        std::getline(std::cin, line);

        // Handle stream errors
        if (std::cin.fail()) {
            std::cin.clear(); // Reset error flags
            // Ignore the rest of the buffer to prevent infinite loops
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nError reading input. Please try again.\n";
            continue;
        }

        // Trim whitespace
        line = trim(line);

        // Check for empty input if not allowed
        if (!allowEmpty && line.empty()) {
            std::cout << "Input cannot be empty.\n";
            continue; 
        }

        // Good input
        break;
    }
    
    return line;
}

/**
 * @brief Wait for user confirmation
 * 
 */
void Input::wait() {
    std::cout << "\nPress ENTER to continue...";
    std::cin.get(); 
}

/**
 * @brief Trim whitespaces
 * 
 * @param str String
 * @return std::string string trimmed
 */
std::string Input::trim(const std::string& str) {
    auto start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return ""; // String is all whitespace

    auto end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

