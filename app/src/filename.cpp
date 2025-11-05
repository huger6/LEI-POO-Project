#include "filename.hpp"

Filename::Filename(const string &fullname) {
    name = getName(fullname);
    extension = getExtension(fullname);
}

Filename::Filename(const string &name, const string &extension) 
    : name(name), extension(extension) {
        // Possible extension and name validations here
        // Throw exception if invalid
        // Caller must catch the exception
}

string Filename::getFullname() const {
    return name + '.' + extension;
}

string Filename::getExtension(const std::string& fullname) {
    size_t pos = fullname.rfind('.');
    return (pos != string::npos) ? fullname.substr(pos + 1) : "";
}

string Filename::getName(const string& fullname) {
    size_t pos = fullname.rfind('.');
    return (pos != string::npos) ? fullname.substr(0, pos) : "";
}
