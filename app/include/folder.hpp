#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "file.hpp"

using namespace std;

class Folder {
    public:
        
    private:
        string name;
        size_t size;
        vector<unique_ptr<File>> files;
        vector<unique_ptr<Folder>> subfolders;
};