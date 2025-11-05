#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>

#include "file.hpp"

using namespace std;
namespace fs = filesystem;

class Folder {
    public:
        Folder(string name, uintmax_t size, Folder *father);

        bool load(const fs::path& path);

        void addFile(unique_ptr<File> file);
        void addFolder(unique_ptr<Folder> folder);

        const string& getName() const;
        const vector<unique_ptr<File>>& getFiles() const;
        const vector<unique_ptr<Folder>>& getSubfolders() const;
    private:
        string name;
        uintmax_t size;
        vector<unique_ptr<File>> files;
        vector<unique_ptr<Folder>> subfolders;
        Folder *root;
};  

