#include "folder.hpp"

using namespace std;

Folder::Folder(string name, uintmax_t size = 0, Folder *father = nullptr) : name(name), size(size) {
    root = father;
}

bool Folder::load(const fs::path& path) {
    if (!fs::exists(path) || !fs::is_directory(path))
        return false;

    for (const auto &entry : fs::directory_iterator(path)) {
        if (entry.is_directory()) {
            // Create new subfolder, 'this' is the father
            unique_ptr<Folder> subfolder = make_unique<Folder>(entry.path().filename().string(), 0, this);
            // Load subfolder's content
            if (subfolder->load(entry.path())) {
                cout << "Folder loaded: " << entry.path().filename().string() << endl;
                // Add folder to its father folders list
                addFolder(move(subfolder));
            }
        }
        else if (entry.is_regular_file()) {
            addFile(make_unique<File>(entry.path().filename().string(), fs::file_size(entry.path())));
            cout << "File loaded: " << entry.path().filename().string() << endl;
        }
    }

    return true;
}

void Folder::addFile(unique_ptr<File> file) {
    files.push_back(move(file));
}
        
void Folder::addFolder(unique_ptr<Folder> folder) {
    subfolders.push_back(move(folder));
}

void Folder::printFolder(ostream &out, ostream *mirror) {
    for (const auto &sub : subfolders) {
        sub->printFolder(out, mirror);
    }
    // Percorrer subfolders e para cada chamar printFolder
    // caso haja ficheiros chamar o printFile
}

// void Folder::tree(uint16_t depth, ostream &out, ostream *mirror) const {
//     string space(depth * SPACES_PER_LEVEL, ' ');
//     string s(depth * SPACES_PER_LEVEL, ' ');

//     s.append("|");
//     s.append("----");
//     out << s << this->getName() << endl;
//     if (mirror) *mirror << s << this->getName() << endl;

//     for (const unique_ptr<Folder>& sub : subfolders) {
//         sub->tree(depth + 1, out, mirror);
//     }

//     for (const unique_ptr<File>& f : files) {
//         out << s << space << f->getName() << endl;
//         if (mirror) *mirror << s << space << f->getName() << endl;
//     }
// }

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
        out << newPrefix << (lastFile ? "└── " : "├── ") << f->getName() << endl;
        if (mirror) *mirror << newPrefix << (lastFile ? "└── " : "├── ") << f->getName() << endl;
    }
}

const string& Folder::getName() const { return name; }

const vector<unique_ptr<File>>& Folder::getFiles() const { return files; }

const vector<unique_ptr<Folder>>& Folder::getSubfolders() const { return subfolders; }