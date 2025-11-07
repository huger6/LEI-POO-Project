#include "file.hpp"

using namespace std;

File::File(const string &name) : filename(Filename(name)) {}

File::File(const string &filename, const uintmax_t size = 0) : filename(Filename(filename)), size(size) {

}

void File::printFile(ostream &out, ostream *mirror) {
    
}

const string File::getName() const { return filename.getFullname(); }