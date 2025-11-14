#pragma once

#include "fileSystem.hpp"
#include <iostream>
#include <string>

class Menu{
    private:
    // Métodos de exibição de menus
        void displayMainMenu();
        void showFileSystemStats();
        void showSearchMenu();

        // Métodos de ação (amostragem/carregamento)
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

        /**
         * @brief Construtor. Inicializa o menu.
         */
        Menu();

        /**
         * @brief Inicia o loop principal do menu.
         */
        void run();
};

Menu::Menu()
{
}

Menu::~Menu()
{
}