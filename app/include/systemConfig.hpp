#pragma once

#include <locale>
#include <iostream>

#ifdef _WIN32
    #include <windows.h> // using namespace std gives compilation error (DO NOT USE on .hpp files)
#endif
#include <locale.h>

/**
 * @brief Configure system output
 * 
 */
class SystemConfig {
    public:
        /**
         * @brief Set terminal with UTF8 charset
         * 
         */
        static void setUTF8() {
            #ifdef _WIN32
                // SetConsoleOutputCP returns 0 if there's an error
                if ((SetConsoleOutputCP(CP_UTF8) == 0)||(SetConsoleCP(CP_UTF8) == 0)) {
                    std::cout << "Ocorreu um erro ao configurar o terminal do Windows para UTF-8." << std::endl;
                    std::cout << "A aplicação irá continuar. Desformatação será visível. Para resolver, reinicie a aplicação." << std::endl;
                }
                setlocale(LC_NUMERIC, "Portuguese"); // Floats now use ',' instead of '.'
            #else
                setlocale(LC_ALL, "pt_PT.UTF-8");
            #endif
        }
};

