#include <filesystem>
#include <fstream>
#include <unordered_set>
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
bool FileSystem::load(const string &rootPath) {
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
string *FileSystem::largestFile() const {
    if (!root) return nullptr;

    const File *f = root->largestFile();
    return new string(f->getName().getFullname());
}

/**
 * @brief Finds the largest folder
 * 
 * @return std::string* Name of the folder, nullptr if error
 */
string *FileSystem::largestFolder() const {
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
void FileSystem::saveToXML(const string &filename) const {
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

// File operations

/**
 * @brief Move a file to a new folder
 * 
 * @param file File to be moved
 * @param newFolder New folder where the file will be moved into
 * @return true Sucess moving the file
 * @return false Failed to move the file
 */
bool FileSystem::moveFile(const string &file, const string &newFolder) {
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

/**
 * @brief Move a folder into another folder
 * 
 * @param oldDir Folder to me moved
 * @param newDir Folder to move oldDir into
 * @return true Success
 * @return false Failure
 */
bool FileSystem::moveFolder(const string &oldDir, const string &newDir) {
    // Find folder to be moved
    Folder *oldF = root->getFolderByName(oldDir);
    if (!oldF) return false;

    // Find folder to move it into
    Folder *newF = root->getFolderByName(newDir);
    if (!newF) return false;

    // Check if newDir is a subfolder of oldDir
    if (oldF == newF || oldF->getFolderByName(newDir)) return false;

    Folder *oldParent = oldF->getParent();
    if (!oldParent) return false; // root must not be moved

    // Get oldF unique_ptr
    unique_ptr<Folder> f = oldParent->removeFolder(oldDir);
    if (!f) return false;

    // Add to newDir
    newF->addFolder(move(f));
    return true;
}

/**
 * @brief Copy 
 * 
 * @param pattern Pattern to find in 
 * @param originDir 
 * @param destinDir 
 * @return true 
 * @return false 
 */
bool FileSystem::copyBatch(const string &pattern, const string &originDir, const string &destinDir) {
    // Find origin folder
    Folder *origin = root->getFolderByName(originDir);
    if (!origin) return false;

    // Find destination folder
    Folder *destin = root->getFolderByName(destinDir);
    if (!destin) return false;

    return origin->copyBatch(pattern, destin);
}

/**
 * @brief Remove all occurences of a file or folder, determined by type
 * 
 * @param name Name of the folder/file to remove
 * @param type What to remove (folder/file)
 * @return true Success (element deleted successfully)
 * @return false Failure (either the element didn't exist or failed to be deleted)
 */
bool FileSystem::removeAll(const string &name, ElementType type) {
    if (name.empty()) return false;
    if (!root) return false;

    bool removedSomething = false;

    if (type == ElementType::File) {
        while (true) {
            // Find file's parent
            Folder *parent = root->getFolderByFileName(name);
            if (!parent) break;
            
            // unique_ptr is destroyed automatically
            unique_ptr<File> removed = parent->removeFile(name);
            if (!removed) break;
            removedSomething = true;
        }
        return removedSomething;
    }
    else if (type == ElementType::Folder) {
        while (true) {
            // Find folder to be deleted
            Folder *oldF = root->getFolderByName(name);
            if (!oldF) break;
    
            Folder *parent = oldF->getParent();
            // Check if the folder being removed is the root
            if (!parent) {
                clear(); // reset filesystem
                removedSomething = true;
                break;
            }
            
            // unique_ptr is destroyed automatically
            unique_ptr<Folder> removed = parent->removeFolder(name);
            if (!removed) break;
            removedSomething = true;
        }
        return removedSomething;
    }

    return false;
}

/**
 * @brief Rename all files to "newName"
 * 
 * @param currentName Current name to be changed
 * @param newName New name WITHOUT EXTENSION
 */
void FileSystem::renameAllFiles(const string &currentName, const string &newName) {
    if (currentName.empty() || newName.empty()) return;
    if (currentName == newName) return;

    while (true) {
        // Find file's parent
        File *f = root->getFileByName(currentName);
        if (!f) break;

        f->setName(newName);
    }
}

/**
 * @brief Get a file's date as a string
 * 
 * @note Caller must delete return value
 * 
 * @param name Name of the file to search for
 * @return string* Date formatted if found, else nullptr
 */
string *FileSystem::getFileDate(const string &name) {
    if (name.empty()) return nullptr;

    File *f = root->getFileByName(name);
    if (!f) return nullptr;

    return new string(f->getDate().getFormattedDate());
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
 * @brief Check if there are any files with the same name on the system
 * 
 * @return true There's duplicate files
 * @return false There's no duplicate files
 */
bool FileSystem::checkDupFiles() {
    unordered_set<string> names;
    return root->checkDupFiles(names);
}

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

