#ifndef __INITIAL_LOAD
#define __INITIAL_LOAD

#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <memory>
#include <stdexcept>
#include <climits>
#include "global.hpp"
#include "utility.hpp"

namespace fs = std::filesystem;

bool checkDBexist(const std::string &name)
{
    std::stringstream file;
    file << dbDirectoryPath;
    file << name;
    file << ".shivam.db";
    return MyUtility::checkIfFileExist(file.str());
}

void initialDatabseLoad()
{
    for (const auto &entry : fs::directory_iterator(dbDirectoryPath))
    {
        if (fs::is_regular_file(entry.status()))
        {
            std::string filename = entry.path().filename().string();

            if (filename.find(".db") != std::string::npos)
            {
                std::string dbname = MyUtility::extractBaseName(filename);
                std::shared_ptr<PythonLikeJSONParser> parser = std::make_shared<PythonLikeJSONParser>();

                // Store parser in global cache
                globalJsonCache[dbname] = parser;

                std::string fullPath = dbDirectoryPath + "/" + filename;
                if (!parser->loadFromFile(fullPath))
                {
                    std::cerr << "Failed to load file: " << fullPath << std::endl;
                    continue;
                }

                try
                {
                    JSONArrayWrapper tablesArray = (*parser)[0][std::string("tables")].asArray();
                    for (size_t i = 0; i < tablesArray.size(); ++i)
                    {
                        std::string tableName = tablesArray[i][std::string("name")].getString();
                        JSONArrayWrapper columnsArray = tablesArray[i][std::string("columns")].asArray();

                        std::vector<std::shared_ptr<TableGlobalColumnNode>> columnNodes;

                        for (size_t j = 0; j < columnsArray.size(); ++j)
                        {
                            std::shared_ptr<TableGlobalColumnNode> node = std::make_shared<TableGlobalColumnNode>();

                            std::string columnDataName = columnsArray[j][std::string("name")].getString();
                            std::string columnDataType = columnsArray[j][std::string("type")].getString();
                            JSONArrayWrapper constraintArray = columnsArray[j][std::string("constraints")].asArray();

                            int length = INT_MAX;
                            bool isUnique = false;
                            bool isPrimary = false;
                            bool autoIncrement = false;
                            bool createIndex = false;

                            for (size_t k = 0; k < constraintArray.size(); ++k)
                            {
                                std::string constraint = constraintArray[k].getString();
                                if (constraint == "primary_key")
                                    isPrimary = true;
                                if (constraint == "auto_increment")
                                    autoIncrement = true;
                                if (constraint == "unique")
                                    isUnique = true;
                                if (constraint == "create_index")
                                    createIndex = true;
                            }

                            if (columnsArray[j].hasKey(std::string("length")))
                            {
                                length = columnsArray[j][std::string("length")];
                            }

                            node->constraint = constraintArray.toStringVector();
                            node->length = length;
                            node->name = columnDataName;
                            node->type = columnDataType;
                            node->autoIncrement = autoIncrement;
                            node->isUnique = isUnique;
                            node->createIndex = createIndex;
                            node->isPrimary = isPrimary;

                            columnNodes.push_back(node);
                        }

                        // Save table columns in globalTableCache
                        globalTableCache[dbname][tableName] = std::move(columnNodes);

                        std::cout << "Loaded table: " << tableName << " from DB: " << dbname << std::endl;
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error accessing 'tables' in " << fullPath << ": " << e.what() << std::endl;
                }
            }
        }
    }
}

void initializePrimaryIndexBtrees() {
    for (const auto& dbPair : globalTableCache) {
        const std::string& dbName = dbPair.first;
        const auto& tables = dbPair.second;

        for (const auto& tablePair : tables) {
            const std::string& tableName = tablePair.first;
            const auto& columns = tablePair.second;

            for (const auto& columnPtr : columns) {
                if (columnPtr->isPrimary || columnPtr->createIndex) {
                    const std::string& columnName = columnPtr->name;
                    const std::string& type = columnPtr->type;

                    TreeVariant tree;

                    if (type == "int") {
                        tree = std::make_shared<BPlusTree<int, IndexNode>>();
                    } else if (type == "string" || type == "varchar" || type == "text") {
                        tree = std::make_shared<BPlusTree<std::string, IndexNode>>();
                    } else {
                        std::cerr << "Unsupported primary key type: " << type
                                  << " for column: " << columnName << std::endl;
                        continue;
                    }

                    dbBtrees[dbName][tableName][columnName] = std::move(tree);

                    std::cout << "Initialized B+ Tree for " << dbName
                              << "." << tableName << "." << columnName << std::endl;
                }
            }
        }
    }
}


#endif // __INITIAL_LOAD
