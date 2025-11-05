#include "fileSystem.hpp"
#include "systemConfig.hpp"

int main() {
    SystemConfig::setUTF8Locale();
    string path = "C:\\Users\\hugoa\\Desktop\\LEI-U\\PINT";
    FileSystem fs(path);
}