#ifndef __PARSER_H
#define __PARSER_H

#include<iostream>
#include<string>
#include<fstream>
#include<memory>
#include<vector>
#include "SQL_LEXER.hpp"

enum class ASTNodeType {
    STATEMENT,
    SELECT_STATEMENT,
    INSERT_STATEMENT,
    UPDATE_STATEMENT,
    DELETE_STATEMENT,
    EXPRESSION,
    IDENTIFIER,
    LITERAL,
    BINARY_EXPRESSION,
    COMPARISON_EXPRESSION,
    LIMIT_EXPRESSION,
    ORDERBY_EXPRESSION

};

struct ASTNode {
    ASTNodeType type;
    virtual ~ASTNode() = default;
};


// literal means string number etc


template<typename T>
struct Literal:public ASTNode{
    T value;
    
    Literal(T value):value(value){
        type  = ASTNodeType::LITERAL;
    }

};

struct LimitStatement:public ASTNode{
    size_t limit;
    LimitStatement(size_t limit):limit(limit){
        type  = ASTNodeType::LIMIT_EXPRESSION;
    }
};

// like name = "shivam" here name is identifier

struct Identifier : public ASTNode {
    std::string name;

    Identifier(std::string name) : name(name) {
        type = ASTNodeType::IDENTIFIER;
    }
};




struct SelectStatement : public ASTNode {
    std::vector<std::string> columns;
    std::string table;
    ASTNode* whereClause = nullptr;

    SelectStatement() {
        type = ASTNodeType::SELECT_STATEMENT;
    }
};


// like >= <= != ==
struct ComparsionExpression : public ASTNode {
    
};


struct WhereStatement : public ASTNode{

};




#endif