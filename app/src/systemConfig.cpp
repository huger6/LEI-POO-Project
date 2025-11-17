#include "systemConfig.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

void SystemConfig::setUTF8Locale() {
    try {
#ifdef _WIN32
        std::locale utf8Locale(".UTF-8");
        system("chcp 65001 > nul"); // set Windows console to UTF-8
#else
        std::locale utf8Locale("en_US.UTF-8");
#endif

        std::locale::global(utf8Locale);
        std::wcout.imbue(utf8Locale);
        std::wcin.imbue(utf8Locale);
        std::wclog.imbue(utf8Locale);

        std::wcout << L"UTF-8 locale successfully configured.\n";
    } catch (const std::exception& e) {
        std::cerr << "Failed to set UTF-8 locale: " << e.what() << '\n';
    }
}