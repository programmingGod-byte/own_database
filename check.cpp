#include <iostream>
#include <string>
#include <vector>
#include "SQL_LEXER.hpp"
#include "SQL_PARSER.hpp"
using namespace std;

std::string typeToString(TokenType TYPE); 

int main(int argc, char const *argv[])
{
   string s = R"(
    SELECT name, age, email 
    FROM users 
    WHERE age >= 18 AND status != 'inactive' OR (country = 'USA' AND subscribed = 1) 
    ORDERBY age 
    
)";

    Lexer lexer(s);

    vector<Token*> tokens = lexer.tokenize();

    for (Token* token : tokens) {
        cout << typeToString(token->TYPE) << " : " << token->VALUE << endl;
    }

     Parser parser(tokens);
      std::unique_ptr<SelectStatement> stmt = parser.parseSelectStatement();

    
        parser.printSelectStatement(stmt.get());

    return 0;
}
