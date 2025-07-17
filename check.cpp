#include <iostream>
#include <string>
#include <vector>
#include "SQL_LEXER.hpp"
#include "SQL_PARSER.hpp"
using namespace std;

std::string typeToString(TokenType TYPE); // already declared

int main(int argc, char const *argv[]) {
    vector<string> testSQLs = {
        R"(CREATE TABLE users (
            id INT NOT NULL PRIMARY KEY,
            name VARCHAR(100) NOT NULL,
            email VARCHAR(100) UNIQUE
        );)",

        R"(INSERT INTO users (
            name,
            age,
            email
        ) VALUES (
            'Alice',
            30,
            'alice@example.com'
        );)",

        R"(SELECT name, age, email FROM users 
           WHERE age >= 18 AND status != 'inactive' 
           OR (country = 'USA' AND subscribed = 1)
           ORDERBY age;)",

    };

    for (const auto& sql : testSQLs) {
        cout << "\n=============================\n";
        cout << "SQL:\n" << sql << endl;
        cout << "=============================\n";

        try {
            Lexer lexer(sql);
            vector<Token*> tokens = lexer.tokenize();

            // Print tokens (optional)
            cout << "Tokens:\n";
            for (Token* token : tokens) {
                cout << typeToString(token->TYPE) << " : " << token->VALUE << endl;
            }

            // Parse
            Parser parser(tokens);
            parser.parse(); // This should call the correct parser (SELECT, INSERT, etc.)

        } catch (const std::exception& e) {
            cerr << "Error: " << e.what() << endl;
        }

        cout << "\n";
    }

    return 0;
}
