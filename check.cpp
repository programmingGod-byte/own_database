#include <iostream>
#include <string>
#include <vector>
#include "SQL_LEXER.hpp"

using namespace std;

std::string typeToString(TokenType TYPE); 

int main(int argc, char const *argv[])
{
   string s = R"(
    SELECT name, age, email 
    FROM users 
    WHERE age >= 18 AND status != 'inactive' OR (country = 'USA' AND subscribed = true) 
    ORDERBY age 
    INSERT INTO logs (event, created_at) VALUES ('login', '2025-07-13') 
    UPDATE users SET last_login = '2025-07-13' WHERE id = 123 
    DELETE FROM sessions WHERE expires < '2025-01-01'
)";

    Lexer lexer(s);

    vector<Token*> tokens = lexer.tokenize();

    for (Token* token : tokens) {
        cout << typeToString(token->TYPE) << " : " << token->VALUE << endl;
    }

    for (Token* token : tokens) {
        delete token;
    }

    return 0;
}
