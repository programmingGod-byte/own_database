#ifndef __DATABASE_SCHEMA_READER
#define __DATABASE_SCHEMA_READER
#include "json.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

// Forward declaration
class JSONWrapper;

// Helper class for array-like access
class JSONArrayWrapper {
private:
    JSONParser::JSONArray arr;
    
public:
    JSONArrayWrapper(const JSONParser::JSONArray& array) : arr(array) {}
    
    // Array access operator
    JSONWrapper operator[](size_t index) const;
    
    // Get size
    size_t size() const { return arr.size(); }
    
    // Iterator support
    auto begin() const { return arr.begin(); }
    auto end() const { return arr.end(); }
    
    // Convert to vector of strings (useful for constraints, etc.)
    std::vector<std::string> toStringVector() const {
        std::vector<std::string> result;
        for (const auto& item : arr) {
            if (std::holds_alternative<std::string>(item.value)) {
                result.push_back(std::get<std::string>(item.value));
            }
        }
        return result;
    }
    
    // Print all elements
    void print() const {
        std::cout << "[";
        for (size_t i = 0; i < arr.size(); ++i) {
            if (std::holds_alternative<std::string>(arr[i].value)) {
                std::cout << "\"" << std::get<std::string>(arr[i].value) << "\"";
            } else if (std::holds_alternative<int>(arr[i].value)) {
                std::cout << std::get<int>(arr[i].value);
            } else if (std::holds_alternative<double>(arr[i].value)) {
                std::cout << std::get<double>(arr[i].value);
            } else if (std::holds_alternative<bool>(arr[i].value)) {
                std::cout << (std::get<bool>(arr[i].value) ? "true" : "false");
            }
            if (i < arr.size() - 1) std::cout << ", ";
        }
        std::cout << "]";
    }
};

// Main wrapper class
class JSONWrapper {
private:
    JSONParser::JSONValue value;
    
public:
    // Constructors
    JSONWrapper(const JSONParser::JSONValue& val) : value(val) {}
    JSONWrapper(const JSONParser::JSONObject& obj) : value(obj) {}
    JSONWrapper(const JSONParser::JSONArray& arr) : value(arr) {}
    
    // Object access operator - returns another JSONWrapper
    JSONWrapper operator[](const std::string& key) const {
        if (std::holds_alternative<JSONParser::JSONObject>(value.value)) {
            const auto& obj = std::get<JSONParser::JSONObject>(value.value);
            auto it = obj.find(key);
            if (it != obj.end()) {
                return JSONWrapper(it->second);
            }
            throw std::runtime_error("Key '" + key + "' not found in object");
        }
        throw std::runtime_error("Cannot use string key on non-object type");
    }
    
    // Array access operator - returns another JSONWrapper
    JSONWrapper operator[](size_t index) const {
        if (std::holds_alternative<JSONParser::JSONArray>(value.value)) {
            const auto& arr = std::get<JSONParser::JSONArray>(value.value);
            if (index >= arr.size()) {
                throw std::runtime_error("Array index out of bounds");
            }
            return JSONWrapper(arr[index]);
        }
        throw std::runtime_error("Cannot use numeric index on non-array type");
    }
    
    // Conversion operators
    operator std::string() const {
        if (std::holds_alternative<std::string>(value.value)) {
            return std::get<std::string>(value.value);
        }
        throw std::runtime_error("Cannot convert to string");
    }
    
    operator int() const {
        if (std::holds_alternative<int>(value.value)) {
            return std::get<int>(value.value);
        }
        throw std::runtime_error("Cannot convert to int");
    }
    
    operator double() const {
        if (std::holds_alternative<double>(value.value)) {
            return std::get<double>(value.value);
        }
        throw std::runtime_error("Cannot convert to double");
    }
    
    operator bool() const {
        if (std::holds_alternative<bool>(value.value)) {
            return std::get<bool>(value.value);
        }
        throw std::runtime_error("Cannot convert to bool");
    }
    
