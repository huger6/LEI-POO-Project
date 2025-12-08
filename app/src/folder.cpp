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
Folder::Folder(string name, Folder *father = nullptr) : Element(name) {
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
                add(move(subfolder));
            }
        }
        else if (entry.is_regular_file()) {
            Date date = Date::convertFileTime(fs::last_write_time(entry.path()));

            add(make_unique<File>(
                entry.path().filename().string(),
                date,
                fs::file_size(entry.path())
            ));
        }
    }

    return true;
}

/**
 * @brief Add an element to this folder
 * 
 * @param element Element to be added
 */
void Folder::add(std::unique_ptr<Element> element) {
    if (!element) return;

    if (element->isFile()) {
        File *f = dynamic_cast<File *>(element.get());
        if (f) {
            std::string fname = f->getName().getFullname();
            std::uint16_t counter = 1;
            while (hasFile(fname)) {
                f->getName().generateSequentialName(counter);
                fname = f->getName().getFullname();
                counter++;
            }
        }
        elements.push_back(move(element));
        return;
    }

    if (element->isFolder()) {
        Folder *sub = dynamic_cast<Folder *>(element.get());
        if (sub) sub->setParent(this);
        elements.push_back(move(element));
        return;
    }

    // Fallback
    elements.push_back(std::move(element));
}

/**
 * @brief Remove an element and return its ownership
 * 
 * @param name Name of the element to be removed
 * @param type Type of the element to be removed
 * @return std::unique_ptr<Element> Ownership or nullptr if failure
 */
std::unique_ptr<Element> Folder::remove(const std::string& name, ElementType type) {
    for (auto it = elements.begin(); it != elements.end(); ++it) {
        if (type == ElementType::File && (*it)->isFile()) {
            File *f = dynamic_cast<File*>((*it).get());
            
            if (f && f->getName().getFullname() == name) {
                std::unique_ptr<Element> el = std::move(*it);
                elements.erase(it);
                return el;
            }
        }
        else if (type == ElementType::Folder && (*it)->isFolder()) {
            Folder *fo = dynamic_cast<Folder*>((*it).get());
            if (fo && fo->getName() == name) {
                std::unique_ptr<Element> el = std::move(*it);
                elements.erase(it);
                return el;
            }
        }
    }
    return nullptr;
}

/**
 * @brief Copy a batch of files to another folder
 * 
 * @param pattern Pattern to find in the name of the file
 * @param destin Destination folder 
 * @return true Copy was successfull
 * @return false No file matching the pattern was found or the copy of the files found was not successful
 */
