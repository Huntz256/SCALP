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

// #define guard prevents multiple inclusion; follows Google style guard naming convention (<PROJECT>_<FILE>_H_)
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
	number,
	variable,
	caret,
	// Trig functions
	sine,
	cosine,
	tangent,
	secant,
	cosecant,
	cotangent,
	// Log functions
	unaryLog, // Base-10 log
	binaryLog, // Base-something else log
	naturalLog, // Log found in the wild and not manufactured by humans
	//comma // Used to declare the base in logarithms
};

// Define a Token structure to indicate the type of the last extracted token and its value
// A Token is a symbol extracted one at a time from input text, where possible tokens include
// '+', '-', '/', '*', '(', ')', numbers, and the end of text.
struct Token {
	TokenType type;

	// Used to store the token's value if it is a number
	double value;

	// Used to store the token's symbol if it is a non-numeric character
	char symbol;

	// Used to store the token's abbreviated function name (sin, cos, etc) if it is a function
	//char function[3];
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

	// Small helper method called by getFunction() in order to reduce the amount of code retyped
	// Checks to see if a function is followed by parentheses
	void requireParen();

	// Helper method called by getFunction() in order to specifically deal with logarithms (see implementation for details)
	TokenType handleLog();

	// Given an index, returns the function located at that position or error if there is none
	TokenType getFunction();

	// A function for each non-terminal symbol (EXP, EXP1, TERM, TERM1, FACTOR)
	// See comments in parser.cpp for further details
	ASTNode* expression();
	ASTNode* expression1();
	ASTNode* term();
	ASTNode* term1();
	ASTNode* factor();
	ASTNode* factor1();
	ASTNode* exponent();

	// Used for AST node creation
	ASTNode* createNode(ASTNodeType type, ASTNode* left, ASTNode* right);
	ASTNode* createUnaryMinusNode(ASTNode* left);
	ASTNode* createNumberNode(double value);
	ASTNode* createVariableNode(char var);

	// Used to match parentheses
	void match(char expected);

	// Skips all whitespaces between two tokens
	void skipWhitespaces();

	// Simplifies a given AST and returns the simplified AST
	ASTNode* simplify(ASTNode* t_ast);
	
public:
	// Parse expression passed in as t_text
	ASTNode* parse(const char* t_text);

};

// Custom ParserException class derived from the base exception class defined in the standard library
class ParserException : public std::exception{
	int position; //The position (AKA the index) at which the exception occurs

public:
	ParserException(const std::string& message, int pos); //Exception method that takes in a custom message, as well as the position of the exception
};


#endif // SCALP_PARSER_H_