    // Get as array wrapper
    JSONArrayWrapper asArray() const {
        if (std::holds_alternative<JSONParser::JSONArray>(value.value)) {
            return JSONArrayWrapper(std::get<JSONParser::JSONArray>(value.value));
        }
        throw std::runtime_error("Value is not an array");
    }
    
    // Helper methods
    bool isString() const { return std::holds_alternative<std::string>(value.value); }
    bool isInt() const { return std::holds_alternative<int>(value.value); }
    bool isDouble() const { return std::holds_alternative<double>(value.value); }
    bool isBool() const { return std::holds_alternative<bool>(value.value); }
    bool isArray() const { return std::holds_alternative<JSONParser::JSONArray>(value.value); }
    bool isObject() const { return std::holds_alternative<JSONParser::JSONObject>(value.value); }
    bool isNull() const { return std::holds_alternative<std::nullptr_t>(value.value); }
    
    // Get string value (safe)
    std::string getString() const {
        if (isString()) return std::get<std::string>(value.value);
        return "";
    }
    
    // Get int value (safe)
    int getInt() const {
        if (isInt()) return std::get<int>(value.value);
        return 0;
    }
    
    // Get double value (safe)
    double getDouble() const {
        if (isDouble()) return std::get<double>(value.value);
        return 0.0;
    }
    
    // Get bool value (safe)
    bool getBool() const {
        if (isBool()) return std::get<bool>(value.value);
        return false;
    }
    
    // Get array size
    size_t size() const {
        if (isArray()) {
            return std::get<JSONParser::JSONArray>(value.value).size();
        }
        return 0;
    }
    
    // Check if key exists (for objects)
    bool hasKey(const std::string& key) const {
        if (isObject()) {
            const auto& obj = std::get<JSONParser::JSONObject>(value.value);
            return obj.find(key) != obj.end();
        }
        return false;
    }
    
    // Get all keys (for objects)
    std::vector<std::string> getKeys() const {
        std::vector<std::string> keys;
        if (isObject()) {
            const auto& obj = std::get<JSONParser::JSONObject>(value.value);
            for (const auto& pair : obj) {
                keys.push_back(pair.first);
            }
        }
        return keys;
    }
    
    // Print value
    void print() const {
        if (isString()) {
            std::cout << "\"" << getString() << "\"";
        } else if (isInt()) {
            std::cout << getInt();
        } else if (isDouble()) {
            std::cout << getDouble();
        } else if (isBool()) {
            std::cout << (getBool() ? "true" : "false");
        } else if (isArray()) {
            asArray().print();
        } else if (isNull()) {
            std::cout << "null";
        } else {
            std::cout << "{object}";
        }
    }
};

// Implementation of JSONArrayWrapper::operator[]
JSONWrapper JSONArrayWrapper::operator[](size_t index) const {
    if (index >= arr.size()) {
        throw std::runtime_error("Array index out of bounds");
    }
    return JSONWrapper(arr[index]);
}

// Main wrapper class for the parser
class PythonLikeJSONParser {
private:
    JSONParser parser;
    
public:
    PythonLikeJSONParser() {}
    
    // Load from string
    bool loadFromString(const std::string& jsonStr) {
        return parser.appendFromString(jsonStr);
    }
    
    // Load from file
    bool loadFromFile(const std::string& filePath) {
        return parser.loadFromFile(filePath);
    }
    
    // Array-like access to root elements
    JSONWrapper operator[](size_t index) const {
        if (index >= parser.size()) {
            throw std::runtime_error("Index out of bounds");
        }
        return JSONWrapper(parser.getObject(index));
    }
    
    // Get size
    size_t size() const {
        return parser.size();
    }
    
    // Get underlying parser
    const JSONParser& getParser() const {
        return parser;
    }
};

// // Example usage
// int main() {
//     PythonLikeJSONParser parser;
    
