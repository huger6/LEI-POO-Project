#pragma once

#include <iostream>
#include <string>

class Filename {
    public:
        Filename(const std::string &fullname);
        Filename(const std::string &name, const std::string &extention);

        
        // Setters
        void setExtension(const std::string &newExtension);
        void setName(const std::string &newName);
        // Getters
        std::string getFullname() const;
        std::string getName() const;
        std::string getExtension() const;
    private:
        std::string name;
        std::string extension;
        std::string getExtension(const std::string& fullname);
        std::string getName(const std::string& fullname);
};