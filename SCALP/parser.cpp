/* 
 * Implements the Parser class in parser.h: 
 * Given an expression, returns an abstract syntax tree representing that expression
 * Throws an ParserException if the expression given is invalid.
 * See parser.h for sample useage.
*/

#include "parser.h"
#include "ast.h"
#include <ctype.h>
#include <stdlib.h>
#include <sstream>

// Parse expression passed in as t_text and return an AST; this is the main function of the class
ASTNode* Parser::parse(const char* t_text) {
	this->text = t_text;
	this->index = 0;
	this->getNextToken();
	return this->expression();
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

	// If we have reached the end of text, then this token is endOfText
	if (text[index] == 0) {
		token.type = endOfText;
		return;
	}

	// If the current character is a digit, then this token is a number
	if (isdigit(text[index])) {
		token.type = number;
		token.value = getNumber();
		return;
	}

	// Let this token be an error for now
	token.type = error;

	// If the current character is an operator or paretheses, then this token is an operator or paretheses
	switch (text[index]) {
	case '+': token.type = plus; break;
	case '-': token.type = minus; break;
	case '*': token.type = mul; break;
	case '/': token.type = division; break;
	case '(': token.type = openParen; break;
	case ')': token.type = closedParen; break;
	}

	// If this token isn't an error, set its symbol
	if (token.type != error) {
		token.symbol = text[index];
		index++;
	}
	// If this token's type is still error, we have a problem
	else {
		std::stringstream sstr;
		sstr << "Invalid token '" << text[index] << "' spotted at position: " << index << ".";
		throw ParserException(sstr.str(), index);
	}
}

// Returns the number at the current location in the expression
// If a number wasn't found, throw an exception
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
		throw ParserException("Number expected, but not found!", index);
	}

	char buffer[32] = { 0 };

	// Copy the values of (index - 1) bytes from &text[i] to buffer
	memcpy(buffer, &text[i], index - i);

	// Parses the characters in buffer, returning its value as a double
	return atof(buffer);
}

// For the functions below, EXP, TERM, FACTOR, etc. are called non-terminal symbols
// +, -, *, /, (, ), and numbers are called terminal symbols.
// All non-terminal symbols can be broken down into terminal symbols.
// Example: For the expression "1+2", EXP -> TERM EXP1 -> FACTOR TERM1 EXP1 -> 1 TERM1 EXP1 -> 1 (nothing) EXP1 
//   -> 1 + TERM EXP1 -> 1 + FACTOR TERM1 EXP1 -> 1 + 2 (nothing) (nothing) -> 1 + 2

// Break an EXP down to TERM EXP1
ASTNode* Parser::expression() {
	ASTNode* termNode = term();
	ASTNode* expression1Node = expression1();

	return createNode(operatorPlus, termNode, expression1Node);
}

// Break an EXP1 down into + TERM EXP1 or - TERM EXP1 or a number 0
ASTNode* Parser::expression1() {
	ASTNode* termNode;
	ASTNode* expression1Node;

	switch (token.type) {
	case plus:
		getNextToken();
		termNode = term();
		expression1Node = expression1();
		return createNode(operatorPlus, expression1Node, termNode);
	case minus:
		getNextToken();
		termNode = term();
		expression1Node = expression1();
		return createNode(operatorMinus, expression1Node, termNode);
	}

	return createNumberNode(0);
}

// Break a TERM down into FACTOR TERM1
ASTNode* Parser::term() {
	ASTNode* factorNode = factor();
	ASTNode* term1Node = term1();

	return createNode(operatorMul, factorNode, term1Node);
}

// Break a TERM1 down into * FACTOR TERM1 or / FACTOR TERM1 or a number 1
ASTNode* Parser::term1() {
	ASTNode* factorNode;
	ASTNode* term1Node;

	switch (token.type) {
	case mul: 
		getNextToken();
		factorNode = factor();
		term1Node = term1();
		return createNode(operatorMul, term1Node, factorNode);
	case division:
		getNextToken();
		factorNode = factor();
		term1Node = term1();
		return createNode(operatorDivision, term1Node, factorNode);
	}

	return createNumberNode(1);
}

// Break a FACTOR down into ( EXP ) or - EXP or a number
ASTNode* Parser::factor() {
	ASTNode* node;
	switch (token.type) {
	case openParen:
		getNextToken();
		node = expression();
		match(')');
		return node;
	case minus:
		getNextToken();
		node = factor();
		return createUnaryMinusNode(node);
	case number:
		{
			double value = token.value;
			getNextToken();
			return createNumberNode(value);
		}
	default:
		std::stringstream sstr;
		sstr << "Unexpected token '" << token.symbol << "' at position: " << index - 1 << "."; //not sure why index is 1 ahead here...
		throw ParserException(sstr.str(), index - 1);
	}
}

// Used to match parenthesis
void Parser::match(char expected) {
	if (text[index - 1] == expected) {
		getNextToken();
	}
	else {
		std::stringstream sstr;
		sstr << "Expected token '" << expected << "' at position: " << index << ".";
		throw ParserException(sstr.str(), index);
	}
}

// Creates a node that looks like this [type]-[]-[LEFT]-[RIGHT]
ASTNode* Parser::createNode(ASTNodeType type, ASTNode* left, ASTNode* right) {
	ASTNode* node = new ASTNode;
	node->type = type;
	node->left = left;
	node->right = right;
	return node;
}

// Creates a node that looks like this [unaryMinus]-[]-[LEFT]-[]
ASTNode* Parser::createUnaryMinusNode(ASTNode* left) {
	ASTNode* node = new ASTNode;
	node->type = unaryMinus;
	node->left = left;
	node->right = NULL;
	return node;
}

// Creates a leaf node that looks like this [numberValue]-[value]-[]-[]
ASTNode* Parser::createNumberNode(double value) {
	ASTNode* node = new ASTNode;
	node->type = numberValue;
	node->value = value;
	return node;
}

// Implementation of ParserException method used to throw exceptions with custom messages
ParserException::ParserException(const std::string& message, int pos) : std::exception(message.c_str()), position(pos) {
	
};
