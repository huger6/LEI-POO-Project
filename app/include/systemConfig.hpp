#pragma once

#include <locale>
#include <iostream>

class SystemConfig{
    public:
        static void setUTF8Locale() {
            try{
                std::locale utf8Locale("en_US.UTF-8");
                std::locale::global (utf8Locale);
                std::wcout.imbue(utf8Locale);
                std::wcin.imbue(utf8Locale);
                std::wclog.imbue(utf8Locale);


                #ifdef _WIN32
                    system("chcp 65001 > nul");

                #endif
                    std::wcout << L"Locale sucessfully set to UTF-8.\n";
            }
            catch(const std::runtime_error& e) {
                std::cerr << "Failed to set UTF-8 locale:" << e.what() << '\n';
            }
        }
};