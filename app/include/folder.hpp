#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <cstdint>
#include <list>
#include <unordered_set>
// tinyxml2 library
#include "tinyxml2.h"

#include "file.hpp"
#include "element.hpp"

constexpr std::uint16_t SPACES_PER_LEVEL = 4;

namespace fs = std::filesystem;
namespace xml = tinyxml2;

/**
 * @brief Handle all folder related operations
 * 
 */
class Folder : public Element {
    public:
        Folder(std::string name, Folder *father);

        bool load(const fs::path& path);

        void add(std::unique_ptr<Element> element);
        std::unique_ptr<Element> remove(const std::string& name, ElementType type);

        bool copyBatch(const std::string &pattern, Folder *destin);
        
        std::uint32_t countFiles() const;
        std::uint32_t countFolders() const;
        std::uintmax_t memory() const;

        const Folder *mostElementsFolder() const;
        const Folder *leastElementsFolder() const;
        const File *largestFile() const;
        const Folder *largestFolder() const;

        void saveToXML(xml::XMLDocument &doc, xml::XMLElement *parentElem) const;
        void readFromXML(xml::XMLElement *dirElem);

        std::string searchFolder(const std::string& name) const;
        void searchAllFolders(std::list<std::string> &li, const std::string& name, const std::string& path) const;
        std::string searchFile(const std::string& name) const;
        void searchAllFiles(std::list<std::string> &li, const std::string& name, const std::string& path) const;

        bool checkDupFiles(std::unordered_set<std::string>& names);
        void tree(const std::string &prefix, bool isLast, std::ostream &out, std::ostream *mirror) const;

        bool hasFile(const std::string &name) const;
        // Setters
        void setParent(Folder *parent);
        // Getters
        Folder *getFolderByName(const std::string& name) const;
        File *getFileByName(const std::string& name) const;
        Folder *getFolderByFileName(const std::string& name) const;
        Folder* getParent() const;
        const std::string getName() const;

        bool isFile() const override { return false; }
        bool isFolder() const override { return true; }
    private:
        std::vector<std::unique_ptr<Element>> elements;
        Folder *root;
};
