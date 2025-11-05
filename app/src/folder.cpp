#include "folder.hpp"

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

const string& Folder::getName() const { return name; }

const vector<unique_ptr<File>>& Folder::getFiles() const { return files; }

const vector<unique_ptr<Folder>>& Folder::getSubfolders() const { return subfolders; }