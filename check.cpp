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
 CREATE TABLE Persons (
    Personid int NOT NULL AUTO_INCREMENT PRIMARY KEY ,
    LastName varchar(255) NOT NULL,
    FirstName varchar(255),
    Age int,
    
);  
)";

    Lexer lexer(s);

    vector<Token*> tokens = lexer.tokenize();

    for (Token* token : tokens) {
        cout << typeToString(token->TYPE) << " : " << token->VALUE << endl;
    }

     Parser parser(tokens);
      std::unique_ptr<CreateStatement> stmt = parser.parseCreateStatement();

    
        parser.printCreateStatement(*stmt);

    return 0;
}
