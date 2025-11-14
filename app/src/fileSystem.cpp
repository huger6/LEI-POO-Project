#include <filesystem>
#include <fstream>
// tinyxml2 library
#include "tinyxml2.h"

#include "fileSystem.hpp"
#include "utils.hpp"

using namespace std;
namespace fs = std::filesystem;
namespace xml = tinyxml2;

/**
 * @brief Construct a new File System:: File System object
 * 
 */
FileSystem::FileSystem() = default;

/**
 * @brief Create a new filesystem with direct loading with feedback
 * 
 * @param path Absolute path to the to-be root directory
 */
FileSystem::FileSystem(const string &rootPath) : path(rootPath) {
    if (load())
        cout << "Loading was sucessful!" << endl;
    else 
        cout << "Loading Failed!" << endl;
}

/**
 * @brief Destroy the File System:: File System object
 * 
 */
FileSystem::~FileSystem() {

}

/**
 * @brief Loads the folders and files to memory from the absolute path stored
 * 
 * @return true Loading succeeded
 * @return false Loading failed
 */
bool FileSystem::load() {
    fs::path dirPath = path;

    if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        return false;
    }

    // Create root
    root = make_unique<Folder>(dirPath.filename().string(), nullptr);
    // Load from root
    return root->load(dirPath);
}

/**
 * @brief Loads the folders and files to memory from an absolute path
 * 
 * @param path Absolute path to the to-be root directory
 * @return true Loading succeeded
 * @return false Loading failed
 */
bool FileSystem::load(const std::string &rootPath) {
    fs::path dirPath = rootPath;

    if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        return false;
    }

    // Create root
    root = make_unique<Folder>(dirPath.filename().string(), nullptr);
    // Load from root
    return root->load(dirPath);
}

void FileSystem::clear() {
    return;
}

// Stats

/**
 * @brief Gets the number of files
 * 
 * @return uint32_t Number of files
 */
uint32_t FileSystem::countFiles() const {
    return (root == nullptr ? 0 : root->countFiles());
}

/**
 * @brief Gets the number of folders
 * 
 * @return uint32_t Number of folders
 */
uint32_t FileSystem::countFolders() const {
    return (root == nullptr ? 0 : 1 + root->countFolders());
}

/**
 * @brief Gets the memory occupied by all files, folders and the current program execution
 * 
 * @return uintmax_t memory in bytes, 0 if error
 */
uintmax_t FileSystem::memory() const {
    return (root == nullptr ? 0 : static_cast<uintmax_t>(sizeof(unique_ptr<Folder>)) + root->memory());
}

/**
 * @brief Finds the folder with the most elements inside (out of all folders)
 * 
 * @note return value must be deleted after use
 * 
 * @return string* Name of the folder, nullptr if error
 */
string *FileSystem::mostElementsFolder() const {
    if (!root) return nullptr;

    const Folder *f = root->mostElementsFolder();
    return new string(f->getName());
} 

/**
 * @brief Finds the folder with the least elements inside (out of all folders)
 * 
 * @note return value must be deleted after use
 * 
 * @return string* Name of the folder, nullptr if error
 */
string *FileSystem::leastElementsFolder() const {
    if (!root) return nullptr;

    const Folder *f = root->leastElementsFolder();
    return new string(f->getName());
}

/**
 * @brief Finds the largest file
 * 
 * @return std::string* Name of the file, nullptr if error
 */
std::string *FileSystem::largestFile() const {
    if (!root) return nullptr;

    const File *f = root->largestFile();
    return new string(f->getName());
}

/**
 * @brief Finds the largest folder
 * 
 * @return std::string* Name of the folder, nullptr if error
 */
std::string *FileSystem::largestFolder() const {
    if (!root) return nullptr;

    const Folder *f = root->largestFolder();
    return new string(f->getName());
}

// XML

/**
 * @brief Save all the filesystem folders and files to XML format
 * 
 * @param filename Name of the file (with or without extension)
 */
void FileSystem::saveToXML(const std::string &filename) const {
    if (!root) {
        cerr << "There is no data to be saved" << endl;
        return;
    }

    if (filename.empty()) {
        cerr << "Filename is empty" << endl;
        return;
    }

    Filename name(filename);
    if (name.getExtension() != string("xml")) {
        name.setExtension(string("xml"));
    }
      
    xml::XMLDocument doc;

    // Create main xml root element
    xml::XMLElement *xmlRoot = doc.NewElement("FileSystem");
    doc.InsertFirstChild(xmlRoot);

    root->saveToXML(doc, xmlRoot);

    doc.SaveFile(name.getFullname().c_str());
}

