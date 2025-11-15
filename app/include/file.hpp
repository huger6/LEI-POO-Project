#pragma once

#include <iostream>
#include <string>
#include <cstdint>

#include "filename.hpp"
#include "date.hpp"

class File {
    public:
        File(const std::string &filename);
        File(const std::string &filename, const std::uintmax_t size);

        void saveToXML(std::ostream &out, std::uint16_t indent) const;

        // Setters
        void setName(const std::string &name);
        // Getters
        const std::string getName() const;
        std::uintmax_t getSize() const;
    private:
        Filename filename;
        std::uintmax_t size;
        Date createDate;
        Date lastModifiedDate;
};
