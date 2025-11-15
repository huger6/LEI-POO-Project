#include "filename.hpp"

using namespace std;

/**
 * @brief Construct a new Filename:: Filename object
 * 
 * @param fullname Name with extension
 */
Filename::Filename(const string &fullname) {
    name = getName(fullname);
    extension = getExtension(fullname);
}

/**
 * @brief Construct a new Filename:: Filename object
 * 
 * @param name Name
 * @param extension Extension
 */
Filename::Filename(const string &name, const string &extension) 
    : name(name), extension(extension) {
        // Possible extension and name validations here
        // Throw exception if invalid
        // Caller must catch the exception
}

/**
 * @brief Change the extension of the file
 * 
 * @param newExtension New extension (without '.')
 */
void Filename::setExtension(const string &newExtension) {
    extension = newExtension;
}

/**
 * @brief Update file's name
 * 
 * @param newName New name to give to the file
 */
void Filename::setName(const string &newName) {
    name = newName;
}

/**
 * @brief Get the fullname of the file
 * 
 * @return string Name.extension
 */
string Filename::getFullname() const {
    return name + '.' + extension;
}

/**
 * @brief Get only the name of the file (no extension)
 * 
 * @return string Name
 */
string Filename::getName() const { return name; }

/**
 * @brief Get only the extension of the file ('.' not included)
 * 
 * @return string Extension
 */
string Filename::getExtension() const { return extension; }

/**
 * @brief Helper to get the extension in a given fullname
 * 
 * @param fullname Fullname to parse
 * @return string Extension
 */
string Filename::getExtension(const std::string& fullname) {
    size_t pos = fullname.rfind('.');
    return (pos != string::npos) ? fullname.substr(pos + 1) : "";
}

/**
 * @brief Helper to get only the name of the file
 * 
 * @param fullname Fullname to parse
 * @return string Name
 */
string Filename::getName(const string& fullname) {
    size_t pos = fullname.rfind('.');
    return (pos != string::npos) ? fullname.substr(0, pos) : "";
}
