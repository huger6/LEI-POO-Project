#pragma once

#include <iostream>
#include <string>
#include <list>
#include <cstdint>
#include <memory>
#include <optional>

#include "folder.hpp"

enum class ElementType { Folder, File };

class FileSystem {
    public:
        FileSystem();
        FileSystem(const std::string &rootPath);
        ~FileSystem();
        
        bool load(); // 1
        bool load(const std::string &rootPath); // 1

        void clear(); // Clear all data from memory
        
        // Stats
        std::uint32_t countFiles() const; // 2
        std::uint32_t countFolders() const; // 3
        std::uintmax_t memory() const; // 4
        
        std::string *mostElementsFolder() const; // 4
        std::string *leastElementsFolder() const; // 6
        std::string *largestFile() const; // 7
        std::string *largestFolder() const; // 8
        
        // XML
        void saveToXML(const std::string &s) const; // 11
        bool readFromXML(const std::string &s); // 12
        
        // File operations
        bool moveFile(const std::string &file, const std::string &newDir); // 13
        bool moveFolder(const std::string &oldDir, const std::string &newDir); // 14
        bool copyBatch(const std::string &pattern, const std::string &originDir, const std::string &destinDir); // 21
        bool removeAll(const std::string &name, ElementType type); // 10
        void renameAllFiles(const std::string &currentName, const std::string &newName); // 19
        std::string *getFileDate(const std::string &file); // 15
        
        // Search operations
        std::optional<std::string> search(const std::string &name, ElementType type);
        void searchAllFolders(std::list<std::string> &li, const std::string &folder) const; // 17
        void searchAllFiles(std::list<std::string> &li, const std::string &file) const; // 18

        // Others
        bool checkDupFiles(); // 20
        void tree(std::ostream &out, std::ostream *mirror = nullptr); // 16

        // Setters
        void setPath(const std::string& path);

        // Getters
        const std::string& getPath() const;
    private:
        std::unique_ptr<Folder> root;
        std::string path; // Path to the root directory
};