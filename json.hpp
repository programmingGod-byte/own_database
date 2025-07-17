#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <type_traits>

class JSONParser {
public:
    // Forward declaration and type definitions for JSON values
    struct JSONValue;
    using JSONObject = std::map<std::string, JSONValue>;
    using JSONArray = std::vector<JSONValue>;
    
    // JSONValue definition using variant
    struct JSONValue {
        std::variant<std::nullptr_t, bool, int, double, std::string, JSONArray, JSONObject> value;
        
        // Constructors
        JSONValue() : value(nullptr) {}
        JSONValue(std::nullptr_t) : value(nullptr) {}
        JSONValue(bool v) : value(v) {}
        JSONValue(int v) : value(v) {}
        JSONValue(double v) : value(v) {}
        JSONValue(const std::string& v) : value(v) {}
        JSONValue(const char* v) : value(std::string(v)) {}
        JSONValue(const JSONArray& v) : value(v) {}
        JSONValue(const JSONObject& v) : value(v) {}
        
        // Copy constructor
        JSONValue(const JSONValue& other) : value(other.value) {}
        
        // Move constructor
        JSONValue(JSONValue&& other) noexcept : value(std::move(other.value)) {}
        
        // Copy assignment operator
        JSONValue& operator=(const JSONValue& other) {
            if (this != &other) {
                value = other.value;
            }
            return *this;
        }
        
        // Move assignment operator
        JSONValue& operator=(JSONValue&& other) noexcept {
            if (this != &other) {
                value = std::move(other.value);
            }
            return *this;
        }
        
        // Template assignment operator for non-JSONValue types
        template<typename T>
        typename std::enable_if_t<!std::is_same_v<std::decay_t<T>, JSONValue>, JSONValue&>
        operator=(T&& v) {
            value = std::forward<T>(v);
            return *this;
        }
    };

private:
    std::vector<JSONValue> data;
    std::string filePath;

    // Helper function to skip whitespace
    void skipWhitespace(const std::string& str, size_t& pos) {
        while (pos < str.length() && std::isspace(str[pos])) {
            pos++;
        }
    }

    // Parse string value
    std::string parseString(const std::string& jsonStr, size_t& pos) {
        if (jsonStr[pos] != '"') {
            throw std::runtime_error("Expected '\"' at start of string");
        }
        pos++; // Skip opening quote
        
        std::string result;
        while (pos < jsonStr.length() && jsonStr[pos] != '"') {
            if (jsonStr[pos] == '\\') {
                pos++;
                if (pos >= jsonStr.length()) {
                    throw std::runtime_error("Unexpected end of string");
                }
                
                char escaped = jsonStr[pos];
                switch (escaped) {
                    case '"': result += '"'; break;
                    case '\\': result += '\\'; break;
                    case '/': result += '/'; break;
                    case 'b': result += '\b'; break;
                    case 'f': result += '\f'; break;
                    case 'n': result += '\n'; break;
                    case 'r': result += '\r'; break;
                    case 't': result += '\t'; break;
                    default: result += escaped; break;
                }
            } else {
                result += jsonStr[pos];
            }
            pos++;
        }
        
        if (pos >= jsonStr.length()) {
            throw std::runtime_error("Unterminated string");
        }
        
        pos++; // Skip closing quote
        return result;
    }

    // Parse object value
    JSONObject parseObject(const std::string& jsonStr, size_t& pos) {
        JSONObject obj;
        pos++; // Skip '{'
        
        skipWhitespace(jsonStr, pos);
        
        if (pos < jsonStr.length() && jsonStr[pos] == '}') {
            pos++; // Skip '}'
            return obj;
        }
        
        while (pos < jsonStr.length()) {
            skipWhitespace(jsonStr, pos);
            
            // Parse key
            std::string key = parseString(jsonStr, pos);
            
            skipWhitespace(jsonStr, pos);
            
            if (pos >= jsonStr.length() || jsonStr[pos] != ':') {
                throw std::runtime_error("Expected ':' after key");
            }
            pos++; // Skip ':'
            
            // Parse value
            JSONValue value = parseValue(jsonStr, pos);
            obj[key] = std::move(value);
            
            skipWhitespace(jsonStr, pos);
            
            if (pos >= jsonStr.length()) {
                throw std::runtime_error("Unexpected end of object");
            }
            
            if (jsonStr[pos] == '}') {
                pos++; // Skip '}'
                break;
            } else if (jsonStr[pos] == ',') {
                pos++; // Skip ','
            } else {
                throw std::runtime_error("Expected ',' or '}' in object");
            }
        }
        
        return obj;
    }

