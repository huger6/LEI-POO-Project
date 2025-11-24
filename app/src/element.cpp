#include "element.hpp"

using namespace std;

/**
 * @brief Construct a new Element:: Element object
 * 
 * @param name 
 */
Element::Element(const string& name) : name(name) {

}

/**
 * @brief Get the fullname of the file
 * 
 * @return const string Name.extension
 */
const Filename Element::getName() const { return name; }

/**
 * @brief Get filename and allow changes
 * 
 * @return Filename& Filename
 */
Filename& Element::getName() { return name; }

/**
 * @brief Change the name of the element
 * 
 * @param newName New name to attribute
 */
void Element::setName(const string& newName) {
    this->name.setName(newName); 
}

