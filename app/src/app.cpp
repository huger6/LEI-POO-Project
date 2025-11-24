#include "app.hpp"

#include <iostream>
#include <string>
#include <fstream>

#include "input.hpp"
#include "utils.hpp"

/**
 * @brief Construct a new App:: App object
 * 
 */
App::App() : mainMenu("Main Menu", {
                "Load/Save",
                "Statistics",
                "Searchs",
                "Operations",
                "Advanced",
                "Exit"
            }), fs() {}

/**
 * @brief Shows the main menu and calls submenus accordingly
 * 
 */
void App::run() {
    while (true) {
        int option = mainMenu.show();

        switch (option) {
            case 0:
                loadSave();
                break;
            case 1:
                statistics();
                break;
            case 2:
                searchs();
                break;
            case 3:
                operations();
                break;
            case 4:
                advanced();
                break;
            case 5:
                // Are you sure you want to exit the program... 
                if(Menu::askYesNo("Are you sure you want to exit the program")) {
                    Utils::clear();
                    return;
                }
                break;
            default:
                Utils::clear();
                return;
        }
    }
}


// Submenus

/**
 * @brief Shows the Load/Save submenu and applies functionalities
 * 
 */
void App::loadSave() {
    while (true) {
        Menu menu("Load/Save", {
            "Load directory to memory",
            "Load from XML file",
            "Save to XML file",
            "Clear/Reset",
            "Set root path",
            "Back"
        });

        int option = menu.show();

        switch (option) {
            case 0:
                if (fs.getPath().empty()) {
                    // Ask for path and store it
                    std::string path = Input::getString("Insert path to the root directory: ");
                    fs.setPath(path);
                }
                (void) fs.load();
                Input::wait();
                break;
            case 1:
                fs.readFromXML(Input::getString("XML file to read from (with extension): "));
                Input::wait();
                break;
            case 2:
                fs.saveToXML(Input::getString("XML file to save to: "));
                Input::wait();
                break;
            case 3:
                fs.clear();
                std::cout << "FileSystem has been reseted successfuly" << std::endl;
                Input::wait();
                break;
            case 4:
                fs.setPath(Input::getString("Insert path to the root directory: "));
                std::cout << "FileSystem path has been set successfuly" << std::endl;
                Input::wait();
                break;
            case 5:
                return;
            default:
                return;
        }
    }
}

/**
 * @brief Shows the Statistics submenu and applies functionalities
 * 
 */
void App::statistics() {
    while (true) {
        Menu menu("Statistics", {
            "Count files",
            "Count folders",
            "Memory usage + files size",
            "Most elements folder",
            "Least elements folder",
            "Largest folder in size",
            "Largest file in size",
            "Back"
        });
        
        int option = menu.show();

        switch (option) {
            case 0: {
                std::uint32_t numFiles = fs.countFiles();
                std::cout << "Number of files: " << numFiles << std::endl;
                Input::wait();
                break;
            }
            case 1: {
                std::uint32_t numFolders = fs.countFolders();
                std::cout << "Number of folders: " << numFolders << std::endl;
                Input::wait();
                break;
            }
            case 2: {
                std::uintmax_t mem = fs.memory();
                std::cout << "Memory usage + file size: " << mem << std::endl;
                Input::wait();
                break;
            }
            case 3: {
                std::string *mostElemFolder = fs.mostElementsFolder();
                std::cout << "Most Elements Folder: " << *mostElemFolder << std::endl;
                Input::wait();
                delete mostElemFolder;
                break;
            }
            case 4: {
                std::string *leastElemFolder = fs.leastElementsFolder();
                std::cout << "Least Elements Folder: " << *leastElemFolder << std::endl;
                Input::wait();
                delete leastElemFolder;
                break;
            }
            case 5: {
                std::string *largestFolder = fs.largestFolder();
                std::cout << "Largest Folder: " << *largestFolder << std::endl;
                Input::wait();
                delete largestFolder;
                break;
            }
            case 6: {
                std::string *largestFile = fs.largestFile();
                std::cout << "Largest File: " << *largestFile << std::endl;
                Input::wait();
                delete largestFile;
                break;
            }
            case 7:
                return;
            default:
                return;
        }
    }
}

/**
 * @brief Shows the Searchs submenu and applies functionalities
 * 
 */
void App::searchs() {
    while (true) {
        Menu menu("Searchs", {
            "Search file (first found)",
            "Search folder (first found)",
            "Search all folders by name",
            "Search all files by name",
            "Back"
        });
        
        int option = menu.show();

        switch (option) {
            case 0: {
                std::optional<std::string> fileFound = fs.search(Input::getString("File to search for (with extension): "), ElementType::File);
                if (fileFound.has_value()) {
                    std::cout << "Matches for the search: " << fileFound.value() << std::endl;
                }
                else {
                    std::cout << "No matches found" << std::endl;
                }
                Input::wait();
                break;
            }
            case 1: {
                std::optional<std::string> folderFound = fs.search(Input::getString("Folder to search for: "), ElementType::Folder);
                if (folderFound.has_value()) {
                    std::cout << "Matches for the search: " << folderFound.value() << std::endl;
                }
                else {
                    std::cout << "No matches found" << std::endl;
                }
                Input::wait();
                break;
            }
            case 2: {
                std::list<std::string> li;
                fs.searchAllFolders(li , Input::getString("Name of the folder to look for: "));

                if (li.empty()) std::cout << "No results found" << std::endl;
                else {
                    std::cout << "Results found:" << std::endl;
                    for (const std::string &folder : li) {
                        std::cout << folder << std::endl;
                    }
                }
                Input::wait();
                break;
            }
            case 3: {
                std::list<std::string> li;
                fs.searchAllFiles(li , Input::getString("Name of the file to look for: "));

                if (li.empty()) std::cout << "No results found" << std::endl;
                else {
                    std::cout << "Results found:" << std::endl;
                    for (const std::string &f : li) {
                        std::cout << f << std::endl;
                    }
                }
                Input::wait();
                break;
            }
            case 4:
                return;
            default:
                return;
        }
    }
}

