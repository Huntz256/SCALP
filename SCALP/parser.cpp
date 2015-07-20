/* 
 * Implements the Parser class in parser.h
 * The class may be modified later for use in building an Abstract Syntax Tree.
*/

#include "parser.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Parse expression passed in as t_text
void Parser::parse(const char* t_text) {
	text = t_text;
	index = 0;
	getNextToken();
	expression();
}

// Skips all whitespaces between two tokens
void Parser::skipWhitespaces() {
	while (isspace(text[index])) {
		index++;
	}
}

// Extracts (get the token's type/value/symbol and stores it into token) the next token from the expression
// If an illegal token appears, throw an exception
void Parser::getNextToken() {
	skipWhitespaces();
	token.value = 0;
	token.symbol = 0;

	// If we have reached the end of text, than this token is endOfText
	if (text[index] == 0) {
		token.type = endOfText;
		return;
	}

	// If the current character is a digit, than this token is a number
	if (isdigit(text[index])) {
		token.type = number;
		token.value = getNumber();
		return;
	}

	// Let this token be an error for now
	token.type = error;

	// If the current character is an operator or paretheses, than this token is an operator or paretheses
	switch (text[index]) {
	case '+': token.type = plus; break;
	case '-': token.type = minus; break;
	case '*': token.type = mul; break;
	case '/': token.type = division; break;
	case '(': token.type = openParen; break;
	case ')': token.type = closenParen; break;
	}

	// If this token isn't an error, set its symbol
	if (token.type != error) {
		token.symbol = text[index];
		index++;
	}
	// If this token's type is still error, we have a problem
	else {
		throw "Unexpected token!";
	}
}

// Returns the number at the current location in the expression
double Parser::getNumber() {
	skipWhitespaces();

	// Handles decimal numbers as well
	int i = index;
	while (isdigit(text[index])) {
		index++;
	}
	if (text[index] == '.') {
		index++;
	}
	while (isdigit(text[index])) {
		index++;
	}

	if (index - i == 0) {
		throw "Number expected, but not found!";
	}

	char buffer[32] = { 0 };

	// Copy the values of (index - 1) bytes from &text[index] to buffer
	memcpy(buffer, &text[index], index - i);

	// Parses the characters in buffer, returning its value as a double
	return atof(buffer);
}

// For the functions below, EXP, TERM, FACTOR, etc. are called non-terminal symbols
// +, -, *, /, (, ), and numbers are called terminal symbols.
// All non-terminal symbols can be broken down into terminal symbols.
// Example: For the expression "1+2", EXP -> TERM EXP1 -> FACTOR TERM1 EXP1 -> 1 TERM1 EXP1 -> 1 (nothing) EXP1 
//   -> 1 + TERM EXP1 -> 1 + FACTOR TERM1 EXP1 -> 1 + 1 (nothing) (nothing) -> 1 + 1

// Break an EXP down to TERM EXP1
void Parser::expression() {
	term();
	expression1();
}

// Break an EXP1 down into + TERM EXP1 or - TERM EXP1 or nothing
void Parser::expression1() {
	switch (token.type) {
	case plus: case minus:
		getNextToken();
		term();
		expression1();
		break;
	default:
		break;
		//nothing
	}
}

// Break a TERM down into FACTOR TERM1
void Parser::term() {
	factor();
	term1();
}

// Break a TERM1 down into * FACTOR TERM1 or / FACTOR TERM1 or nothing
void Parser::term1() {
	switch (token.type) {
	case mul: case division:
		getNextToken();
		factor();
		term1();
		break;
	default:
		break;
		//nothing
	}
}

// Break a FACTOR down into ( EXP ) or - EXP or a number
void Parser::factor() {
	switch (token.type) {
	case openParen:
		getNextToken();
		expression();
		match(')');
		break;
	case minus:
		getNextToken();
		factor();
		break;
	case number:
		getNextToken();
		break;
	default:
		throw "Unexcepted token! Oh no!";
	}
}

// Used to match parenthesis
void Parser::match(char expected) {
	if (text[index - 1] == expected) {
		getNextToken();
	}
	else {
		throw "Expected another token at this position!";
	}
}