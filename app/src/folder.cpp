#include "folder.hpp"

#include "date.hpp"
#include "utils.hpp"

using namespace std;

/**
 * @brief Construct a new Folder:: Folder object
 * 
 * @param name Name of the folder
 * @param father Folder's parent folder
 */
Folder::Folder(string name, Folder *father = nullptr) : name(name) {
    root = father;
}

/**
 * @brief Load all files and folders to memory on this folder
 * 
 * @param path Path of the current folder to load
 * @return true Folder and all it's content loaded successfuly
 * @return false Path does not exist or it isn't a folder
 */
bool Folder::load(const fs::path& path) {
    if (!fs::exists(path) || !fs::is_directory(path))
        return false;

    for (const auto &entry : fs::directory_iterator(path)) {
        if (entry.is_directory()) {
            // Create new subfolder, 'this' is the father
            unique_ptr<Folder> subfolder = make_unique<Folder>(entry.path().filename().string(), this);
            // Load subfolder's content
            if (subfolder->load(entry.path())) {
                // Add folder to its father folders list
                addFolder(move(subfolder));
            }
        }
        else if (entry.is_regular_file()) {
            Date date = Date::convertFileTime(fs::last_write_time(entry.path()));

            addFile(make_unique<File>(
                entry.path().filename().string(),
                date,
                fs::file_size(entry.path())
            ));
        }
    }

    return true;
}

/**
 * @brief Add a file to the folder
 * 
 * @param file File to add
 */
void Folder::addFile(unique_ptr<File> file) {
    if (!file) return;

    string name = file->getName().getFullname();

    uint16_t counter = 1;

    while (hasFile(name)) {
        file->getName().generateSequentialName(counter);
        counter++;
    }

    files.push_back(move(file));
}

/**
 * @brief Removes a file from the current folder and returns its unique_ptr
 * 
 * @param name Name of the file to remove
 * @return unique_ptr<File> File if found, else nullptr
 */
unique_ptr<File> Folder::removeFile(const string& name) {
    for (vector<unique_ptr<File>>::iterator it = files.begin(); it != files.end(); ++it) {
        if ((*it)->getName().getFullname() == name) {
            unique_ptr<File> file = move(*it);
            files.erase(it);
            return file;
        }
    }
    return nullptr;
}

/**
 * @brief Removes a folder from the current folder (can be the folder itself) and returns its unique_ptr
 * 
 * @note This method is non recursive, as it only checks for direct children of the current folder
 * 
 * @param name Name of the folder to remove
 * @return unique_ptr<Folder> Folder if found, else nullptr
 */
unique_ptr<Folder> Folder::removeFolder(const string& name) {
    for (vector<unique_ptr<Folder>>::iterator it = subfolders.begin(); it != subfolders.end(); ++it) {
        if ((*it)->getName() == name) {
            unique_ptr<Folder> f = move(*it);
            subfolders.erase(it);
            return f;
        }
    }
    return nullptr;
}

/**
 * @brief Add a subfolder to the folder
 * 
 * @param folder (sub)Folder to add
 */
void Folder::addFolder(unique_ptr<Folder> folder) {
    folder->setParent(this);
    subfolders.push_back(move(folder));
}

/**
 * @brief 
 * 
 * @param pattern Pattern to find in the name of the file
 * @param destin Destination folder 
 * @return true Copy was successfull
 * @return false No file matching the pattern was found or the copy of the files found was not successful
 */
bool Folder::copyBatch(const string &pattern, Folder *destin) {
    bool copied = false;
    // Search in this folder's files
    for (const unique_ptr<File> &f : files) {
        if (Utils::hasPattern(f->getName().getFullname(), pattern)) {
            string cName = f->getName().getFullname();
            Date cDate = f->getDate();
            uintmax_t cSize = f->getSize();

            unique_ptr<File> copy = make_unique<File>(cName, cDate, cSize);
            if (!copy) continue;

            destin->addFile(move(copy));
            copied = true;
        }
    }

    // Check subfolders
    for (const unique_ptr<Folder> &sub : subfolders) {
        if (sub->copyBatch(pattern, destin)) {
            copied = true;
        }
    }

    return copied;
}

/**
 * @brief Get number of files
 * 
 * @return uint32_t Number of files
 */
uint32_t Folder::countFiles() const {
    uint32_t foldersFiles = static_cast<uint32_t>(files.size());

    // SubFolders Files
    for (const unique_ptr<Folder>& sub : subfolders) {
        foldersFiles += sub->countFiles();
    }
    return foldersFiles;
}