/**
 * @brief Load all the filesystem folders and files from XML format to memory
 * 
 * @param filename Filename with extension
 * @return true Success
 * @return false Failure
 */
bool FileSystem::readFromXML(const string &filename) {
    if (filename.empty()) {
        cerr << "Filename is empty" << endl;
        return false;
    }

    Filename name(filename);
    if (name.getExtension() != string("xml")) {
        cerr << "Filename is not a XML file" << endl;
        return false;
    }

    xml::XMLDocument doc;

    xml::XMLError error = doc.LoadFile(filename.c_str());
    if (error != xml::XML_SUCCESS) {
        cerr << "Error reading XML: " << error << endl;
        return false;
    }

    xml::XMLElement *xmlRoot = doc.FirstChildElement("FileSystem");
    if (!xmlRoot) {
        cerr << "Invalid XML: missing <FileSystem> root" << endl;
        return false;
    };

    // Clear existing data before loading
    clear();

    // Find first folder
    xml::XMLElement *dir = xmlRoot->FirstChildElement("Folder");
    if (!dir) {
        cerr << "XML contains no directories" << endl;
        return false;
    }

    // Create root 
    const char *rootName = dir->Attribute("name");
    if (!rootName) {
        cerr << "Root directory name is missing" << endl;
        return false;
    }
    string nameS = rootName;
    root = make_unique<Folder>(nameS, nullptr);
    root->readFromXML(dir);

    return true;
}

// Move operations

/**
 * @brief Move a file to a new folder
 * 
 * @param file File to be moved
 * @param newFolder New folder where the file will be moved into
 * @return true Sucess moving the file
 * @return false Failed to move the file
 */
bool FileSystem::moveFile(const std::string &file, const std::string &newFolder) {
    // Find file's parent
    Folder *parent = root->getFolderByFileName(file);
    if (!parent) return false;

    // Find destination folder
    Folder *dest = root->getFolderByName(newFolder);
    if (!dest) return false;

    // Check if moving is unnecessary
    if (parent == dest) return false;

    // Get file
    unique_ptr<File> f = parent->removeFile(file);
    if (!f) return false;

    dest->addFile(move(f));
    return true;
}

// Search Operations

/**
 * @brief Search by the name of a folder/file
 * 
 * @param name Name to search for
 * @param type Type Folder/File
 * @return nullopt if there are no search results
 * @return optional<string> Absolute path to the type element
 */
optional<string> FileSystem::search(const string &name, ElementType type) {
    if (name.empty() || !root) {
        return nullopt;
    }
    string path(root->getName());
    string resultPath;
    
    if (type == ElementType::Folder) {
        if (path == name) return path.append("/");
        resultPath = root->searchFolder(name);
    }
    else if (type == ElementType::File) {
        resultPath = root->searchFile(name);
    }

    if (resultPath.empty()) return nullopt;

    path.append("/").append(resultPath);

    return resultPath;
}

/**
 * @brief Search all folders with name 'folder' and place them in 'li'
 * 
 * @param li List where results will be placed
 * @param folder Name of the folder to search for
 */
void FileSystem::searchAllFolders(list<string> &li, const string &folder) const {
    if (folder.empty()) return;

    root->searchAllFolders(li, folder, path);
}

/**
 * @brief Search all folders for files with name 'file' and place them in 'li'
 * 
 * @param li List where results will be placed
 * @param folder Name of the file to search for
 */
void FileSystem::searchAllFiles(list<string> &li, const string &file) const {
    if (file.empty()) return;

    root->searchAllFiles(li, file, path);
}

// Others

/**
 * @brief Output Windows like tree command
 * 
 * @param out Where to show the tree
 * @param mirror Use to show to multiple interfaces concurrently
 */
void FileSystem::tree(ostream &out, ostream *mirror) {
    root->tree("", true, out, mirror);
}

// Setters

/**
 * @brief Set the absolute path to the root directory
 * 
 * @param newPath New path to be set
 */
void FileSystem::setPath(const string& newPath) {
    path = newPath;
}

// Getters

/**
 * @brief Get the current absolute path being used
 * 
 * @return const string& Path
 */
const string& FileSystem::getPath() const { return path; }
