#ifndef __GENERATOR

#define __GENERATOR

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include <filesystem> // Include for std::filesystem
#include <fstream>
#include "json.hpp"
#include "utility.hpp"
#include "global.hpp"
#include "SQL_PARSER.hpp"

namespace CommandRunner
{

    void generateCreateTableStatement(const std::unique_ptr<CreateStatement> &stmt)
    {
        // Step 1: Convert column definitions to JSON
        JSONParser::JSONArray columnArray;

        for (const auto &col : stmt->columns)
        {
            JSONParser::JSONObject colJson = {
                {"name", JSONParser::JSONValue(col.name)},
                {"type", JSONParser::JSONValue(col.type)}};

            // Extract length if it's a VARCHAR with size, e.g., varchar(255)
            if (col.type.find("varchar(") != std::string::npos)
            {
                size_t start = col.type.find("(") + 1;
                size_t end = col.type.find(")");
                if (start != std::string::npos && end != std::string::npos && end > start)
                {
                    std::string lengthStr = col.type.substr(start, end - start);
                    try
                    {
                        int length = std::stoi(lengthStr);
                        colJson["length"] = JSONParser::JSONValue(length);
                        colJson["type"] = JSONParser::JSONValue("varchar"); // strip size from type
                    }
                    catch (...)
                    {
                        throw std::runtime_error("Invalid VARCHAR length");
                    }
                }
            }

            // Convert constraints
            JSONParser::JSONArray constraintArray;
            for (const auto &c : col.constraints)
            {
                switch (c)
                {
                case ColumnConstraint::NOT_NULL:
                    constraintArray.push_back(JSONParser::JSONValue("not_null"));
                    break;
                case ColumnConstraint::PRIMARY_KEY:
                    constraintArray.push_back(JSONParser::JSONValue("primary_key"));
                    break;
                case ColumnConstraint::UNIQUE:
                    constraintArray.push_back(JSONParser::JSONValue("unique"));
                    break;
                case ColumnConstraint::AUTO_INCREMENT:
                    constraintArray.push_back(JSONParser::JSONValue("auto_increment"));
                    break;
                default:
                    break;
                }
            }

            colJson["constraints"] = JSONParser::JSONValue(constraintArray);
            columnArray.push_back(JSONParser::JSONValue(colJson));
        }

        // Step 0: Check if table already exists in globalTableCache
        if (globalTableCache[currentDatabase].find(stmt->name) != globalTableCache[currentDatabase].end())
        {
            throw std::runtime_error("❌ Table '" + stmt->name + "' already exists in DB '" + currentDatabase + "'");
        }

        // Step 2: Create table JSON
        JSONParser::JSONObject tableJson = {
            {"name", JSONParser::JSONValue(stmt->name)},
            {"columns", JSONParser::JSONValue(columnArray)}};

        // Step 3: Load existing JSON file
        std::string filePath = "./db/" + currentDatabase + ".shivam.db";
        JSONParser parser(filePath);

        if (!parser.loadFromFile())
        {
            throw std::runtime_error("❌ Failed to load DB file: " + filePath);
        }

        JSONParser::JSONValue root = parser.getObject(0);
        if (!std::holds_alternative<JSONParser::JSONObject>(root.value))
        {
            throw std::runtime_error("Root of DB JSON must be an object");
        }

        auto &dbObj = std::get<JSONParser::JSONObject>(root.value);

        // Step 4: Add table to the JSON
        if (dbObj.find("tables") != dbObj.end() &&
            std::holds_alternative<JSONParser::JSONArray>(dbObj["tables"].value))
        {

            auto &tables = std::get<JSONParser::JSONArray>(dbObj["tables"].value);
            tables.push_back(JSONParser::JSONValue(tableJson));
        }
        else
        {
            dbObj["tables"] = JSONParser::JSONValue(JSONParser::JSONArray{
                JSONParser::JSONValue(tableJson)});
        }

        // Step 5: Save back to file
        parser.clear();
        parser.appendValue(JSONParser::JSONValue(dbObj));

        if (!parser.saveToFile())
        {
            throw std::runtime_error("❌ Failed to save DB JSON file");
        }

        // Optional: Update in-memory cache too
        globalTableCache[currentDatabase][stmt->name] = {}; // You can populate columns later

        std::cout << "✅ Table '" << stmt->name << "' added to DB '" << currentDatabase << "' successfully.\n";
        std::string tablename = stmt->name;
        std::stringstream indexFile, dataFile;

        indexFile << tableDirectory << "/" << currentDatabase <<"/"<< tablename << ".index";
        dataFile << tableDirectory << "/" << currentDatabase <<"/"<< tablename << ".data";

        MyUtility::createFile(indexFile.str(),"");
        MyUtility::createFile(dataFile.str(),"");
    }


    void generateInsertStatement(){
        
    }

};
#endif