    // Parse array value
    JSONArray parseArray(const std::string& jsonStr, size_t& pos) {
        JSONArray arr;
        pos++; // Skip '['
        
        skipWhitespace(jsonStr, pos);
        
        if (pos < jsonStr.length() && jsonStr[pos] == ']') {
            pos++; // Skip ']'
            return arr;
        }
        
        while (pos < jsonStr.length()) {
            JSONValue value = parseValue(jsonStr, pos);
            arr.push_back(std::move(value));
            
            skipWhitespace(jsonStr, pos);
            
            if (pos >= jsonStr.length()) {
                throw std::runtime_error("Unexpected end of array");
            }
            
            if (jsonStr[pos] == ']') {
                pos++; // Skip ']'
                break;
            } else if (jsonStr[pos] == ',') {
                pos++; // Skip ','
            } else {
                throw std::runtime_error("Expected ',' or ']' in array");
            }
        }
        
        return arr;
    }

    // Parse boolean value
    bool parseBoolean(const std::string& jsonStr, size_t& pos) {
        if (pos + 4 <= jsonStr.length() && jsonStr.substr(pos, 4) == "true") {
            pos += 4;
            return true;
        } else if (pos + 5 <= jsonStr.length() && jsonStr.substr(pos, 5) == "false") {
            pos += 5;
            return false;
        } else {
            throw std::runtime_error("Invalid boolean value");
        }
    }

    // Parse null value
    std::nullptr_t parseNull(const std::string& jsonStr, size_t& pos) {
        if (pos + 4 <= jsonStr.length() && jsonStr.substr(pos, 4) == "null") {
            pos += 4;
            return nullptr;
        } else {
            throw std::runtime_error("Invalid null value");
        }
    }

    // Parse number value
    JSONValue parseNumber(const std::string& jsonStr, size_t& pos) {
        size_t start = pos;
        
        if (jsonStr[pos] == '-') {
            pos++;
        }
        
        if (pos >= jsonStr.length() || !std::isdigit(jsonStr[pos])) {
            throw std::runtime_error("Invalid number");
        }
        
        while (pos < jsonStr.length() && std::isdigit(jsonStr[pos])) {
            pos++;
        }
        
        bool isFloat = false;
        
        if (pos < jsonStr.length() && jsonStr[pos] == '.') {
            isFloat = true;
            pos++;
            
            if (pos >= jsonStr.length() || !std::isdigit(jsonStr[pos])) {
                throw std::runtime_error("Invalid number");
            }
            
            while (pos < jsonStr.length() && std::isdigit(jsonStr[pos])) {
                pos++;
            }
        }
        
        if (pos < jsonStr.length() && (jsonStr[pos] == 'e' || jsonStr[pos] == 'E')) {
            isFloat = true;
            pos++;
            
            if (pos < jsonStr.length() && (jsonStr[pos] == '+' || jsonStr[pos] == '-')) {
                pos++;
            }
            
            if (pos >= jsonStr.length() || !std::isdigit(jsonStr[pos])) {
                throw std::runtime_error("Invalid number");
            }
            
            while (pos < jsonStr.length() && std::isdigit(jsonStr[pos])) {
                pos++;
            }
        }
        
        std::string numStr = jsonStr.substr(start, pos - start);
        
        if (isFloat) {
            return JSONValue(std::stod(numStr));
        } else {
            return JSONValue(std::stoi(numStr));
        }
    }

