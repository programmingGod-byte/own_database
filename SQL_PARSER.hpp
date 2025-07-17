#ifndef __PARSER_AST_HPP
#define __PARSER_AST_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "SQL_LEXER.hpp"

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

struct DropStatement:public ASTNode{
    bool istable;
    std::string name;
    
    
    ASTNodeType getType() const override { return ASTNodeType::DROP_STATEMENT; }

};



struct CreateStatement:public ASTNode{
    bool isDatabase;
    std::string name;

    
    
    ASTNodeType getType() const override { return ASTNodeType::DROP_STATEMENT; }

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
    Parser(const std::vector<Token *> &tokens) : tokens(tokens) {}


    std::unique_ptr<DropStatement> parseDropStatement(){
        expect(TokenType::DROP,"Expected drop keyword");
        auto stmt = std::make_unique<DropStatement>();
        Token * token = advance();
        switch (token->TYPE)
        {
        case TokenType::TABLE:{
            Token * identifier = expect(TokenType::IDENTIFIER,"not a identifier\n");
            stmt->name = identifier->VALUE;
            stmt->istable = true;
        }
            
            break;
        case TokenType::DATABASE:{
            Token * identifier = expect(TokenType::IDENTIFIER,"not a identifier\n");
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


    void printExpression(const Expression* expr, int indent) {
    auto pad = [indent]() { for (int i = 0; i < indent; ++i) std::cout << "  "; };

    if (!expr) return;

    switch (expr->getType()) {
        case ASTNodeType::IDENTIFIER: {
            const auto* id = static_cast<const Identifier*>(expr);
            pad(); std::cout << "Identifier: " << id->name << "\n";
            break;
        }
        case ASTNodeType::INT_LITERAL: {
            const auto* num = static_cast<const IntLiteral*>(expr);
            pad(); std::cout << "IntLiteral: " << num->value << "\n";
            break;
        }
        case ASTNodeType::STRING_LITERAL: {
            const auto* str = static_cast<const StringLiteral*>(expr);
            pad(); std::cout << "StringLiteral: \"" << str->value << "\"\n";
            break;
        }
        case ASTNodeType::COMPARISON_EXPRESSION: {
            const auto* comp = static_cast<const ComparisonExpression*>(expr);
            pad(); std::cout << "ComparisonExpression: ";

            switch (comp->op) {
                case ComparisonOperator::EQUAL: std::cout << "==\n"; break;
                case ComparisonOperator::NOT_EQUAL: std::cout << "!=\n"; break;
                case ComparisonOperator::GREATER: std::cout << ">\n"; break;
                case ComparisonOperator::LESS: std::cout << "<\n"; break;
                case ComparisonOperator::GREATER_EQUAL: std::cout << ">=\n"; break;
                case ComparisonOperator::LESS_EQUAL: std::cout << "<=\n"; break;
            }

            printExpression(comp->left.get(), indent + 1);
            printExpression(comp->right.get(), indent + 1);
            break;
        }
        case ASTNodeType::LOGICAL_EXPRESSION: {
            const auto* log = static_cast<const LogicalExpression*>(expr);
            pad(); std::cout << "LogicalExpression: " << (log->op == LogicalOperator::AND ? "AND" : "OR") << "\n";
            printExpression(log->left.get(), indent + 1);
            printExpression(log->right.get(), indent + 1);
            break;
        }
        case ASTNodeType::PARENTHESIZED_EXPRESSION: {
            const auto* paren = static_cast<const ParenthesizedExpression*>(expr);
            pad(); std::cout << "ParenthesizedExpression:\n";
            printExpression(paren->expression.get(), indent + 1);
            break;
        }
        default:
            pad(); std::cout << "Unknown Expression Type\n";
    }
}


    void printSelectStatement(const SelectStatement *stmt, int indent = 0)
    {
        auto pad = [indent]()
        { for (int i = 0; i < indent; ++i) std::cout << "  "; };

        pad();
        std::cout << "SelectStatement\n";

        pad();
        std::cout << "  Columns:\n";
        for (const auto &col : stmt->columns)
        {
            pad();
            std::cout << "    - " << col << "\n";
        }

        pad();
        std::cout << "  From: " << stmt->table << "\n";

        if (stmt->whereClause)
        {
            pad();
            std::cout << "  Where:\n";
            printExpression(stmt->whereClause->condition.get(), indent + 2);
        }

        if (stmt->limitClause)
        {
            pad();
            std::cout << "  Limit: " << stmt->limitClause->limit << "\n";
        }
    }
};

#endif
