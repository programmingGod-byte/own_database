#ifndef __PARSER_AST_HPP
#define __PARSER_AST_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "SQL_LEXER.hpp"
#include <filesystem> // Include for std::filesystem
#include <fstream>
#include "json.hpp"
#include "global.hpp"
#include "utility.hpp"
namespace fs = std::filesystem; // Shorthand for std::filesystem

bool fileExists(const std::string &filename)
{
    return fs::exists(filename);
}

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

// ==== Parser ====

class Parser
{
private:
    std::vector<Token *> tokens;
    size_t position = 0;

    Token *peek(int offset = 0)
    {
        if (position + offset >= tokens.size())
            return nullptr;
        return tokens[position + offset];
    }

    Token *current() { return peek(0); }

    Token *advance()
    {
        if (position < tokens.size())
            position++;
        return previous();
    }

    Token *previous()
    {
        if (position == 0)
            return nullptr;
        return tokens[position - 1];
    }

    void rewind()
    {
        if (position > 0)
            position--;
    }

    bool match(TokenType expected)
    {
        if (current() && current()->TYPE == expected)
        {
            advance();
            return true;
        }
        return false;
    }

    Token *expect(TokenType expected, const std::string &message)
    {
        if (match(expected))
            return previous();
        throw std::runtime_error("Parse error: " + message);
    }

public:
    std::string currentDb = "";
    Parser(const std::vector<Token *> &tokens) : tokens(tokens)
    {
        ensureCurrentDbFile("db/current_db.meta");
    }

    void ensureCurrentDbFile(const std::string &filePath)
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

