#pragma once

#include <iostream>
#include <string>
#include <list>

#include "folder.hpp"

using namespace std;

class FileSystem {
    public:
        FileSystem();
        FileSystem(const string &path);
        ~FileSystem();
        
        bool load(const string &path);
        
        // Stats
        uint32_t countFiles() const;
        uint32_t countDirectories() const;
        uint32_t memory() const;
        
        string *mostElementsDirectory();
        string *leastElementsDirectory();
        string *largestDirectory();
        string *getFileDate(const string &file);

        // XML
        void saveToXML(const string &s);
        bool readFromXML(const string &s);
        
        // File operations
        bool moveFile(const string &file, const string &newDir);
        bool moveDir(const string &oldDir, const string &newDir);
        bool copyBatch(const string &pattern, const string &originDir, const string &destinDir);
        bool removeAll(const string &s, int type);
        void renameAllFiles(const string &old_file, const string &new_file);
        
        // Search operations
        string *search(const string &s, int type);
        void searchAllDirs(list<string> &li, const string &dir);
        void searchAllFiles(list<string> &li, const string &file);

        // Others
        bool checkDupFiles();
        void tree(const string *file);
    private:
        unique_ptr<Folder> root;
};