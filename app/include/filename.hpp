#pragma once

#include <iostream>
#include <string>

using namespace std;

class Filename {
    public:
        Filename(const string &fullname);
        Filename(const string &name, const string &extention);

        string getFullname() const;
    protected:
        string name;
        string extension;
    private:
        string getExtension(const string& fullname);
        string getName(const string& fullname);
};