        if (!fs::exists(filePath))
        {
            std::ofstream outfile(filePath);
            if (outfile.is_open())
            {
                outfile << "{\"current_db\":\"test\"}" << std::endl; // JSON-style default
                std::cout << "File '" << filePath << "' created and initialized." << std::endl;
                outfile.close();
                currentDatabase = "test";
            }
            else
            {
                throw std::runtime_error("Error: Could not create file '" + filePath + "'");
            }
        }
        else
        {
            std::cout << "hfffffffffff\n";
            std::ifstream in(filePath);
            std::stringstream buffer;
            buffer << in.rdbuf();
            in.close();
            std::string jsonContent = buffer.str();

            JSONParser jsonParser;
            if (!jsonParser.appendFromString(jsonContent))
            {
                throw std::runtime_error("Failed to parse current_db.meta JSON.");
            }
            JSONParser::JSONValue obj = jsonParser.getObject(0);

            if (std::holds_alternative<JSONParser::JSONObject>(obj.value))
            {
                auto jsonObj = std::get<JSONParser::JSONObject>(obj.value);
                if (jsonObj.find("current_db") != jsonObj.end())
                {
                    const auto &val = jsonObj["current_db"];
                    if (std::holds_alternative<std::string>(val.value))
                    {
                        this->currentDb = std::get<std::string>(val.value); // store it inside parser
                        currentDatabase = this->currentDb;
                    }
                }
            }
        }
    }

    std::unique_ptr<InsertStatement> parseInsertStatement()
    {
        expect(TokenType::INSERT, "Expected 'INSERT'");
        expect(TokenType::INTO, "Expected 'INTO'");

        Token *tableToken = expect(TokenType::IDENTIFIER, "Expected table name");
        std::unique_ptr<InsertStatement> stmt = std::make_unique<InsertStatement>();
        stmt->tableName = tableToken->VALUE;

        expect(TokenType::OPEN_PAREN, "Expected '(' before column list");

        // Parse columns
        do
        {
            Token *col = expect(TokenType::IDENTIFIER, "Expected column name");
            stmt->columns.push_back(col->VALUE);
        } while (match(TokenType::COMMA));

        expect(TokenType::CLOSE_PAREN, "Expected ')' after column list");

        expect(TokenType::VALUES, "Expected 'VALUES'");
        expect(TokenType::OPEN_PAREN, "Expected '(' before values");

        // Parse values
        do
        {
            if (match(TokenType::STRING) || match(TokenType::NUMBER))
            {
                stmt->values.push_back(previous()->VALUE);
            }
            else
            {
                throw std::runtime_error("Expected a STRING in quotes or a NUMBER");
            }
        } while (match(TokenType::COMMA));

        expect(TokenType::CLOSE_PAREN, "Expected ')' after values");
        expect(TokenType::SEMICOLON, "Expected ';' at end");

        return stmt;
    }

    std::unique_ptr<CreateStatement> parseCreateStatement()
    {
        expect(TokenType::CREATE, "Expected CREATE keyword");
        std::unique_ptr<CreateStatement> stmt = std::make_unique<CreateStatement>();

        if (match(TokenType::TABLE))
        {
            Token *tableName = expect(TokenType::IDENTIFIER, "Expected table name");
            stmt->name = tableName->VALUE;

            expect(TokenType::OPEN_PAREN, "Expected '(' after table name");

            while (!match(TokenType::CLOSE_PAREN))
            {
                Token *colName = expect(TokenType::IDENTIFIER, "Expected column name");

                Token *typeToken = current();
                if (match(TokenType::INT) || match(TokenType::VARCHAR))
                {
                    typeToken = previous();
                }
                else
                {
                    throw std::runtime_error("Parse error: Expected column type (int or varchar)");
                }

                ColumnDefinition column(colName->VALUE, typeToken->VALUE);

                // Handle VARCHAR(255) size syntax
                if (typeToken->TYPE == TokenType::VARCHAR && match(TokenType::OPEN_PAREN))
                {
                    Token *size = expect(TokenType::NUMBER, "Expected size in VARCHAR()");
                    expect(TokenType::CLOSE_PAREN, "Expected ')' after VARCHAR size");
                    column.type += "(" + size->VALUE + ")";
                }

                // Parse optional constraints
                while (true)
                {
                    if (match(TokenType::NOT))
                    {
                        expect(TokenType::NULL_T, "Expected NULL after NOT");
                        column.constraints.push_back(ColumnConstraint::NOT_NULL);
                    }
                    else if (match(TokenType::PRIMARY))
                    {
                        expect(TokenType::KEY, "Expected KEY after PRIMARY");
                        column.constraints.push_back(ColumnConstraint::PRIMARY_KEY);
                    }
                    else if (match(TokenType::AUTO_INCREMENT))
                    {
                        column.constraints.push_back(ColumnConstraint::AUTO_INCREMENT);
                    }
                    else if (match(TokenType::UNIQUE))
                    {
                        column.constraints.push_back(ColumnConstraint::UNIQUE);
                    }
                    else
                    {
                        break;
                    }
                }

                stmt->columns.push_back(column);

                if (match(TokenType::COMMA))
                {
                    continue;
                }
                else if (peek()->TYPE == TokenType::CLOSE_PAREN)
                {
                    continue;
                }
                else
                {
                    throw std::runtime_error("Expected ',' or ')' in column list");
                }
            }
        }
        else if (match(TokenType::DATABASE))
        {
            stmt->isDatabase = true;
            stmt->name = expect(TokenType::IDENTIFIER, "Expected database name")->VALUE;
            std::stringstream filename;
            filename << "./db/";
            filename << stmt->name;
            filename << ".shivam.db";

            if (MyUtility::checkIfFileExist(filename.str()))
            {
                throw std::runtime_error("Database already exists");
            }
            else
            {
                std::stringstream s;
                s << R"(
{
  "name": ")" << stmt->name
                  << R"(",
  "tables": []
}
)";
                MyUtility::createFile(filename.str(), s.str());
                currentDatabase = stmt->name;
                MyUtility::changeCurrentDb(currentDatabase);
            }
        }
        else
        {
            throw std::runtime_error("Expected TABLE or DATABASE keyword");
        }

        return stmt;
    }

    std::unique_ptr<DropStatement> parseDropStatement()
    {
        expect(TokenType::DROP, "Expected drop keyword");
        auto stmt = std::make_unique<DropStatement>();
        Token *token = advance();
        switch (token->TYPE)
        {
        case TokenType::TABLE:
        {
            Token *identifier = expect(TokenType::IDENTIFIER, "not a identifier\n");
            stmt->name = identifier->VALUE;
            stmt->istable = true;
        }

        break;
        case TokenType::DATABASE:
        {
            Token *identifier = expect(TokenType::IDENTIFIER, "not a identifier\n");
            stmt->name = identifier->VALUE;
            stmt->istable = false;
        }

        default:
            std::runtime_error("error in drop ");
        }
        return stmt;
    }

    std::unique_ptr<SelectStatement> parseSelectStatement()
    {
        expect(TokenType::SELECT, "Expected SELECT keyword");

        auto stmt = std::make_unique<SelectStatement>();

        while (true)
        {
            Token *column = expect(TokenType::IDENTIFIER, "Expected column name");
            stmt->columns.push_back(column->VALUE);
            if (!match(TokenType::COMMA))
                break;
        }

        expect(TokenType::FROM, "Expected FROM keyword");
        Token *table = expect(TokenType::IDENTIFIER, "Expected table name");
        stmt->table = table->VALUE;

        if (match(TokenType::WHERE))
        {
            auto condition = parseExpression();
            stmt->whereClause = std::make_unique<WhereClause>(std::move(condition));
        }

        // Optional: limit
        if (match(TokenType::IDENTIFIER) && previous()->VALUE == "limit")
        {
            Token *limitValue = expect(TokenType::NUMBER, "Expected number after LIMIT");
            stmt->limitClause = std::make_unique<LimitClause>(std::stoi(limitValue->VALUE));
        }

        return stmt;
    }

    std::unique_ptr<Expression> parseExpression()
    {
        return parseLogical();
    }

    std::unique_ptr<Expression> parseLogical()
    {
        auto left = parseComparison();

        while (match(TokenType::AND) || match(TokenType::OR))
        {
            LogicalOperator op = (previous()->TYPE == TokenType::AND) ? LogicalOperator::AND : LogicalOperator::OR;
            auto right = parseComparison();
            left = std::make_unique<LogicalExpression>(std::move(left), op, std::move(right));
        }

        return left;
    }

    std::unique_ptr<Expression> parseComparison()
    {
        auto left = parsePrimary();

        if (match(TokenType::EQUAL) || match(TokenType::NOT_EQUAL) ||
            match(TokenType::GREATER) || match(TokenType::LESS) ||
            match(TokenType::GREATER_EQUAL) || match(TokenType::LESS_EQUAL))
        {

            TokenType opToken = previous()->TYPE;
            ComparisonOperator op;

            switch (opToken)
            {
            case TokenType::EQUAL:
                op = ComparisonOperator::EQUAL;
                break;
            case TokenType::NOT_EQUAL:
                op = ComparisonOperator::NOT_EQUAL;
                break;
            case TokenType::GREATER:
                op = ComparisonOperator::GREATER;
                break;
            case TokenType::LESS:
                op = ComparisonOperator::LESS;
                break;
            case TokenType::GREATER_EQUAL:
                op = ComparisonOperator::GREATER_EQUAL;
                break;
            case TokenType::LESS_EQUAL:
                op = ComparisonOperator::LESS_EQUAL;
                break;
            default:
                throw std::runtime_error("Invalid comparison operator");
            }

            auto right = parsePrimary();
            return std::make_unique<ComparisonExpression>(std::move(left), op, std::move(right));
        }

        return left;
    }

    std::unique_ptr<Expression> parsePrimary()
    {
        if (match(TokenType::OPEN_PAREN))
        {
            auto expr = parseExpression();
            expect(TokenType::CLOSE_PAREN, "Expected ')'");
            return std::make_unique<ParenthesizedExpression>(std::move(expr));
        }

        if (match(TokenType::IDENTIFIER))
        {
            std::string val = previous()->VALUE;
            if (val == "true" || val == "false")
            {
                return std::make_unique<BoolLiteral>(val == "true");
            }
            return std::make_unique<Identifier>(val);
        }

        if (match(TokenType::NUMBER))
        {
            return std::make_unique<IntLiteral>(std::stoi(previous()->VALUE));
        }

        if (match(TokenType::STRING))
        {
            return std::make_unique<StringLiteral>(previous()->VALUE);
        }

        throw std::runtime_error("Unexpected token in expression");
    }

    void printExpression(const Expression *expr, int indent)
    {
        auto pad = [indent]()
        { for (int i = 0; i < indent; ++i) std::cout << "  "; };

        if (!expr)
            return;

        switch (expr->getType())
        {
        case ASTNodeType::IDENTIFIER:
        {
            const auto *id = static_cast<const Identifier *>(expr);
            pad();
            std::cout << "Identifier: " << id->name << "\n";
            break;
        }
        case ASTNodeType::INT_LITERAL:
        {
            const auto *num = static_cast<const IntLiteral *>(expr);
            pad();
            std::cout << "IntLiteral: " << num->value << "\n";
            break;
        }
        case ASTNodeType::STRING_LITERAL:
        {
            const auto *str = static_cast<const StringLiteral *>(expr);
            pad();
            std::cout << "StringLiteral: \"" << str->value << "\"\n";
            break;
        }
        case ASTNodeType::COMPARISON_EXPRESSION:
        {
            const auto *comp = static_cast<const ComparisonExpression *>(expr);
            pad();
            std::cout << "ComparisonExpression: ";

            switch (comp->op)
            {
            case ComparisonOperator::EQUAL:
                std::cout << "==\n";
                break;
            case ComparisonOperator::NOT_EQUAL:
                std::cout << "!=\n";
                break;
            case ComparisonOperator::GREATER:
                std::cout << ">\n";
                break;
            case ComparisonOperator::LESS:
                std::cout << "<\n";
                break;
            case ComparisonOperator::GREATER_EQUAL:
                std::cout << ">=\n";
                break;
            case ComparisonOperator::LESS_EQUAL:
                std::cout << "<=\n";
                break;
            }

            printExpression(comp->left.get(), indent + 1);
            printExpression(comp->right.get(), indent + 1);
            break;
        }
        case ASTNodeType::LOGICAL_EXPRESSION:
        {
            const auto *log = static_cast<const LogicalExpression *>(expr);
            pad();
            std::cout << "LogicalExpression: " << (log->op == LogicalOperator::AND ? "AND" : "OR") << "\n";
            printExpression(log->left.get(), indent + 1);
            printExpression(log->right.get(), indent + 1);
            break;
        }
        case ASTNodeType::PARENTHESIZED_EXPRESSION:
        {
            const auto *paren = static_cast<const ParenthesizedExpression *>(expr);
            pad();
            std::cout << "ParenthesizedExpression:\n";
            printExpression(paren->expression.get(), indent + 1);
            break;
        }
        default:
            pad();
            std::cout << "Unknown Expression Type\n";
        }
    }

    void parse()
    {
        if (match(TokenType::CREATE))
        {
            rewind(); // Go back one token to reprocess CREATE in parseCreateStatement
            auto stmt = parseCreateStatement();
            printCreateStatement(*stmt);
        }
        else if (match(TokenType::INSERT))
        {
            rewind();
            auto stmt = parseInsertStatement();
            printInsertStatement(*stmt);
        }
        else if (match(TokenType::SELECT))
        {
            rewind();
            auto stmt = parseSelectStatement();
            // printSelectStatement(*stmt);
        }
        else
        {
            throw std::runtime_error("Unsupported SQL statement or missing statement type (CREATE, INSERT, SELECT)");
        }
    }

    void printSelectStatement(const SelectStatement &stmt, int indent = 0)
    {
        auto pad = [indent]()
        { for (int i = 0; i < indent; ++i) std::cout << "  "; };

        pad();
        std::cout << "SelectStatement\n";

        pad();
        std::cout << "  Columns:\n";
        for (const auto &col : stmt.columns)
        {
            pad();
            std::cout << "    - " << col << "\n";
        }

        pad();
        std::cout << "  From: " << stmt.table << "\n";

        if (stmt.whereClause)
        {
            pad();
            std::cout << "  Where:\n";
            printExpression(stmt.whereClause->condition.get(), indent + 2);
        }

        if (stmt.limitClause)
        {
            pad();
            std::cout << "  Limit: " << stmt.limitClause->limit << "\n";
        }
    }
    void printCreateStatement(const CreateStatement &stmt)
    {
        std::cout << "CREATE ";
        if (stmt.isDatabase)
        {
            std::cout << "DATABASE ";
        }
        else
        {
            std::cout << "TABLE ";
        }

        std::cout << stmt.name << "\n";

        if (!stmt.isDatabase)
        {
            for (const auto &col : stmt.columns)
            {
                std::cout << "  Column: " << col.name << " Type: " << col.type << "\n";

                for (const auto &constraint : col.constraints)
                {
                    std::string constraintStr;
                    switch (constraint)
                    {
                    case ColumnConstraint::NOT_NULL:
                        constraintStr = "NOT NULL";
                        break;
                    case ColumnConstraint::PRIMARY_KEY:
                        constraintStr = "PRIMARY KEY";
                        break;
                    case ColumnConstraint::AUTO_INCREMENT:
                        constraintStr = "AUTO_INCREMENT";
                        break;
                    case ColumnConstraint::UNIQUE:
                        constraintStr = "UNIQUE";
                        break;
                    default:
                        constraintStr = "UNKNOWN";
                        break;
                    }
                    std::cout << "    Constraint: " << constraintStr << "\n";
                }
            }
        }
    }

    void printInsertStatement(const InsertStatement &stmt)
    {
        std::cout << "INSERT INTO " << stmt.tableName << " (\n";
        for (const auto &col : stmt.columns)
        {
            std::cout << "  " << col << "\n";
        }
        std::cout << ") VALUES (\n";
        for (const auto &val : stmt.values)
        {
            std::cout << "  " << val << "\n";
        }
        std::cout << ");\n";
    }
};

#endif
