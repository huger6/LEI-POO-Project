#pragma once

#include <iostream>
#include <string>
#include <cstdint>

#include "filename.hpp"
#include "date.hpp"
#include "element.hpp"


/**
 * @brief Handle all file related operations
 * 
 */
class File : public Element {
    public:
        File(const std::string &filename);
        File(const std::string &filename, Date date, const std::uintmax_t size);
        File(const std::string &filename, const std::string &date, const std::uintmax_t size);

        // Setters
        void setDate(const Date &newDate);
        // Getters
        std::uintmax_t getSize() const;
        const Date getDate() const;

        bool isFile() const override { return true; }
        bool isFolder() const override { return false; }
    private:
        std::uintmax_t size;
        Date date;
};

