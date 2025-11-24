#pragma once

#include <string>
#include <ostream>

#include "filename.hpp"

enum class ElementType { Folder, File };

/**
 * @brief Base class for a filesystem element
 * 
 */
class Element {
    public:
        Element(const std::string& name);
        virtual ~Element() = default; 

        virtual bool isFile() const = 0;
        virtual bool isFolder() const = 0;

        // Getters
        const Filename getName() const;
        Filename& getName();
        // Setters
        void setName(const std::string& name);
    protected:
        Filename name;
};