bool Folder::copyBatch(const string &pattern, Folder *destin) {
    if (!destin) return false;

    bool copied = false;

    // Iterate over generic elements
    for (const unique_ptr<Element> &el : elements) {
        if (!el) continue;

        if (el->isFile()) {
            File *f = dynamic_cast<File *>(el.get());
            if (!f) continue;

            if (Utils::hasPattern(f->getName().getFullname(), pattern)) {
                string cName = f->getName().getFullname();
                Date cDate = Date::now(); // update date
                uintmax_t cSize = f->getSize();

                unique_ptr<File> copy = make_unique<File>(cName, cDate, cSize);
                destin->add(move(copy));
                copied = true;
            }
        }
        else if (el->isFolder()) {
            Folder *sub = dynamic_cast<Folder*>(el.get());
            if (sub && sub->copyBatch(pattern, destin)) {
                copied = true;
            }
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
    uint32_t count = 0;

    for (const unique_ptr<Element> &el : elements) {
        if (!el) continue;

        if (el->isFile()) {
            File *f = dynamic_cast<File *>(el.get());
            if (!f) continue;

            count++;
        }
        else if (el->isFolder()) {
            Folder *sub = dynamic_cast<Folder *>(el.get());
            if (sub) {
                count += sub->countFiles();
            }
        }
    }
    return count;
}

/**
 * @brief Get number of folders
 * 
 * @return uint32_t Number of folders
 */
uint32_t Folder::countFolders() const {
    uint32_t count = 0;

    for (const unique_ptr<Element> &el : elements) {
        if (!el) continue;

        if (el->isFile()) continue;
        else if (el->isFolder()) {
            Folder *sub = dynamic_cast<Folder *>(el.get());
            if (sub) {
                count += 1 + sub->countFolders();
            }
        }
    }
    return count;
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
    for (const unique_ptr<Element>& el : elements) {
        // Files
        if (el->isFile()) {
            File *f = dynamic_cast<File *>(el.get());
            if (!f) continue;

            const string& name = f->getName().getFullname();
    
            // Check if name exists in names
            if (!names.insert(name).second) {
                return true;
            }
            // Note: names.insert(name) already inserts the name on the uset unless !.second (returns false if already existed)
        }
        // Subfolders
        else if (el->isFolder()) {
            Folder *sub = dynamic_cast<Folder *>(el.get());
            if (!sub) continue;
            
            if (sub->checkDupFiles(names)) 
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
    // Print this folder
    out << prefix << (isLast ? "└── " : "├── ") << getName() << endl;
    if (mirror) *mirror << prefix << (isLast ? "└── " : "├── ") << getName() << endl;

    // Prepare prefix for children
    string newPrefix = prefix + (isLast ? "    " : "│   ");

    // Iterate mixed elements vector
    for (size_t i = 0; i < elements.size(); ++i) {
        bool last = (i == elements.size() - 1);
        const unique_ptr<Element> &el = elements[i];
        if (!el) continue;

        if (el->isFolder()) {
            Folder *sub = dynamic_cast<Folder *>(el.get());
            if (sub) {
                // Recurse into subfolder
                sub->tree(newPrefix, last, out, mirror);
            }
        }
        else if (el->isFile()) {
            File *f = dynamic_cast<File *>(el.get());
            if (f) {
                out << newPrefix << (last ? "└── " : "├── ") << f->getName().getFullname() << endl;
                if (mirror) *mirror << newPrefix << (last ? "└── " : "├── ") << f->getName().getFullname() << endl;
            }
        }
        else {
            // Fallback for unknown Element types
            out << newPrefix << (last ? "└── " : "├── ") << el->getName().getFullname() << endl;
            if (mirror) *mirror << newPrefix << (last ? "└── " : "├── ") << el->getName().getFullname() << endl;
        }
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
    mem += sizeof(*this);
    // Pointers
    mem += elements.size() * sizeof(unique_ptr<Element>);
    
    // SubFolders
    for (const unique_ptr<Element>& el : elements) {
        if (!el) continue;

        mem += sizeof(*el); // object's memory

        if (el->isFile()) {
            File *f = dynamic_cast<File *>(el.get());
            if (f) mem += f->getSize();
        }
        else if (el->isFolder()) {
            Folder *sub = dynamic_cast<Folder *>(el.get());
            if (sub) mem += sub->memory();
        }
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
    size_t maxCount = elements.size();

    for (const unique_ptr<Element>& el : elements) {
        if (!el->isFolder()) continue;

        const Folder *sub = dynamic_cast<const Folder *>(el.get());
        if (!sub) continue;

        const Folder *candidate = sub->mostElementsFolder();

        size_t candidateCount = candidate->elements.size();

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
    size_t minCount = elements.size();

    for (const unique_ptr<Element>& el : elements) {
        if (!el->isFolder()) continue;

        const Folder *sub = dynamic_cast<const Folder *>(el.get());
        if (!sub) continue;

        const Folder *candidate = sub->leastElementsFolder();

        size_t candidateCount = candidate->elements.size();

        if (candidateCount < minCount) { // keep the first if equal
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

    for (const unique_ptr<Element>& el : elements) {
        if (el->isFile()) {
            const File *candidate = dynamic_cast<File *>(el.get());
            if (!candidate) continue;

            if (candidate->getSize() > largestSize) {
                largestSize = candidate->getSize();
                largest = candidate;
            }
        }
        else if (el->isFolder()) {
            const File *candidate = dynamic_cast<const Folder *>(el.get())->largestFile();
            if (!candidate) continue;
            
            if (candidate->getSize() > largestSize) {
                largestSize = candidate->getSize();
                largest = candidate;
            }
        }
    }

    return largest;
}

/**
 * @brief Get the largest folder in size
 * 
 * @return const Folder* Largest folder
 */
const Folder *Folder::largestFolder(bool isRoot = false) const {
    const Folder *largest = isRoot ? nullptr : this;
    
    uintmax_t largestSize = (largest != nullptr) ? elements.size() : 0;

    for (const unique_ptr<Element>& el : elements) {
        if (!el->isFolder()) continue;

        const Folder *sub = dynamic_cast<const Folder *>(el.get());
        if (!sub) continue;

        const Folder *candidate = sub->largestFolder(false);

        if (candidate != nullptr) {
            uintmax_t candidateSize = candidate->elements.size();

            if (largest == nullptr || candidateSize > largestSize) {
                largestSize = candidateSize;
                largest = candidate;
            }
        }
    }
    return largest;
}

/**
 * @brief Remove type element recursively
 * 
 * @param name Name to remove
 * @param type Type of the element
 * @return true Removed with success
 * @return false Failed to remove or name and type don't exist
 */
bool Folder::removeAll(const std::string &name, ElementType type) {
    bool removed = false;

    for (auto it = elements.begin(); it != elements.end(); ) {
        bool erased = false;

        // Check if current element matches criteria for removal
        if (type == ElementType::File && (*it)->isFile()) {
            File *f = dynamic_cast<File*>((*it).get());
            if (f && f->getName().getFullname() == name) {
                it = elements.erase(it);
                removed = true;
                erased = true;
            }
        }
        else if (type == ElementType::Folder && (*it)->isFolder()) {
            Folder *fo = dynamic_cast<Folder*>((*it).get());
            if (fo && fo->getName() == name) {
                it = elements.erase(it);
                removed = true;
                erased = true;
            }
        }

        // If not erased, check recursively if it is a folder
        if (!erased) {
            if ((*it)->isFolder()) {
                Folder *sub = dynamic_cast<Folder*>((*it).get());
                if (sub && sub->removeAll(name, type)) {
                    removed = true;
                }
            }
            ++it;
        }
    }

    return removed;
}

/**
 * @brief Rename all files recursively
 * 
 * @param currentName Current name to change
 * @param newName New name to change to
 */
void Folder::renameAllFiles(const std::string &currentName, const std::string &newName) {
    for (const unique_ptr<Element>& el : elements) {
        if (el->isFile()) {
            File *f = dynamic_cast<File*>(el.get());
            if (f && f->getName().getFullname() == currentName) {
                f->getName().setName(newName);
            }
        }
        else if (el->isFolder()) {
            Folder *sub = dynamic_cast<Folder*>(el.get());
            if (sub) {
                sub->renameAllFiles(currentName, newName);
            }
        }
    }
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
    dirElem->SetAttribute("name", name.getName().c_str());
    parentElem->InsertEndChild(dirElem);

    // Write all files
    for (const unique_ptr<Element> &el : elements) {
        if (el->isFolder()) {
            const Folder *sub = dynamic_cast<const Folder *>(el.get());
            if (!sub) continue;

            sub->saveToXML(doc, dirElem);
        }
        else if (el->isFile()) {
            const File *f = dynamic_cast<const File *>(el.get());
            if (!f) continue;

            xml::XMLElement *fileElem = doc.NewElement("File");
            fileElem->SetAttribute("name", f->getName().getFullname().c_str());
            fileElem->SetAttribute("size", static_cast<unsigned long long>(f->getSize()));
            fileElem->SetAttribute("date", f->getDate().getFormattedDate().c_str());
            dirElem->InsertEndChild(fileElem);
        }
    }

    
}

/**
 * @brief Load from an XML file to memory
 * 
 * @param dirElem Current folder to load in tinyxml2 format
 */
void Folder::readFromXML(xml::XMLElement *dirElem) {
    if (!dirElem) return;
    // Clear existing data
    elements.clear();

    // Load all files
    for (xml::XMLElement *fileElem = dirElem->FirstChildElement("File"); fileElem != nullptr; fileElem = fileElem->NextSiblingElement("File")) {
        const char* fname = fileElem->Attribute("name");
        unsigned long long size = 0;
        // const char* dateAttr = fileElem->Attribute("date");

        fileElem->QueryUnsigned64Attribute("size", &size);

        const char* dateStr = fileElem->Attribute("date");

        // Create file
        elements.push_back(make_unique<File>(fname ? fname : "Unnamed", dateStr, size));
    }

    // Load all subdirectories
    for (xml::XMLElement *subElem = dirElem->FirstChildElement("Folder"); subElem != nullptr; subElem = subElem->NextSiblingElement("Folder")) {
        const char *subName = subElem->Attribute("name");
        // Create the folder with the provided name
        unique_ptr<Folder> subfolder = make_unique<Folder>(subName ? subName : "Unnamed", this);
        subfolder->readFromXML(subElem);
        elements.push_back(move(subfolder));
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
    for (const unique_ptr<Element>& el : elements) {
        if (el->isFolder()) {
            Folder *sub = dynamic_cast<Folder *>(el.get());
            if (!sub) continue;

            string path = sub->searchFolder(name);
            if (!path.empty()) return this->getName() + "/" + path;
        }
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
    for (const unique_ptr<Element>& el : elements) {
        if (el->isFolder()) {
            Folder *sub = dynamic_cast<Folder *>(el.get());
            if (!sub) continue;

            sub->searchAllFolders(li, name, currentPath);
        }
    }
}

/**
 * @brief Get the path to the file whose name is 'name'
 * 
 * @note Searches in this Folder first
 * 
 * @param name Name of the file to search
 * @return string Path of the file found or "" if not found
 */
string Folder::searchFile(const string& name) const {
    // Files
    for (const unique_ptr<Element>& el : elements) {
        if (el->isFile()) {
            File *f = dynamic_cast<File *>(el.get());
            if (!f) continue;

            if (f->getName().getFullname() == name) return this->getName() + "/" + f->getName().getFullname();
        }
    }
    // Folders recursive
    for (const unique_ptr<Element>& el : elements) {
        if (el->isFolder()) {
            Folder *sub = dynamic_cast<Folder *>(el.get());
            if (!sub) continue;

            string path = sub->searchFile(name);
            if (!path.empty()) return this->getName() + "/" + path;
        }
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
    for (const unique_ptr<Element>& el : elements) {
        if (el->isFile()) {
            const File *f = dynamic_cast<const File *>(el.get());
            if (!f) continue;

            if (f->getName().getFullname() == name) {
                li.push_back(currentPath + "/" + name);
            }
        }
    }
    
    // Search in all subfolders
    for (const unique_ptr<Element>& el : elements) {
        if (el->isFolder()) {
            const Folder *sub = dynamic_cast<const Folder *>(el.get());
            if (!sub) continue;

            sub->searchAllFiles(li, name, currentPath);
        }
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
    for (const unique_ptr<Element>& el : elements) {
        if (el->isFile() && el->getName().getFullname() == name)
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
    for (const unique_ptr<Element>& el : elements) {
        if (el->isFolder()) {
            Folder *sub = dynamic_cast<Folder *>(el.get());
            if (!sub) continue;

            Folder *found = sub->getFolderByName(name);
            if (found) return found;
        }
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
    for (const unique_ptr<Element>& el : elements) {
        if (el->isFile()) {
            File *f = dynamic_cast<File *>(el.get());
            if (!f) continue;

            if (f->getName().getFullname() == name) return f;
        }
    }

    // Subfolders
    for (const unique_ptr<Element>& el : elements) {
        if (el->isFolder()) {
            Folder *sub = dynamic_cast<Folder *>(el.get());
            if (!sub) continue;

            File *f = sub->getFileByName(name);
            if (f) return f;
        }
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
    for (const unique_ptr<Element>& el : elements) {
        if (el->isFile()) {
            File *f = dynamic_cast<File *>(el.get());
            if (!f) continue;

            if (f->getName().getFullname() == name) return const_cast<Folder *>(this);
        }
    }

    // Subfolders
    for (const unique_ptr<Element>& el : elements) {
        if (el->isFolder()) {
            Folder *sub = dynamic_cast<Folder *>(el.get());
            if (!sub) continue;

            Folder *found = sub->getFolderByFileName(name);
            if (found) return found;
        }
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
const string Folder::getName() const { return name.getName(); }

