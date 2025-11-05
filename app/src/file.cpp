#include "file.hpp"

File::File(const string &name) : filename(Filename(name)) {}

File::File(const string &filename, const uintmax_t size = 0) : filename(Filename(filename)), size(size) {

}