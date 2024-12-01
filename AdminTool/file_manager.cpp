#include "pch.h"
#include "file_manager.h"

void ManageFiles() {
    std::string path = "C:\\Users\\Public\\Documents";
    std::cout << "Listing Files in: " << path << std::endl;

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;
    }
}
