/* 
 * Declares a Parser class that, given an expression, makes sure it is correct syntactically.
 *
*/


//#define guard prevents multiple inclusion
#ifndef SCALP_PARSER_H_
#define SCALP_PARSER_H_

//Let TokenType::error = 0, TokenType::plus = 1, and so on
enum TokenType {
	error,
	plus,
	minus,
	mul,
	div,
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
	double value;
	char symbol;
};

// Given an expression, makes sure it is correct syntactically.
//
// Sample usage:
//   Parser parser;
//   try { 
//     parser.parse(text); 
//   } 
//   catch (ParserException& excpetion) {
//     std::cout << "Error." << std::endl; 
//   }
class Parser {
	Token token;
public:
	void parse(const char* text);
};


#endif //SCALP_PARSER_H_