//     // Your JSON data
//     std::string jsonData = R"([
//  {
// "name": "testing",
// "tables": [
//  {
// "columns": [
//  {
// "constraints": [
// "primary_key",
// "auto_increment"
//  ],
// "name": "id",
// "type": "int"
//  },
//  {
// "constraints": [],
// "length": 100,
// "name": "name",
// "type": "varchar"
//  },
//  {
// "constraints": [
// "unique"
//  ],
// "length": 100,
// "name": "email",
// "type": "varchar"
//  },
//  {
// "constraints": [],
// "name": "age",
// "type": "int"
//  },
//  {
// "constraints": [],
// "length": 100,
// "name": "department",
// "type": "varchar"
//  },
//  {
// "constraints": [],
// "name": "year",
// "type": "int"
//  }
//  ],
// "name": "students"
//  }
//  ]
//  }
// ])";
    
//     if (parser.loadFromString(jsonData)) {
//         std::cout << "=== Python-like JSON Access Demo ===" << std::endl;
        
//         // Access database name - parser[0]["name"]
//         std::cout << "\nDatabase name: ";
//         parser[0]["name"].print();
//         std::cout << std::endl;
        
//         // Or get as string
//         std::string dbName = parser[0]["name"];
//         std::cout << "Database name (as string): " << dbName << std::endl;
        
//         // Access table name - parser[0]["tables"][0]["name"]
//         std::cout << "\nTable name: ";
//         parser[0]["tables"][0]["name"].print();
//         std::cout << std::endl;
        
//         // Get all columns - parser[0]["tables"][0]["columns"]
//         std::cout << "\nAll columns:" << std::endl;
//         JSONArrayWrapper columns = parser[0]["tables"][0]["columns"].asArray();
        
//         for (size_t i = 0; i < columns.size(); ++i) {
//             JSONWrapper column = columns[i];
//             std::cout << "Column " << i << ": ";
//             std::cout << "name=" << column["name"].getString();
//             std::cout << ", type=" << column["type"].getString();
            
//             // Check if length exists
//             if (column.hasKey("length")) {
//                 std::cout << ", length=" << column["length"].getInt();
//             }
            
//             // Get constraints
//             if (column.hasKey("constraints")) {
//                 JSONArrayWrapper constraints = column["constraints"].asArray();
//                 if (constraints.size() > 0) {
//                     std::cout << ", constraints=";
//                     constraints.print();
//                 }
//             }
//             std::cout << std::endl;
//         }
        
//         // Access specific column by index - parser[0]["tables"][0]["columns"][2]
//         std::cout << "\nEmail column (index 2):" << std::endl;
//         JSONWrapper emailColumn = parser[0]["tables"][0]["columns"][2];
//         std::cout << "Name: " << emailColumn["name"].getString() << std::endl;
//         std::cout << "Type: " << emailColumn["type"].getString() << std::endl;
//         std::cout << "Length: " << emailColumn["length"].getInt() << std::endl;
//         std::cout << "Constraints: ";
//         emailColumn["constraints"].asArray().print();
//         std::cout << std::endl;
        
//         // More examples
//         std::cout << "\n=== More Examples ===" << std::endl;
        
//         // Get all column names
//         std::cout << "\nAll column names:" << std::endl;
//         for (size_t i = 0; i < columns.size(); ++i) {
//             std::cout << "- " << columns[i]["name"].getString() << std::endl;
//         }
        
//         // Get constraints for specific column
//         std::cout << "\nConstraints for 'id' column:" << std::endl;
//         JSONArrayWrapper idConstraints = parser[0]["tables"][0]["columns"][0]["constraints"].asArray();
//         std::vector<std::string> constraintsList = idConstraints.toStringVector();
//         for (const auto& constraint : constraintsList) {
//             std::cout << "- " << constraint << std::endl;
//         }
        
//     } else {
//         std::cout << "Failed to load JSON data!" << std::endl;
//     }
    
//     return 0;
// }
#endif