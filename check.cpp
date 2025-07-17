#include <iostream>
#include <string>
#include <vector>
#include "SQL_LEXER.hpp"
#include "SQL_PARSER.hpp"

using namespace std;

std::string typeToString(TokenType TYPE); // already declared

int main(int argc, char const *argv[]) {
    vector<string> testSQLs = {
      
        R"(CREATE TABLE a (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(255) NOT NULL,
    email VARCHAR(255) UNIQUE
);
)",

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
