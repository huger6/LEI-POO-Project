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
 * @param date Last modified date
 */
File::File(const string &filename, Date date, const uintmax_t size = 0) : filename(Filename(filename)), size(size), date(date) {

}

/**
 * @brief Construct a new File:: File object
 * 
 * @param filename Name of the file with extension
 * @param size Size occupied by the file
 * @param date Last modified date in string format (day/month/year)
 */
File::File(const string &filename, const string &date, const uintmax_t size) : filename(Filename(filename)), size(size), date(date) {
    
}

// Setters

/**
 * @brief Change the name of the file
 * 
 * @param name New name
 */
void File::setName(const string &name) {
    filename.setName(name);
    // Update date
    setDate(Date::now());
}

/**
 * @brief Change the date of the file
 * 
 * @param newDate New date
 */
void File::setDate(const Date &newDate) {
    date = newDate;
}

// Getters

/**
 * @brief Get the fullname of the file
 * 
 * @return const string Name.extension
 */
const Filename File::getName() const { return filename; }

/**
 * @brief Get filename and allow changes
 * 
 * @return Filename& Filename
 */
Filename& File::getName() { return filename; }

/**
 * @brief Get the size occupied by the file
 * 
 * @return uintmax_t Size
 */
uintmax_t File::getSize() const { return size; }

/**
 * @brief Get the name formatted as a string
 * 
 * @return const string Date
 */
const Date File::getDate() const { return date; }