/**
 * @brief Get number of folders
 * 
 * @return uint32_t Number of folders
 */
uint32_t Folder::countFolders() const {
    uint32_t folders = static_cast<uint32_t>(subfolders.size());
    // SubFolders subfolders
    for (const unique_ptr<Folder>& sub : subfolders) {
        folders += sub->countFolders();
    }
    return folders;
}

/**
 * @brief Check for duplicate files in this folder
 * 
 * @param names Names found so far
 * @return true There's duplicates
 * @return false There's no duplicates
 */
bool Folder::checkDupFiles(unordered_set<string>& names) {
    // Check this folder
    for (const unique_ptr<File>& f : files) {
        const string& name = f->getName().getFullname();

        // Check if name exists in names
        if (!names.insert(name).second) {
            return true;
        }
        // Note: names.insert(name) already inserts the name on the uset unless !.second (returns false if already existed)
    }

    // Subfolders
    for (const unique_ptr<Folder>& sub : subfolders) {
        if (sub->checkDupFiles(names)) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Output Windows like tree command for the current folder
 * 
 * @param prefix Prefix to the output string
 * @param isLast Wether it is the last file/folder of the current folder
 * @param out Output file
 * @param mirror Output file mirror, if needed
 */
void Folder::tree(const string &prefix, bool isLast, ostream &out, ostream *mirror) const {
    // Connector and prefix
    out << prefix << (isLast ? "└── " : "├── ") << getName() << endl;
    if (mirror) *mirror << prefix << (isLast ? "└── " : "├── ") << getName() << endl;

    // Next level prefix
    string newPrefix = prefix + (isLast ? "    " : "│   ");

    // SubFolders
    for (size_t i = 0; i < subfolders.size(); ++i) {
        bool lastSub = (i == subfolders.size() - 1 && files.empty());
        subfolders[i]->tree(newPrefix, lastSub, out, mirror);
    }

    // Files
    for (size_t i = 0; i < files.size(); ++i) {
        bool lastFile = (i == files.size() - 1);
        const auto &f = files[i];
        out << newPrefix << (lastFile ? "└── " : "├── ") << f->getName().getFullname() << endl;
        if (mirror) *mirror << newPrefix << (lastFile ? "└── " : "├── ") << f->getName().getFullname() << endl;
    }
}

/**
 * @brief Get memory utilization + size of the folder
 * 
 * @return uintmax_t Memory
 */
uintmax_t Folder::memory() const {
    uintmax_t mem = 0;

    // This folder's memory
    mem += sizeof(Folder);
    
    // SubFolders
    for (const unique_ptr<Folder>& sub : subfolders) {
        mem += sub->memory();
        mem += sizeof(unique_ptr<Folder>);
    }

    // Files
    for (const unique_ptr<File>& f : files) {
        mem += f->getSize();
        mem += sizeof(unique_ptr<File>);
    }
    return mem;
}

/**
 * @brief Finds the folder or subfolder with the most number of elements
 * 
 * @return const Folder* Folder found 
 */
const Folder *Folder::mostElementsFolder() const {
    const Folder *maxFolder = this;
    size_t maxCount = files.size() + subfolders.size();

    for (const unique_ptr<Folder>& sub : subfolders) {
        const Folder *candidate = sub->mostElementsFolder();
        size_t candidateCount = candidate->files.size() + candidate->subfolders.size();

        if (candidateCount > maxCount) { // keeps the first
            maxCount = candidateCount;
            maxFolder = candidate;
        }
    }
    return maxFolder;
}

/**
 * @brief Finds the folder or subfolder with the least number of elements
 * 
 * @return const Folder* Folder found 
 */
const Folder *Folder::leastElementsFolder() const {
    const Folder *minFolder = this;
    size_t minCount = files.size() + subfolders.size();

    for (const unique_ptr<Folder>& sub : subfolders) {
        const Folder *candidate = sub->leastElementsFolder();
        size_t candidateCount = candidate->files.size() + candidate->subfolders.size();

        if (candidateCount < minCount) { // keeps the first
            minCount = candidateCount;
            minFolder = candidate;
        }
    }
    return minFolder;
}

/**
 * @brief Get the largest file in size
 * 
 * @return const File* Largest file
 */
const File *Folder::largestFile() const {
    const File *largest = nullptr;
    uintmax_t largestSize = 0;

    for (const unique_ptr<File>& f : files) {
        if (f->getSize() > largestSize) {
            largestSize = f->getSize();
            largest = f.get();
        }
    }

    for (const unique_ptr<Folder>& sub : subfolders) {
        const File *candidate = sub->largestFile();
        if (candidate && candidate->getSize() > largestSize) {
            largestSize = candidate->getSize();
            largest = candidate;
        }
    }

    return largest;
}

/**
 * @brief Get the largest folder in size
 * 
 * @return const Folder* Largest folder
 */
const Folder *Folder::largestFolder() const {
    const Folder *largest = this;
    uintmax_t largestSize = files.size() + subfolders.size();

    for (const unique_ptr<Folder>& sub : subfolders) {
        const Folder *candidate = sub->largestFolder();
        uintmax_t candidateSize = candidate->files.size() + candidate->subfolders.size();

        if (candidateSize > largestSize) { // keeps the first
            largestSize = candidateSize;
            largest = candidate;
        }
    }
    return largest;
}

// XML

/**
 * @brief Save a folder to XML document 'doc'
 * 
 * @param doc Document in tinyxml2 format
 * @param parentElem Parent folder in tinyxml2 format
 */
void Folder::saveToXML(xml::XMLDocument &doc, xml::XMLElement *parentElem) const {
    // Create element for this folder
    xml::XMLElement *dirElem = doc.NewElement("Folder");
    dirElem->SetAttribute("name", name.c_str());
    parentElem->InsertEndChild(dirElem);

    // Write all files
    for (const unique_ptr<File> &f : files) {
        xml::XMLElement *fileElem = doc.NewElement("File");
        fileElem->SetAttribute("name", f->getName().getFullname().c_str());
        fileElem->SetAttribute("size", static_cast<unsigned long long>(f->getSize()));
        fileElem->SetAttribute("date", f->getDate().getFormattedDate().c_str());
        dirElem->InsertEndChild(fileElem);
    }

    // Write all subfolders
    for (const unique_ptr<Folder> &sub : subfolders) {
        sub->saveToXML(doc, dirElem);
    }
}

/**
 * @brief Load from an XML file to memory
 * 
 * @param dirElem Current folder to load in tinyxml2 format
 */
void Folder::readFromXML(xml::XMLElement *dirElem) {
    if (!dirElem) return;
    // Clear existing data (optional but recommended)
    files.clear();
    subfolders.clear();

    // Load all files
    for (xml::XMLElement *fileElem = dirElem->FirstChildElement("File"); fileElem != nullptr; fileElem = fileElem->NextSiblingElement("File")) {
        const char* fname = fileElem->Attribute("name");
        unsigned long long size = 0;
        // const char* dateAttr = fileElem->Attribute("date");

        fileElem->QueryUnsigned64Attribute("size", &size);

        const char* dateStr = fileElem->Attribute("date");

        // Create file
        files.push_back(make_unique<File>(fname ? fname : "Unnamed", dateStr, size));
    }

    // Load all subdirectories
    for (xml::XMLElement *subElem = dirElem->FirstChildElement("Folder"); subElem != nullptr; subElem = subElem->NextSiblingElement("Folder")) {
        const char *subName = subElem->Attribute("name");
        // Create the folder with the provided name
        unique_ptr<Folder> subfolder = make_unique<Folder>(subName ? subName : "Unnamed", this);
        subfolder->readFromXML(subElem);
        subfolders.push_back(move(subfolder));
    }
}

// Search

/**
 * @brief Get the path to the folder whose name is 'name'
 * 
 * @param name Name of the folder to search
 * @return string Path of the folder found or "" if not found
 */
string Folder::searchFolder(const string& name) const {
    // If this folder matches, return it
    if (this->getName() == name) return this->getName() + "/";

    // Search subfolders recursively and return the first non-empty path
    for (const unique_ptr<Folder>& sub : subfolders) {
        string path = sub->searchFolder(name);
        if (!path.empty()) return this->getName() + "/" + path;
    }

    // Not found
    return "";
}

/**
 * @brief Search all folders whose name is 'name' and store the path in 'li'
 * 
 * @param li List where to store the paths
 * @param name Name to search
 * @param path Initial path, "" if calling on root
 */
void Folder::searchAllFolders(list<string> &li, const string& name, const string& path) const {
    // Set current path
    string currentPath = path.empty() ? getName() : path + "/" +  getName();

    // Check if this folder matches
    if (getName() == name) {
        li.push_back(currentPath + "/");
    }

    // Search in all subfolders
    for (const unique_ptr<Folder>& sub : subfolders) {
        sub->searchAllFolders(li, name, currentPath);
    }
}

/**
 * @brief Get the path to the file whose name is 'name'
 * 
 * @param name Name of the file to search
 * @return string Path of the file found or "" if not found
 */
string Folder::searchFile(const string& name) const {
    // Files
    for (const unique_ptr<File>& f : files) {
        if (f->getName().getFullname() == name) return this->getName() + "/" + f->getName().getFullname();
    }

    // Subfolders
    for (const unique_ptr<Folder>& sub : subfolders) {
        string path = sub->searchFile(name);
        if (!path.empty()) return this->getName() + "/" + path;
    }

    // Not found
    return "";
}

/**
 * @brief Search all files whose name is 'name' and store the path in 'li'
 * 
 * @param li List where to store the paths
 * @param name Name to search
 * @param path Initial path, "" if calling on root
 */
void Folder::searchAllFiles(list<string> &li, const string& name, const string& path) const {
    // Set current path
    string currentPath = path.empty() ? getName() : path + "/" +  getName();

    // Check if this folder matches
    for (const unique_ptr<File>& f : files) {
        if (f->getName().getFullname() == name) {
            li.push_back(currentPath + "/" + name);
        }
    }
    
    // Search in all subfolders
    for (const unique_ptr<Folder>& sub : subfolders) {
        sub->searchAllFiles(li, name, currentPath);
    }
}


/**
 * @brief Check if there a file in this folder (does not check subfolders)
 * 
 * @param name Name to search for
 * @return true File exists
 * @return false File does not exist
 */
bool Folder::hasFile(const std::string &name) const {
    for (const unique_ptr<File>& f : files) {
        if (f->getName().getFullname() == name)
            return true;
    }
    return false;
}

// Setters

/**
 * @brief Set the folder's parent folder
 * 
 * @param parent Pointer to the father
 */
void Folder::setParent(Folder *parent) {
    root = parent;
}

// Getters

/**
 * @brief Get a pointer to the folder by name
 * 
 * @param name Name to search for
 * @return Folder* Folder if found, else nullptr
 */
Folder *Folder::getFolderByName(const string& name) const {
    // If this folder matches, return it
    if (this->getName() == name) return const_cast<Folder *>(this);

    // Search subfolders recursively and return the first non-empty path
    for (const unique_ptr<Folder>& sub : subfolders) {
        Folder *f = sub->getFolderByName(name);
        if (f) return f;
    }

    // Not found
    return nullptr;
}

/**
 * @brief Get a pointer to a file by name search
 * 
 * @param name Name to search for
 * @return File* File if found, else nullptr
 */
File *Folder::getFileByName(const string& name) const {
    // Files
    for (const unique_ptr<File>& f : files) {
        if (f->getName().getFullname() == name) return f.get();
    }

    // Subfolders
    for (const unique_ptr<Folder>& sub : subfolders) {
        File *f = sub->getFileByName(name);
        if (f) return f;
    }

    // Not found
    return nullptr;
}

/**
 * @brief Get the parent folder of a file by its name
 * 
 * @param name Name of the file to search
 * @return Folder* Folder if found, else nullptr
 */
Folder *Folder::getFolderByFileName(const string& name) const {
    // Files
    for (const unique_ptr<File>& f : files) {
        if (f->getName().getFullname() == name) return const_cast<Folder *>(this);
    }

    // Subfolders
    for (const unique_ptr<Folder>& sub : subfolders) {
        Folder *f = sub->getFolderByFileName(name);
        if (f) return f;
    }

    // Not found
    return nullptr;
}

/**
 * @brief Get the parent folder of the current folder
 * 
 * @return Folder* Folder
 */
Folder* Folder::getParent() const { return root; }

/**
 * @brief Get the folder's name
 * 
 * @return const string& Name
 */
const string& Folder::getName() const { return name; }

/**
 * @brief Get all files
 * 
 * @return const vector<unique_ptr<File>>& Files
 */
const vector<unique_ptr<File>>& Folder::getFiles() const { return files; }

/**
 * @brief Get all subfolders
 * 
 * @return const vector<unique_ptr<Folder>>& Subfolders
 */
const vector<unique_ptr<Folder>>& Folder::getSubfolders() const { return subfolders; }