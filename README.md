# File System Manager --- OOP Project 2025/2026

This project was developed for the **Object-Oriented Programming** curricular unit of the **Bachelor's in Computer Engineering (ESTGV, IPV)** --- 2nd year, 1st semester (2025/2026).

The goal is to implement, in modern C++, an in-memory **file system manager**, supporting directory navigation, file manipulation, XML import/export, recursive operations, and several analytical features.
The development follows a strict **object-oriented** architecture, with careful memory management and modular design.

------------------------------------------------------------------------

## 🧰 Technologies & Tools

-   **Language:** C++26
-   **Compiler:**
    -   *gcc.exe (MinGW-W64 x86_64-ucrt-posix-seh, built by Brecht Sanders, r4) 15.1.0*
    -   Fully tested on **Windows**
    -   Linux compatibility still pending
-   **Build system:** CMake 4.1.1
-   **External dependency:**
    -   [`tinyxml2`](https://github.com/leethomason/tinyxml2) (files included locally)
-   **Paradigm:** Object-Oriented Programming

------------------------------------------------------------------------

## 📁 Project Structure

    app/
     ├── include/
     │    ├── date.hpp
     │    ├── file.hpp
     │    ├── filename.hpp
     │    ├── fileSystem.hpp
     │    ├── folder.hpp
     │    ├── menu.hpp
     │    ├── systemConfig.hpp
     │    ├── tinyxml2.h
     │    └── utils.hpp
     │
     └── src/
          ├── date.cpp
          ├── file.cpp
          ├── filename.cpp
          ├── fileSystem.cpp
          ├── folder.cpp
          ├── main.cpp
          ├── menu.cpp
          └── tinyxml2.cpp

The architecture is designed for clarity, modularity, and strict separation between interface (`.hpp`) and implementation (`.cpp`).

------------------------------------------------------------------------

## 🎯 Project Objectives

The core objective is to implement a complete in-memory **FileSystem**
class capable of:

-   Representing a hierarchical folder and file structure
-   Loading a directory from disk
-   Searching, counting, renaming, and removing elements
-   Moving files and directories
-   Exporting/importing using XML
-   Generating a console/file tree view
-   Detecting duplicates
-   Batch copying with sequential numbering
-   Providing memory usage statistics
-   Managing creation/modification dates

All methods required by the official project specification are implemented faithfully.

------------------------------------------------------------------------

## 🧩 Key Functionalities (Specification Summary)

The FileSystem supports operations including:

-   Load a directory into memory
-   Count files and directories
-   Determine directories with more/less elements
-   Find the largest file
-   Search for files or directories
-   Remove all directories or files recursively
-   Save/Load the entire system in XML
-   Move files/directories
-   Retrieve file dates
-   Generate a tree-like representation
-   Search and collect all matching files/directories
-   Rename multiple files
-   Detect duplicate filenames
-   Perform batch copy with auto-increment suffixes

------------------------------------------------------------------------

## 🔧 Build Instructions

The project uses **CMake** for portability and ease of use.

### 1️⃣ Generate build files

``` bash
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
```

### 2️⃣ Compile

``` bash
cmake --build build --parallel 4
```

The final executable will be available inside:

    build

------------------------------------------------------------------------

## 🧪 Compatibility

-   ✔️ Fully tested on **Windows 10/11**
-   ⚠️ Linux support expected but not validated yet
-   ✔️ Uses C++26 features
-   ✔️ Includes `tinyxml2` locally within the project

------------------------------------------------------------------------

## 📚 Concepts Applied

The project makes strong use of:

-   Object-Oriented Design
-   Encapsulation
-   Modular architecture
-   RAII and safe memory management
-   Smart pointers (`unique_ptr`)
-   Recursive directory traversal
-   Operator and method overloading
-   XML processing via `tinyxml2`
-   Separation of logic (core classes) and interaction (Menu system)

------------------------------------------------------------------------

## 👥 Authors

This project was developed by students of the **Computer Engineering Bachelor's Degree** (ESTGV, IPV) as part of the **Object-Oriented Programming** course, academic year **2025/2026**.

**Authors:**
- Hugo Afonso *30032*
- Mateus Silva *29989*
- Rodrigo Martins *30773*

For academic purposes only. All rights reserved to the respective authors.

------------------------------------------------------------------------

## 📄 Additional Notes

-   `tinyxml2` is bundled and used under its permissive license.
-   The project structure is organized for readability and maintainability.
-   The Menu class provides a central interaction point with clean separation from logic.

