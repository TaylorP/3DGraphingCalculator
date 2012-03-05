//Written by Jason Chan

#ifndef RPN_BUILDER_INCLUDE 
#define RPN_BUILDER_INCLUDE

#include "Token.h"
#include <vector>
#include <stack>

class RPNBuilder
{
private: 
	char* opList;
	
	std::vector<Token> buildTokenList ( std::string s );
	std::vector<Token> toRPN ( std::vector<Token> );
	
	int findInOpList ( char v );
	int opLevel(std::string op);
	void preprocessString ( std::string &s ) ;

public:
	RPNBuilder();
	std::vector<Token> convertToRPN ( std::string expression );
};

#endif