/**
 * @brief Shows the Operations submenu and applies functionalities
 * 
 */
void App::operations() {
    while (true) {
        Menu menu("Operations", {
            "Remove all files by name",
            "Remove all folders by name",
            "Move file",
            "Move folder",
            "Copy batch of files",
            "Back"
        });
        
        int option = menu.show();

        switch (option) {
            case 0: {
                std::string name = Input::getString("Name of the file to remove: ");
                bool removed = fs.removeAll(name, ElementType::File);
                if (removed)
                    std::cout << "All files with the name " << name << " were removed." << std::endl;
                else
                    std::cout << "File doesn't exist or an error occurred trying to remove it." << std::endl;
                Input::wait();
                break;
            }
            case 1: {
                std::string name = Input::getString("Name of the folder to remove: ");
                bool removed = fs.removeAll(name, ElementType::Folder);
                if (removed)
                    std::cout << "All folders with the name " << name << " were removed." << std::endl;
                else
                    std::cout << "Folder doesn't exist or an error occurred trying to remove it." << std::endl;
                Input::wait();
                break;
            }
            case 2: {
                std::string name = Input::getString("Name of the file to move: ");
                std::string folder = Input::getString("Name of the folder to move the file into: ");

                bool res = fs.moveFile(name, folder);
                if (res)
                    std::cout << "File was moved successfuly to the provided folder." << std::endl;
                else 
                    std::cout << "File couldn't be moved to the destination folder." << std::endl;

                Input::wait();

                break;
            }
            case 3: {
                std::string orig = Input::getString("Name of the folder to move: ");
                std::string dest = Input::getString("Name of the destination folder: ");

                bool res = fs.moveFolder(orig, dest);
                if (res)
                    std::cout << "Folder was moved successfuly to the provided folder." << std::endl;
                else 
                    std::cout << "Folder couldn't be moved to the destination folder." << std::endl;

                Input::wait();

                break;
            }
            case 4: {
                std::string pattern = Input::getString("Pattern to look for: ");
                std::string orig = Input::getString("Folder where to copy from: ");
                std::string dest = Input::getString("Destination folder: ");

                bool res = fs.copyBatch(pattern, orig, dest);
                if (res)
                    std::cout << "Files with pattern \"" << pattern << "\" were copied with success to the destination folder." << std::endl;
                else 
                    std::cout << "There's no files with the matching pattern in the provided folder or an error has occurred." << std::endl;
                Input::wait();

                break;
            }
            case 5:
                return;
            default:
                return;
        }
    }
}

/**
 * @brief Shows the Advanced submenu and applies functionalities
 * 
 */
void App::advanced() {
    while (true) {
        Menu menu("Advanced", {
            "Tree",
            "Obtain a file's date",
            "Rename all files by name",
            "Scan system for duplicate files",
            "Back"
        });
        
        int option = menu.show();

        switch (option) {
            case 0: {
                if (Menu::askYesNo("Do you want to save the output to a file? ", true)) {
                    std::string outStr = Input::getString("Filename: ", true);
                    if (outStr.empty()) outStr = "tree.txt";
                    else {
                        // Ensure correct extension
                        Filename fname(outStr);
                        if (fname.getExtension() != "txt") {
                            fname.setExtension("txt");
                        }
                        outStr = fname.getFullname();
                    }
                    std::ofstream out(outStr);
                    fs.tree(std::cout, &out);
                }
                else
                    fs.tree(std::cout, nullptr);

                Input::wait();
                break;
            }
            case 1: {
                std::string file = Input::getString("File to look for: ");

                std::string *fDate = fs.getFileDate(file);
                if (fDate) 
                    std::cout << "Date of the file \"" << file << "\": " << *fDate << std::endl;
                else 
                    std::cout << "Date could not be retrieved, file doesn't exist or an error has ocurred" << std::endl;

                Input::wait();
                delete fDate;
                break;
            }
            case 2: {
                std::string oldName = Input::getString("Name of the files to replace: ");
                std::string newName = Input::getString("New name: ");

                fs.renameAllFiles(oldName, newName);

                std::cout << "All files with name \"" << oldName << "\" were renamed successfuly to \"" << newName << "\"." << std::endl;
                
                Input::wait();
                break;
            }
            case 3: {
                if (fs.checkDupFiles())
                    std::cout << "Duplicate files were found." << std::endl;
                else
                    std::cout << "There's no duplicate files" << std::endl;

                Input::wait();
                break;
            }
            case 4:
                return;
            default:
                return;
        }
    }
}