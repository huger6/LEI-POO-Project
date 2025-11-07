#include "fileSystem.hpp"
#include "systemConfig.hpp"

// using namepsace std should not be used on this file

int main() {
    SystemConfig::setUTF8();
    std::string path = "C:\\Users\\hugoa\\Desktop\\LEI-U\\PINT";
    FileSystem fs(path);
    fs.tree(std::cout, nullptr);
}