    // Main parse function
    JSONValue parseValue(const std::string& jsonStr, size_t& pos) {
        skipWhitespace(jsonStr, pos);
        
        if (pos >= jsonStr.length()) {
            throw std::runtime_error("Unexpected end of JSON");
        }
        
        char c = jsonStr[pos];
        
        if (c == '"') {
            return JSONValue(parseString(jsonStr, pos));
        } else if (c == '{') {
            return JSONValue(parseObject(jsonStr, pos));
        } else if (c == '[') {
            return JSONValue(parseArray(jsonStr, pos));
        } else if (c == 't' || c == 'f') {
            return JSONValue(parseBoolean(jsonStr, pos));
        } else if (c == 'n') {
            return JSONValue(parseNull(jsonStr, pos));
        } else if (c == '-' || std::isdigit(c)) {
            return parseNumber(jsonStr, pos);
        } else {
            throw std::runtime_error("Invalid JSON character: " + std::string(1, c));
        }
    }

    // Convert JSONValue to string representation
    std::string valueToString(const JSONValue& val, int indent = 0) const {
        std::string indentStr(indent, ' ');
        std::string nextIndentStr(indent + 2, ' ');
        
        return std::visit([&](const auto& v) -> std::string {
            using T = std::decay_t<decltype(v)>;
            
            if constexpr (std::is_same_v<T, std::nullptr_t>) {
                return "null";
            } else if constexpr (std::is_same_v<T, bool>) {
                return v ? "true" : "false";
            } else if constexpr (std::is_same_v<T, int>) {
                return std::to_string(v);
            } else if constexpr (std::is_same_v<T, double>) {
                return std::to_string(v);
            } else if constexpr (std::is_same_v<T, std::string>) {
                return "\"" + v + "\"";
            } else if constexpr (std::is_same_v<T, JSONArray>) {
                if (v.empty()) return "[]";
                
                std::string result = "[\n";
                for (size_t i = 0; i < v.size(); ++i) {
                    result += nextIndentStr + valueToString(v[i], indent + 2);
                    if (i < v.size() - 1) result += ",";
                    result += "\n";
                }
                result += indentStr + "]";
                return result;
            } else if constexpr (std::is_same_v<T, JSONObject>) {
                if (v.empty()) return "{}";
                
                std::string result = "{\n";
                auto it = v.begin();
                for (size_t i = 0; i < v.size(); ++i, ++it) {
                    result += nextIndentStr + "\"" + it->first + "\": " + 
                             valueToString(it->second, indent + 2);
                    if (i < v.size() - 1) result += ",";
                    result += "\n";
                }
                result += indentStr + "}";
                return result;
            }
            return "";
        }, val.value);
    }

public:
    // Constructor
    JSONParser(const std::string& filePath = "") : filePath(filePath) {}

    // Append a JSON object from string
    bool appendFromString(const std::string& jsonStr) {
        try {
            size_t pos = 0;
            JSONValue value = parseValue(jsonStr, pos);
            data.push_back(std::move(value));
            
            std::cout << "Object appended successfully. Total objects: " << data.size() << std::endl;
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            return false;
        }
    }

    // Append a simple object (key-value pairs)
    void appendObject(const JSONObject& obj) {
        data.push_back(JSONValue(obj));
        std::cout << "Object appended successfully. Total objects: " << data.size() << std::endl;
    }

    // Append any JSONValue
    void appendValue(const JSONValue& value) {
        data.push_back(value);
        std::cout << "Value appended successfully. Total objects: " << data.size() << std::endl;
    }

