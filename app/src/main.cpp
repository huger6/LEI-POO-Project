#include "fileSystem.hpp"
#include "systemConfig.hpp"

int main() {
    string path = "C:\\Users\\hugoa\\Desktop\\LEI-U\\PINT";
    FileSystem sys(path);

    SystemConfig::setUTF8Locale();

    FileSystem fs;
    fs.load("C:\\Users\\Projects");

    return 0;
}