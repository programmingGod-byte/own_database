#include <iostream>
#include <string>
#include <vector>

#include "SQL_LEXER.hpp"
#include "SQL_PARSER.hpp"
#include "initialLoad.hpp"

// ✅ Fix: Proper declaration with semicolon

using namespace std;
std::string typeToString(TokenType TYPE);

int main(int argc, char const *argv[]) {
    initialDatabseLoad();  // Load DB metadata
    initializePrimaryIndexBtrees();

    vector<string> testSQLs = {
       R"(
        CREATE DATABASE testing;
       )", R"(

CREATE TABLE testing(
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(255),
    email VARCHAR(255) UNIQUE
);

)"
    };

    for (const auto& sql : testSQLs) {
        cout << "\n=============================\n";
        cout << "SQL:\n" << sql << endl;
        cout << "=============================\n";

        try {
            Lexer lexer(sql);
            vector<Token*> tokens = lexer.tokenize();

            // Debug: Print tokens
            cout << "Tokens:\n";
            for (Token* token : tokens) {
                cout << typeToString(token->TYPE) << " : " << token->VALUE << endl;
            }

            Parser parser(tokens);
            parser.parse();  // Parse the SQL

        } catch (const std::exception& e) {
            cerr << "Error: " << e.what() << endl;
        }

        cout << "\n";
    }

    return 0;
}
