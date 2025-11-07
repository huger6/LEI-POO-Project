#include <filesystem>

#include "fileSystem.hpp"

using namespace std;
namespace fs = std::filesystem;

FileSystem::FileSystem() = default;

FileSystem::FileSystem(const string &path) {
    if (load(path))
        cout << "Loading was sucessful!" << endl;
    else 
        cout << "Loading Failed!" << endl;
}

FileSystem::~FileSystem() {}

bool FileSystem::load(const string &path) {
    fs::path dirPath = path;

    if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        return false;
    }

    // Create root
    root = make_unique<Folder>(dirPath.filename().string(), 0, nullptr);
    // Load from root
    return root->load(dirPath);
}

void FileSystem::tree(ostream &out, ostream *mirror) {
    root->tree("", true, out, mirror);
}