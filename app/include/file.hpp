#pragma once

#include <iostream>
#include <string>

using namespace std;

class File {
    public:
        File(const string &name, const size_t size);
    private:
        string name;
        size_t size;
        string create_date;
        string last_modified_date;
        string absolute_path;
};