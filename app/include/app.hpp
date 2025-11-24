#pragma once

#include <iostream>

#include "menu.hpp"
#include "fileSystem.hpp"


/**
 * @brief Main application logic
 * 
 */
class App {
    public:
        App();

        void run();
    private:
        Menu mainMenu;
        FileSystem fs;

        void loadSave();
        void statistics();
        void searchs();
        void operations();
        void advanced();
};

