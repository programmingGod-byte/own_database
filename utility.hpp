#ifndef MYUTILITY_UTILITY_HPP
#define MYUTILITY_UTILITY_HPP

#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>
#include <filesystem> // Include for std::filesystem
#include <fstream>
#include "json.hpp"   // Assuming this is a necessary include
#include "global.hpp" // Assuming this is a necessary include
#include <utility>
#include "databaseSchemaReader.hpp"
namespace MyUtility
{                                   // Define a namespace called MyUtility
    namespace fs = std::filesystem; // Shorthand for std::filesystem


    std::string extractBaseName(const std::string& filename) {
    fs::path p(filename);
    std::string stem = p.stem().string();  // first .stem() call removes ".db"
    while (fs::path(stem).extension() != "") {
        stem = fs::path(stem).stem().string();  // repeat to strip ".something"
    }
    return stem;  // returns "hello"
}

    void createFile(const std::string &filePath, const std::string &content)
    {
        fs::path parentDir = fs::path(filePath).parent_path();
        if (!parentDir.empty() && !fs::exists(parentDir))
        {
            std::error_code ec;
            if (fs::create_directories(parentDir, ec))
            {
                std::cout << "Created directory: " << parentDir << std::endl;
            }
            else
            {
                throw std::runtime_error("Error creating directory: " + ec.message());
            }
        }

        std::ofstream outfile(filePath); // Create and open the file
        if (outfile.is_open())
        {
            outfile << content; // Write the content to the file
            outfile.close();    // Close the file
        }
        else
        {
            throw std::runtime_error("Error: Could not create or open file '" + filePath + "' for writing");
        }
    }

    bool checkIfFileExist(const std::string &filePath)
    {
        if (fs::exists(filePath))
        {
            return true;
        }
        return false;
    }

    void changeCurrentDb(const std::string &newDbName)
    {

        std::ofstream outfile(currentDbPath);
        if (outfile.is_open())
        {
            std::string content = "{\"current_db\":\"" + newDbName + "\"}";
            outfile << content;
            outfile.close();
        }
        else
        {
            throw std::runtime_error("Error: Could not open file '" + currentDbPath);
        }
    }

    std::pair<bool, std::string> checkIfTableExist(const std::string &table)
    {
        std::stringstream s;
        s << "./db/";
        s << currentDatabase;
        s << ".shivam.db";
        PythonLikeJSONParser parser;

        if (checkIfFileExist(s.str()))
        {
            std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            parser.loadFromFile(s.str());
            JSONArrayWrapper columns = parser[0][std::string("tables")].asArray();
            for(size_t i = 0;i<columns.size();i++){
                std::string tableName = columns[i][std::string("name")];
                if( tableName== table){
                    return std::make_pair(true,"");
                }
            }
            
        }

        return std::make_pair(false, "the database not exist");
    }

} // namespace MyUtility

#endif
