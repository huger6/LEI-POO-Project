#pragma once

#include <iostream>
#include <string>

class Filename {
    public:
        Filename(const std::string &fullname);
        Filename(const std::string &name, const std::string &extention);

        std::string getFullname() const;
    protected:
        std::string name;
        std::string extension;
    private:
        std::string getExtension(const std::string& fullname);
        std::string getName(const std::string& fullname);
};