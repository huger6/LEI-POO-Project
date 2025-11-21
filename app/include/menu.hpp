/*

#pragma once

#include <iostream>
#include <string>

#include "fileSystem.hpp"

class Menu {
    private:
    // Métodos de exibição de menus
        void displayMainMenu();
        void showFileSystemStats();
        void showSearchMenu();

        // Métodos de ação (amostragem/carioregamento)
    void loadFileSystem();
    void showTree();
    void checkDuplicates();
    void exit();

    // Métodos utilitários
    int getUserChoice() const;
    std::string getStringInput(const std::string& prompt) const;
    void clearScreen() const;
    void pause() const;

    FileSystem fs;  // A instância principal do FileSystem
    bool running;   // Controla o loop principal do menu
    bool loaded;    // Rastreia se um FileSystem foi carregado
    
    public:

        
        Menu();

        
        void run();
};

Menu::Menu()
{
}

Menu::~Menu()
{
}

*/