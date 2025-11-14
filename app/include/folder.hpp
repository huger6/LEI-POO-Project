#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <cstdint>
#include <list>
// tinyxml2 library
#include "tinyxml2.h"

#include "file.hpp"

constexpr std::uint16_t SPACES_PER_LEVEL = 4;

namespace fs = std::filesystem;
namespace xml = tinyxml2;

class Folder {
    public:
        Folder(std::string name, Folder *father);

        bool load(const fs::path& path);

        void addFile(std::unique_ptr<File> file);
        std::unique_ptr<File> removeFile(const std::string& name);
        void addFolder(std::unique_ptr<Folder> folder);
        
        std::uint32_t countFiles() const;
        std::uint32_t countFolders() const;
        std::uintmax_t memory() const;

        const Folder *mostElementsFolder() const;
        const Folder *leastElementsFolder() const;
        const File *largestFile() const;
        const Folder *largestFolder() const;

        void saveToXML(xml::XMLDocument &doc, xml::XMLElement *parentElem) const;
        void readFromXML(xml::XMLElement *dirElem);
        // read

        std::string searchFolder(const std::string& name) const;
        void searchAllFolders(std::list<std::string> &li, const std::string& name, const std::string& path) const;
        std::string searchFile(const std::string& name) const;
        void searchAllFiles(std::list<std::string> &li, const std::string& name, const std::string& path) const;

        void tree(const std::string &prefix, bool isLast, std::ostream &out, std::ostream *mirror) const;

        // Getters
        Folder *getFolderByName(const std::string& name) const;
        File *getFileByName(const std::string& name) const;
        Folder *getFolderByFileName(const std::string& name) const;
        const std::string& getName() const;
        const std::vector<std::unique_ptr<File>>& getFiles() const;
        const std::vector<std::unique_ptr<Folder>>& getSubfolders() const;
    private:
        std::string name;
        std::vector<std::unique_ptr<File>> files;
        std::vector<std::unique_ptr<Folder>> subfolders;
        Folder *root;
};
