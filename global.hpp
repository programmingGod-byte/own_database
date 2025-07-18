#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <string>
#include <unordered_map>
#include <memory>
#include <variant>
#include <vector>
#include <climits>

#include "databaseSchemaReader.hpp"
#include "storageTree.hpp"

// --- File Paths ---
inline std::string currentDbPath = "db/current_db.meta";
inline std::string dbDirectoryPath = "./db";
inline std::string allTableDataDirectory = "./db/data";
inline std::string currentDatabase = "";
inline std::string tableDirectory = "./db/tables";
// --- Schema Node Structure ---
struct TableGlobalColumnNode {
    std::string type;
    std::string name;
    std::vector<std::string> constraint;
    bool autoIncrement = false;
    bool isUnique = false;
    bool isPrimary = false;
    bool createIndex = false;
    int length = INT_MAX;
};

// --- JSON Parser Cache ---
// db_name -> JSON parser
 std::unordered_map<std::string, std::shared_ptr<PythonLikeJSONParser>> globalJsonCache;

// --- Table Metadata Cache ---
// db_name -> table_name -> vector of column definitions
 std::unordered_map<
    std::string,
    std::unordered_map<
        std::string,
        std::vector<std::shared_ptr<TableGlobalColumnNode>>>>
    globalTableCache;

// --- Index Node Representation ---
struct IndexNode {
    int64_t start;
    int16_t end;
};

// --- B+ Tree Variant for different key types ---
using TreeVariant = std::variant<
    std::shared_ptr<BPlusTree<int, IndexNode>>,
    std::shared_ptr<BPlusTree<std::string, IndexNode>>>;

// --- B+ Tree Cache ---
// db_name -> table_name -> column_name -> B+ Tree
 std::unordered_map<
    std::string,
    std::unordered_map<
        std::string,
        std::unordered_map<
            std::string,
            TreeVariant>>>
    dbBtrees;

#endif // GLOBALS_HPP
