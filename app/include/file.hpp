#pragma once

#include <iostream>
#include <string>
#include <cstdint>

#include "filename.hpp"

using namespace std;

class File {
    public:
        File(const string &filename);
        File(const string &filename, const uintmax_t size);

    private:
        Filename filename;
        uintmax_t size;
        string create_date;
        string last_modified_date;
        string absolute_path;
};
