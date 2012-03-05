#include "Token.h"

Token::Token ( TokenType type, std::string repres )
{
	_type = type;
	_value = repres;
}

Token::Token ( double value ) 
{
	_valueAsFloat = value;
	_type = Literal;
}

double Token::getValue ( ) 
{
	return _valueAsFloat;
}

TokenType Token::getType ( ) 
{
	return _type;
}

std::string Token::toString ( ) 
{
	return _value;
}