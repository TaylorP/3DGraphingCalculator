#ifndef TOKEN_INCLUDE 
#define TOKEN_INCLUDE

#include <string>

enum TokenType
{
	Base,
	Literal,
	Variable,
	Operator
};

class Token
{
protected:
	std::string _value;
	TokenType	_type;
	double _valueAsFloat;

public:
	Token ( TokenType type, std::string repres );
	Token ( double value );
	std::string	toString ( );
	double		evaluate ( );
	double		getValue ( );
	TokenType	getType ( );
};

#endif