    // Get object by index
    JSONValue getObject(size_t index) const {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // Remove object by index
    bool removeObject(size_t index) {
        if (index >= data.size()) {
            std::cerr << "Index out of range" << std::endl;
            return false;
        }
        
        data.erase(data.begin() + index);
        std::cout << "Object at index " << index << " removed successfully." << std::endl;
        return true;
    }

    // Get total number of objects
    size_t size() const {
        return data.size();
    }

    // Clear all data
    void clear() {
        data.clear();
        std::cout << "All data cleared." << std::endl;
    }

    // Save to file
    bool saveToFile(const std::string& customPath = "") {
        std::string targetPath = customPath.empty() ? filePath : customPath;
        
        if (targetPath.empty()) {
            std::cerr << "Error: No file path provided." << std::endl;
            return false;
        }
        
        try {
            // Create directory if it doesn't exist
            std::filesystem::path path(targetPath);
            if (path.has_parent_path()) {
                std::filesystem::create_directories(path.parent_path());
            }
            
            std::ofstream file(targetPath);
            if (!file.is_open()) {
                std::cerr << "Error: Cannot open file for writing: " << targetPath << std::endl;
                return false;
            }
            
            file << "[\n";
            for (size_t i = 0; i < data.size(); ++i) {
                file << "  " << valueToString(data[i], 2);
                if (i < data.size() - 1) file << ",";
                file << "\n";
            }
            file << "]";
            
            file.close();
            
            std::cout << "Data saved successfully to: " << targetPath << std::endl;
            std::cout << "Saved " << data.size() << " objects" << std::endl;
            return true;
            
        } catch (const std::exception& e) {
            std::cerr << "Error saving to file: " << e.what() << std::endl;
            return false;
        }
    }

    // Load from file
    bool loadFromFile(const std::string& customPath = "", bool appendMode = false) {
        std::string targetPath = customPath.empty() ? filePath : customPath;
        
        if (targetPath.empty()) {
            std::cerr << "Error: No file path provided." << std::endl;
            return false;
        }
        
        if (!std::filesystem::exists(targetPath)) {
            std::cerr << "Error: File does not exist: " << targetPath << std::endl;
            return false;
        }
        
        try {
            std::ifstream file(targetPath);
            if (!file.is_open()) {
                std::cerr << "Error: Cannot open file for reading: " << targetPath << std::endl;
                return false;
            }
            
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string jsonContent = buffer.str();
            file.close();
            
            size_t pos = 0;
            JSONValue loadedValue = parseValue(jsonContent, pos);
            
            if (!appendMode) {
                data.clear();
            }
            
            // If loaded value is an array, add all elements
            if (std::holds_alternative<JSONArray>(loadedValue.value)) {
                JSONArray arr = std::get<JSONArray>(loadedValue.value);
                for (auto& item : arr) {
                    data.push_back(std::move(item));
                }
                std::cout << "Loaded " << arr.size() << " objects from: " << targetPath << std::endl;
            } else {
                // If it's a single object, add it
                data.push_back(std::move(loadedValue));
                std::cout << "Loaded 1 object from: " << targetPath << std::endl;
            }
            
            std::cout << "Total objects in parser: " << data.size() << std::endl;
            return true;
            
        } catch (const std::exception& e) {
            std::cerr << "Error loading from file: " << e.what() << std::endl;
            return false;
        }
    }

    // Print all objects
    void printAll() const {
        std::cout << "\n=== JSON Parser Contents ===" << std::endl;
        std::cout << "Total objects: " << data.size() << std::endl;
        
        for (size_t i = 0; i < data.size(); ++i) {
            std::cout << "\nObject " << i << ":" << std::endl;
            std::cout << valueToString(data[i]) << std::endl;
        }
    }

    // Print specific object
    void printObject(size_t index) const {
        if (index >= data.size()) {
            std::cerr << "Index out of range" << std::endl;
            return;
        }
        
        std::cout << "Object " << index << ":" << std::endl;
        std::cout << valueToString(data[index]) << std::endl;
    }

    // Set file path
    void setFilePath(const std::string& path) {
        filePath = path;
    }

    // Get file path
    std::string getFilePath() const {
        return filePath;
    }

    // Get all data as vector
    const std::vector<JSONValue>& getData() const {
        return data;
    }
};

#endif // JSON_PARSER_HPP