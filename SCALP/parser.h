/* 
 * Declares a Parser class that, given an expression, returns an abstract syntax tree representing that expression
 * Throws an ParserException if the expression given is invalid.
 *
 *  Sample usage:
 *   Parser parser; ASTNode* ast;
 *   try {
 *     ast = parser.parse(text);
 *   }
 *   catch (const ParserException& e) {
 *     std::cout << "\"" << text << "\"" << "  INVALID: " << e.what() << "\n\n";
 *   }
*/

//#define guard prevents multiple inclusion; follows Google style guard naming convention (<PROJECT>_<FILE>_H_)
#ifndef SCALP_PARSER_H_
#define SCALP_PARSER_H_

#include <exception>
#include <string>
#include "ast.h"

//Let TokenType::error = 0, TokenType::plus = 1, and so on
//Also limits TokenType to these tokens
enum TokenType {
	error,
	plus,
	minus,
	mul,
	division,
	endOfText,
	openParen,
	closenParen,
	number
};

// Define a Token structure to indicate the type of the last extracted token and its value
// A Token is a symbol extracted one at a time from input text, where possible tokens include
// '+', '-', '/', '*', '(', ')', numbers, and the end of text.
struct Token {
	TokenType type;

	// Used to store the token's value if is a number
	double value;

	// Used to store the token' symbol if is a non-numberic character
	char symbol;
};

// Given an expression, makes sure it is correct syntactically.
class Parser {
	// Stores the expression that Parser is parsing
	const char* text;

	/// Used to store a token in the expression
	Token token;

	// This keeps track of where we are in the expression
	// size_t is the type commonly used to represent sizes (as its name implies) and counts (like indexes), but can also be used as an unsigned int
	size_t index;

	// Extracts the next token in the expression
	void getNextToken();

	// Returns the number at the current location in the expression
	double getNumber();

	// A function for each non-terminal symbol (EXP, EXP1, TERM, TERM1, FACTOR)
	// See comments in parser.cpp for further details
	ASTNode* expression();
	ASTNode* expression1();
	ASTNode* term();
	ASTNode* term1();
	ASTNode* factor();

	// Used for AST node creation
	ASTNode* createNode(ASTNodeType type, ASTNode* left, ASTNode* right);
	ASTNode* createUnaryMinusNode(ASTNode* left);
	ASTNode* createNumberNode(double value);

	// Used to match parentheses
	void match(char expected);

	// Skips all whitespaces between two tokens
	void skipWhitespaces();
	
public:
	// Parse expression passed in as t_text
	ASTNode* parse(const char* t_text);
};

//Custome ParserException class derived from the base exception class defined in the standard library
class ParserException : public std::exception{
	int position; //The position (AKA the index) at which the exception occurs

public:
	ParserException(const std::string& message, int pos); //Exception method that takes in a custom message, as well as the position of the exception
};

#endif //SCALP_PARSER_H