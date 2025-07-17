#ifndef __META_DATA
#define __META_DATA
#include "json.hpp"

#include "json.hpp"
#include <map>
#include <string>

#include <iostream>
#include <vector>
#include <string>

class DatabaseModifier {
private:
    JSONParser parser;
    std::string filePath;
    
    // Helper to find a table by name
    JSONParser::JSONObject* findTable(JSONParser::JSONValue& schema, const std::string& tableName) {
        if (!std::holds_alternative<JSONParser::JSONObject>(schema.value)) {
            return nullptr;
        }
        
        auto& dbObj = std::get<JSONParser::JSONObject>(schema.value);
        
        if (dbObj.find("tables") == dbObj.end() || 
            !std::holds_alternative<JSONParser::JSONArray>(dbObj["tables"].value)) {
            return nullptr;
        }
        
        auto& tables = std::get<JSONParser::JSONArray>(dbObj["tables"].value);
        
        for (auto& table : tables) {
            if (std::holds_alternative<JSONParser::JSONObject>(table.value)) {
                auto& tableObj = std::get<JSONParser::JSONObject>(table.value);
                
                if (tableObj.find("name") != tableObj.end() &&
                    std::holds_alternative<std::string>(tableObj["name"].value) &&
                    std::get<std::string>(tableObj["name"].value) == tableName) {
                    return &tableObj;
                }
            }
        }
        
        return nullptr;
    }
    
public:
    DatabaseModifier(const std::string& path) : filePath(path), parser(path) {}
    
    // Load database schema from file
    bool loadDatabase() {
        return parser.loadFromFile();
    }
    
    // Save database schema to file
    bool saveDatabase() {
        return parser.saveToFile();
    }
    
    // Print current database schema
    void printDatabase() {
        parser.printAll();
    }
    
    // Add a new column to an existing table
    bool addColumnToTable(const std::string& tableName, 
                         const std::string& columnName,
                         const std::string& columnType,
                         const std::vector<std::string>& constraints = {},
                         int length = -1) {
        
        if (parser.size() == 0) {
            std::cerr << "No database schema loaded!" << std::endl;
            return false;
        }
        
        // Get the schema (assuming single schema in file)
        JSONParser::JSONValue schema = parser.getObject(0);
        
        JSONParser::JSONObject* table = findTable(schema, tableName);
        if (!table) {
            std::cerr << "Table '" << tableName << "' not found!" << std::endl;
            return false;
        }
        
        // Find columns array
        if (table->find("columns") == table->end() || 
            !std::holds_alternative<JSONParser::JSONArray>((*table)["columns"].value)) {
            std::cerr << "Table '" << tableName << "' has no columns array!" << std::endl;
            return false;
        }
        
        auto& columns = std::get<JSONParser::JSONArray>((*table)["columns"].value);
        
        // Create new column
        JSONParser::JSONObject newColumn = {
            {"name", JSONParser::JSONValue(columnName)},
            {"type", JSONParser::JSONValue(columnType)}
        };
        
        if (length > 0) {
            newColumn["length"] = JSONParser::JSONValue(length);
        }
        
        if (!constraints.empty()) {
            JSONParser::JSONArray constraintsArray;
            for (const auto& constraint : constraints) {
                constraintsArray.push_back(JSONParser::JSONValue(constraint));
            }
            newColumn["constraints"] = JSONParser::JSONValue(constraintsArray);
        }
        
        columns.push_back(JSONParser::JSONValue(newColumn));
        
        // Update the parser with modified schema
        parser.clear();
        parser.appendValue(schema);
        
        std::cout << "Added column '" << columnName << "' to table '" << tableName << "'" << std::endl;
        return true;
    }
    
    // Add a new table
    bool addTable(const std::string& tableName, 
                  const std::vector<std::map<std::string, std::variant<std::string, int, std::vector<std::string>>>>& columnDefs) {
        
        if (parser.size() == 0) {
            std::cerr << "No database schema loaded!" << std::endl;
            return false;
        }
        
        JSONParser::JSONValue schema = parser.getObject(0);
        
        if (!std::holds_alternative<JSONParser::JSONObject>(schema.value)) {
            std::cerr << "Invalid schema format!" << std::endl;
            return false;
        }
        
        auto& dbObj = std::get<JSONParser::JSONObject>(schema.value);
        
        if (dbObj.find("tables") == dbObj.end() || 
            !std::holds_alternative<JSONParser::JSONArray>(dbObj["tables"].value)) {
            std::cerr << "No tables array found!" << std::endl;
            return false;
        }
        
        auto& tables = std::get<JSONParser::JSONArray>(dbObj["tables"].value);
        
        // Create columns array
        JSONParser::JSONArray columns;
        for (const auto& colDef : columnDefs) {
            JSONParser::JSONObject column;
            
            for (const auto& [key, value] : colDef) {
                if (std::holds_alternative<std::string>(value)) {
                    column[key] = JSONParser::JSONValue(std::get<std::string>(value));
                } else if (std::holds_alternative<int>(value)) {
                    column[key] = JSONParser::JSONValue(std::get<int>(value));
                } else if (std::holds_alternative<std::vector<std::string>>(value)) {
                    const auto& strVec = std::get<std::vector<std::string>>(value);
                    JSONParser::JSONArray jsonArray;
                    for (const auto& str : strVec) {
                        jsonArray.push_back(JSONParser::JSONValue(str));
                    }
                    column[key] = JSONParser::JSONValue(jsonArray);
                }
            }
            
            columns.push_back(JSONParser::JSONValue(column));
        }
        
        // Create new table
        JSONParser::JSONObject newTable = {
            {"name", JSONParser::JSONValue(tableName)},
            {"columns", JSONParser::JSONValue(columns)}
        };
        
        tables.push_back(JSONParser::JSONValue(newTable));
        
        // Update the parser with modified schema
        parser.clear();
        parser.appendValue(schema);
        
        std::cout << "Added table '" << tableName << "'" << std::endl;
        return true;
    }
    
