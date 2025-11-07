#pragma once

#include <iostream>
#include <string>
#include <cstdint>

#include "filename.hpp"

class File {
    public:
        File(const std::string &filename);
        File(const std::string &filename, const std::uintmax_t size);

        void printFile(std::ostream &out, std::ostream *mirror = nullptr);
        const std::string getName() const;
    private:
        Filename filename;
        std::uintmax_t size;
        std::string create_date;
        std::string last_modified_date;
        std::string absolute_path;
};
