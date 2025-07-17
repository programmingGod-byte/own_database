#ifndef MYUTILITY_UTILITY_HPP
#define MYUTILITY_UTILITY_HPP

#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>
#include <filesystem>    // Include for std::filesystem
#include <fstream>
#include "json.hpp"      // Assuming this is a necessary include
#include "global.hpp"    // Assuming this is a necessary include

namespace MyUtility { // Define a namespace called MyUtility
    namespace fs = std::filesystem; // Shorthand for std::filesystem

    void createFile(const std::string &filePath, const std::string &content) {
        fs::path parentDir = fs::path(filePath).parent_path();
        if (!parentDir.empty() && !fs::exists(parentDir)) {
            std::error_code ec;
            if (fs::create_directories(parentDir, ec)) {
                std::cout << "Created directory: " << parentDir << std::endl;
            } else {
                throw std::runtime_error("Error creating directory: " + ec.message());
            }
        }

        std::ofstream outfile(filePath); // Create and open the file
        if (outfile.is_open()) {
            outfile << content; // Write the content to the file
            outfile.close(); // Close the file
        } else {
            throw std::runtime_error("Error: Could not create or open file '" + filePath + "' for writing");
        }
    }

    bool checkIfFileExist(const std::string &filePath) {
        if (fs::exists(filePath)) {
            return true;
        }
        return false;
    }

    void changeCurrentDb(const std::string &newDbName) {
        
        std::ofstream outfile(currentDbPath);
        if (outfile.is_open()) {
            std::string content = "{\"current_db\":\"" + newDbName + "\"}";
            outfile << content;
            outfile.close();
        }else{
            throw std::runtime_error("Error: Could not open file '" + currentDbPath);
        }

    }

} // namespace MyUtility

#endif