    // Change database name
    bool changeDatabaseName(const std::string& newName) {
        if (parser.size() == 0) {
            std::cerr << "No database schema loaded!" << std::endl;
            return false;
        }
        
        JSONParser::JSONValue schema = parser.getObject(0);
        
        if (!std::holds_alternative<JSONParser::JSONObject>(schema.value)) {
            std::cerr << "Invalid schema format!" << std::endl;
            return false;
        }
        
        auto& dbObj = std::get<JSONParser::JSONObject>(schema.value);
        dbObj["database"] = JSONParser::JSONValue(newName);
        
        // Update the parser with modified schema
        parser.clear();
        parser.appendValue(schema);
        
        std::cout << "Changed database name to '" << newName << "'" << std::endl;
        return true;
    }
    
    // Remove a column from a table
    bool removeColumnFromTable(const std::string& tableName, const std::string& columnName) {
        if (parser.size() == 0) {
            std::cerr << "No database schema loaded!" << std::endl;
            return false;
        }
        
        JSONParser::JSONValue schema = parser.getObject(0);
        JSONParser::JSONObject* table = findTable(schema, tableName);
        
        if (!table) {
            std::cerr << "Table '" << tableName << "' not found!" << std::endl;
            return false;
        }
        
        if (table->find("columns") == table->end() || 
            !std::holds_alternative<JSONParser::JSONArray>((*table)["columns"].value)) {
            std::cerr << "Table '" << tableName << "' has no columns array!" << std::endl;
            return false;
        }
        
        auto& columns = std::get<JSONParser::JSONArray>((*table)["columns"].value);
        
        // Find and remove the column
        for (auto it = columns.begin(); it != columns.end(); ++it) {
            if (std::holds_alternative<JSONParser::JSONObject>(it->value)) {
                auto& columnObj = std::get<JSONParser::JSONObject>(it->value);
                
                if (columnObj.find("name") != columnObj.end() &&
                    std::holds_alternative<std::string>(columnObj["name"].value) &&
                    std::get<std::string>(columnObj["name"].value) == columnName) {
                    
                    columns.erase(it);
                    
                    // Update the parser with modified schema
                    parser.clear();
                    parser.appendValue(schema);
                    
                    std::cout << "Removed column '" << columnName << "' from table '" << tableName << "'" << std::endl;
                    return true;
                }
            }
        }
        
        std::cerr << "Column '" << columnName << "' not found in table '" << tableName << "'" << std::endl;
        return false;
    }
};

// // Example usage
// int main() {
//     // Create a sample database file first
//     std::ofstream file("sample_db.json");
//     file << R"([{
//     "database": "mydb",
//     "tables": [
//         {
//             "name": "users",
//             "columns": [
//                 {
//                     "name": "id",
//                     "type": "int",
//                     "constraints": ["primary_key", "auto_increment"]
//                 },
//                 {
//                     "name": "name",
//                     "type": "varchar",
//                     "length": 100,
//                     "constraints": ["not_null"]
//                 },
//                 {
//                     "name": "email",
//                     "type": "varchar",
//                     "length": 255,
//                     "constraints": ["unique"]
//                 }
//             ]
//         }
//     ]
// }])";
//     file.close();
    
//     // Use the modifier
//     DatabaseModifier modifier("sample_db.json");
    
//     // Load the database
//     if (!modifier.loadDatabase()) {
//         std::cerr << "Failed to load database!" << std::endl;
//         return 1;
//     }
    
//     std::cout << "=== Original Database ===" << std::endl;
//     modifier.printDatabase();
    
//     // Add a new column
//     modifier.addColumnToTable("users", "created_at", "timestamp", {"default_current_timestamp"});
    
//     // Add a new table
//     std::vector<std::map<std::string, std::variant<std::string, int, std::vector<std::string>>>> productColumns = {
//         {
//             {"name", std::string("id")},
//             {"type", std::string("int")},
//             {"constraints", std::vector<std::string>{"primary_key", "auto_increment"}}
//         },
//         {
//             {"name", std::string("product_name")},
//             {"type", std::string("varchar")},
//             {"length", 200},
//             {"constraints", std::vector<std::string>{"not_null"}}
//         },
//         {
//             {"name", std::string("price")},
//             {"type", std::string("decimal")},
//             {"constraints", std::vector<std::string>{"not_null"}}
//         }
//     };
    
//     modifier.addTable("products", productColumns);
    
//     // Change database name
//     modifier.changeDatabaseName("updated_mydb");
    
//     std::cout << "\n=== Modified Database ===" << std::endl;
//     modifier.printDatabase();
    
//     // Save the changes
//     if (modifier.saveDatabase()) {
//         std::cout << "\nDatabase saved successfully!" << std::endl;
//     }
    
//     return 0;
// }


#endif