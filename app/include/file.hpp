#pragma once

#include <iostream>
#include <string>
#include <cstdint>

#include "filename.hpp"
#include "date.hpp"

class File {
    public:
        File(const std::string &filename);
        File(const std::string &filename, Date date, const std::uintmax_t size);
        File(const std::string &filename, const std::string &date, const std::uintmax_t size);

        // Setters
        void setName(const std::string &name);
        // Getters
        const Filename getName() const;
        Filename& getName();
        std::uintmax_t getSize() const;
        const Date getDate() const;
    private:
        Filename filename;
        std::uintmax_t size;
        Date date;
};
