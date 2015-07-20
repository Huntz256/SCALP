/* 
 * Declares a Parser class that, given an expression, makes sure it is correct syntactically.
 * This class may be modified later for use in building an Abstract Syntax Tree.
*/

//#define guard prevents multiple inclusion
#ifndef SCALP_PARSER_H_
#define SCALP_PARSER_H_

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
//
// Sample usage:
//   Parser parser;
//   try { 
//     parser.parse(text); 
//   } 
//   catch (char* excpetion) {
//     std::cout << "Error." << std::endl; 
//   }
class Parser {
	// Stores the expression that Parser is parsing
	const char* text;

	/// Used to store a token in the expression
	Token token;

	// This keeps track of where we are in the expression
	// size_t is the type used to represent sizes (as its name implies), but can also be used as an int
	size_t index;

	// Extracts the next token in the expression
	void getNextToken();

	// Returns the number at the current location in the expression
	double getNumber();

	// A function for each non-terminal symbol (EXP, EXP1, TERM, TERM1, FACTOR)
	// See comments in parser.cpp for further details
	void expression();
	void expression1();
	void term();
	void term1();
	void factor();

	// Used to match parenthesis
	void match(char expected);

	// Skips all whitespaces between two tokens
	void skipWhitespaces();
	
public:
	// Parse expression passed in as t_text
	void parse(const char* t_text);
};

#endif //SCALP_PARSER_H_