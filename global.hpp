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



enum class ASTNodeType
{
    STATEMENT,
    SELECT_STATEMENT,
    INSERT_STATEMENT,
    UPDATE_STATEMENT,
    DELETE_STATEMENT,
    EXPRESSION,
    IDENTIFIER,
    INT_LITERAL,
    STRING_LITERAL,
    BOOLEAN_LITERAL,
    COMPARISON_EXPRESSION,
    LOGICAL_EXPRESSION,
    PARENTHESIZED_EXPRESSION,
    LIMIT_CLAUSE,
    WHERE_CLAUSE,
    DROP_STATEMENT,
    CREATE_STATEMENT
};

enum class LogicalOperator
{
    AND,
    OR
};

enum class ComparisonOperator
{
    EQUAL,
    NOT_EQUAL,
    GREATER,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL
};
enum class ColumnConstraint
{
    NONE,
    NOT_NULL,
    PRIMARY_KEY,
    UNIQUE,
    AUTO_INCREMENT
};

// ==== AST Nodes ====

struct ASTNode
{
    virtual ~ASTNode() = default;
    virtual ASTNodeType getType() const = 0;
};

struct Expression : public ASTNode
{
    virtual ~Expression() = default;
};

struct Identifier : public Expression
{
    std::string name;
    Identifier(const std::string &name) : name(name) {}
    ASTNodeType getType() const override { return ASTNodeType::IDENTIFIER; }
};

struct IntLiteral : public Expression
{
    int value;
    IntLiteral(int value) : value(value) {}
    ASTNodeType getType() const override { return ASTNodeType::INT_LITERAL; }
};

struct StringLiteral : public Expression
{
    std::string value;
    StringLiteral(const std::string &value) : value(value) {}
    ASTNodeType getType() const override { return ASTNodeType::STRING_LITERAL; }
};

struct BoolLiteral : public Expression
{
    bool value;
    BoolLiteral(bool value) : value(value) {}
    ASTNodeType getType() const override { return ASTNodeType::BOOLEAN_LITERAL; }
};

struct ComparisonExpression : public Expression
{
    std::unique_ptr<Expression> left;
    ComparisonOperator op;
    std::unique_ptr<Expression> right;
    ComparisonExpression(std::unique_ptr<Expression> left, ComparisonOperator op, std::unique_ptr<Expression> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}
    ASTNodeType getType() const override { return ASTNodeType::COMPARISON_EXPRESSION; }
};

struct LogicalExpression : public Expression
{
    std::unique_ptr<Expression> left;
    LogicalOperator op;
    std::unique_ptr<Expression> right;
    LogicalExpression(std::unique_ptr<Expression> left, LogicalOperator op, std::unique_ptr<Expression> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}
    ASTNodeType getType() const override { return ASTNodeType::LOGICAL_EXPRESSION; }
};

struct ParenthesizedExpression : public Expression
{
    std::unique_ptr<Expression> expression;
    ParenthesizedExpression(std::unique_ptr<Expression> expr)
        : expression(std::move(expr)) {}
    ASTNodeType getType() const override { return ASTNodeType::PARENTHESIZED_EXPRESSION; }
};

struct WhereClause : public ASTNode
{
    std::unique_ptr<Expression> condition;
    WhereClause(std::unique_ptr<Expression> condition) : condition(std::move(condition)) {}
    ASTNodeType getType() const override { return ASTNodeType::WHERE_CLAUSE; }
};

struct LimitClause : public ASTNode
{
    size_t limit;
    LimitClause(size_t limit) : limit(limit) {}
    ASTNodeType getType() const override { return ASTNodeType::LIMIT_CLAUSE; }
};

struct SelectStatement : public ASTNode
{
    std::vector<std::string> columns;
    std::string table;
    std::unique_ptr<WhereClause> whereClause = nullptr;
    std::unique_ptr<LimitClause> limitClause = nullptr;

    ASTNodeType getType() const override { return ASTNodeType::SELECT_STATEMENT; }
};

struct DropStatement : public ASTNode
{
    bool istable;
    std::string name;

    ASTNodeType getType() const override { return ASTNodeType::DROP_STATEMENT; }
};

struct ColumnDefinition
{
    std::string name;
    std::string type;
    std::vector<ColumnConstraint> constraints;

    ColumnDefinition(const std::string &name, const std::string &type)
        : name(name), type(type) {}
};

struct CreateStatement : public ASTNode
{
    bool isDatabase = false;
    std::string name;
    std::vector<ColumnDefinition> columns;

    ASTNodeType getType() const override { return ASTNodeType::CREATE_STATEMENT; }
};

struct InsertStatement
{
    std::string tableName;
    std::vector<std::string> columns;
    std::vector<std::string> values;
};

#endif // GLOBALS_HPP
