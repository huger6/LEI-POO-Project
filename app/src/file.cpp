#include "file.hpp"

using namespace std;

/**
 * @brief Construct a new File:: File object
 * 
 * @param filename Name of thye file with extension
 */
File::File(const string &filename) : filename(Filename(filename)) {
    size = 0;
}

/**
 * @brief Construct a new File:: File object
 * 
 * @param filename Name of the file with extension
 * @param size Size occupied by the file
 */
File::File(const string &filename, const uintmax_t size = 0) : filename(Filename(filename)), size(size) {

}

// XML

/**
 * @brief Save file's content to XML format
 * 
 * @param out Write-to file
 * @param indent Level of indentation
 */
void File::saveToXML(ostream &out, uint16_t indent) const {
    std::string space(indent, '\t');
    out << space << "<File name=\"" << filename.getFullname() << "\" size=\"" << size << "\"/>\n";
}

// Getters

/**
 * @brief Get the fullname of the file
 * 
 * @return const string Name.extension
 */
const string File::getName() const { return filename.getFullname(); }

/**
 * @brief Get the size occupied by the file
 * 
 * @return uintmax_t Size
 */
uintmax_t File::getSize() const { return size; }