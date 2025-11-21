#include <fstream>

#include "fileSystem.hpp"
#include "systemConfig.hpp"

// using namepsace std should not be used on this file

int main() {
    // Configure terminal to support UTF8 charset
    SystemConfig::setUTF8();
    // Set path here
    // std::string path = "C:\\Users\\hugoa\\Desktop\\Uni\\Licenciatura EI\\2º Ano";
    std::string path = "C:\\Users\\hugoa\\Desktop\\LEI-U\\PINT";
    // Initialize filesystem
    FileSystem fs;
    // Set path
    fs.setPath(path);
    // Load
    fs.load();

    // Load from XML
    // fs.readFromXML("data.xml");

    // Still needs to be tested with a file input and mirror
    std::ofstream out("tree.txt");
    fs.tree(std::cout, &out);
    
    // Stats

    std::cout << "Files: " << fs.countFiles() << std::endl;
    std::cout << "Folders: " << fs.countFolders() << std::endl;
    
    std::cout << "Memory: " << fs.memory() << " bytes" << std::endl;

    std::string *mostElemFolder = fs.mostElementsFolder();
    std::cout << "Most Elements Folder Name: " << *mostElemFolder << std::endl;
    delete mostElemFolder;

    std::string *leastElemFolder = fs.leastElementsFolder();
    std::cout << "Least Elements Folder Name: " << *leastElemFolder << std::endl;
    delete leastElemFolder;

    std::string *largestFile = fs.largestFile();
    std::cout << "Largest File: " << *largestFile << std::endl;
    delete largestFile;

    std::string *largestFolder = fs.largestFolder();
    std::cout << "Largest Folder: " << *largestFolder << std::endl;
    delete largestFolder;

    // Save to XML
    fs.saveToXML(std::string("data.xml"));

    // Search

    std::optional<std::string> search = fs.search("Redes sadas2", ElementType::Folder);

    if (search.has_value()) {
        std::cout << search.value() << std::endl;
    }
    else {
        std::cout << "No matches found" << std::endl;
    }

    std::optional<std::string> file = fs.search("resumAABS.pdf", ElementType::File);

    if (file.has_value()) {
        std::cout << file.value() << std::endl;
    }
    else {
        std::cout << "No matches found" << std::endl;
    }

    std::list<std::string> li;
    fs.searchAllFolders(li , "Consultor");

    for (auto it : li) {
        std::cout << it << std::endl;
    }

    std::list<std::string> li2;
    fs.searchAllFiles(li2 , "sneakpeekit-2-mobiles-grid.pdf");

    for (auto it : li) {
        std::cout << it << std::endl;
    }

    // Move file
    if (fs.moveFile("Browser - Admin.pdf", "Grelhas")) {
        std::cout << "Sucess changing file to Grelhas" << std::endl;
        fs.tree(std::cout, nullptr);
    }
    else {
        std::cout << "Failure changing file to Grelhas" << std::endl;
    }

    // Move folder
    if (fs.moveFolder("Consultor", "Grelhas")) {
        std::cout << "Sucess changing Consultor to Grelhas" << std::endl;
        fs.tree(std::cout, nullptr);
    }
    else {
        std::cout << "Failure changing Consultor to Grelhas" << std::endl;
    }

    // Remove all
    bool remove1 = fs.removeAll("Consultor", ElementType::Folder);
    std::cout << remove1 << std::endl;
    fs.tree(std::cout, nullptr);

    bool remove2 = fs.removeAll("logo-softinsa.svg", ElementType::File);
    std::cout << remove2 << std::endl;
    fs.tree(std::cout, nullptr);
    
    // Rename all files
    fs.renameAllFiles("Browser - Admin.pdf", "I CHANGED THIS");
    fs.renameAllFiles("Tradução de requisitos funcionais em necessidades dos utilizadores - Hugo Afonso 30032.pdf", "I CHANGED THIS");
    fs.tree(std::cout, nullptr);

    // Dups
    std::cout << fs.checkDupFiles() << std::endl;

    // Get file date
    std::string *fDate = fs.getFileDate("Browser - Service Line.pdf");
    std::cout << "File date: " << *fDate << std::endl;
    delete fDate;

    // Copy batch
    std::cout << fs.copyBatch("sneakpeekit", "Grelhas", "PAF") << std::endl;
    fs.tree(std::cout, nullptr);
    
    return 0;
}