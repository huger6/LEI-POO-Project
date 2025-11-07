#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <cstdint>

#include "file.hpp"

constexpr std::uint16_t SPACES_PER_LEVEL = 4;

namespace fs = std::filesystem;

class Folder {
    public:
        Folder(std::string name, std::uintmax_t size, Folder *father);

        bool load(const fs::path& path);

        void addFile(std::unique_ptr<File> file);
        void addFolder(std::unique_ptr<Folder> folder);
        
        void printFolder(std::ostream &out, std::ostream *mirror = nullptr);

        // void tree(std::uint16_t depth, std::ostream &out, std::ostream *mirror = nullptr) const;
        void tree(const std::string &prefix, bool isLast, std::ostream &out, std::ostream *mirror) const;

        const std::string& getName() const;
        const std::vector<std::unique_ptr<File>>& getFiles() const;
        const std::vector<std::unique_ptr<Folder>>& getSubfolders() const;
    private:
        std::string name;
        std::uintmax_t size;
        std::vector<std::unique_ptr<File>> files;
        std::vector<std::unique_ptr<Folder>> subfolders;
        Folder *root;
};
