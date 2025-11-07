#pragma once

#include <iostream>
#include <string>
#include <list>
#include <cstdint>
#include <memory>

#include "folder.hpp"

class FileSystem {
    public:
        FileSystem();
        FileSystem(const std::string &path);
        ~FileSystem();
        
        bool load(const std::string &path);
        
        // Stats
        std::uint32_t countFiles() const;
        std::uint32_t countDirectories() const;
        std::uint32_t memory() const;
        
        std::string *mostElementsDirectory();
        std::string *leastElementsDirectory();
        std::string *largestDirectory();
        std::string *getFileDate(const std::string &file);

        // XML
        void saveToXML(const std::string &s);
        bool readFromXML(const std::string &s);
        
        // File operations
        bool moveFile(const std::string &file, const std::string &newDir);
        bool moveDir(const std::string &oldDir, const std::string &newDir);
        bool copyBatch(const std::string &pattern, const std::string &originDir, const std::string &destinDir);
        bool removeAll(const std::string &s, int type);
        void renameAllFiles(const std::string &old_file, const std::string &new_file);
        
        // Search operations
        std::string *search(const std::string &s, int type);
        void searchAllDirs(std::list<std::string> &li, const std::string &dir);
        void searchAllFiles(std::list<std::string> &li, const std::string &file);

        // Others
        bool checkDupFiles();
        void tree(std::ostream &out, std::ostream *mirror = nullptr);
    private:
        std::unique_ptr<Folder> root;
};