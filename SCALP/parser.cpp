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

	// If the current character is a letter, figure out if it's a function or variable
	if (isalpha(text[index])) {
		token.type = getFunction();
		if (token.type != error) {
			// Possibly need to set token.function?
			return;
		}
		else {
			token.type = variable;
		}
	}
	else {
		// If the current character is an operator or parethesis, then this token is an operator or parethesis
		switch (text[index]) {
		case '+': token.type = plus; break;
		case '-': token.type = minus; break;
		case '*': token.type = mul; break;
		case '/': token.type = division; break;
		case '(': token.type = openParen; break;
		case ')': token.type = closenParen; break;
		case '^': token.type = caret; break;
		//case ',': token.type = comma; break;
		}

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
		std::stringstream sstr;
		sstr << "Number expected at position: " << index << ", but not found.";
		throw ParserException(sstr.str(), index);
	}

	char buffer[32] = { 0 };

	// Copy the values of (index - 1) bytes from &text[i] to buffer
	memcpy(buffer, &text[i], index - i);

	// Parses the characters in buffer, returning its value as a double
	return atof(buffer);
}

// Helper method called by Parser::getFunction() to make sure a function is followed by parentheses
void Parser::requireParen(){
	if (text[index] != '('){
		std::stringstream sstr;
		sstr << "Functions require parentheses. Found '" << text[index] << "' instead of '(' at position: " << index << ".";
		throw ParserException(sstr.str(), index);
	}
}

// Helper method called by Parser:getFunction() to specifically deal with logarithms
// Allows logs to have a specific base or just have an implied base of 10
// Ex: log(2,8) is valid, interpreted as log base-2 of 8
// Ex: log(5) is also valid, interpreted as log base-10 of 5
TokenType Parser::handleLog(){
	for (int i = index + 1; text[i] != ')' && text[i] != 0; i++){
		if (text[i] == ',') return binaryLog; // Checks to see if there's a comma that would signal a log base declaration
	}
	return unaryLog; // Else it's just a base-10 log
}

// Given an index, returns the function located at that position or error if there is none
TokenType Parser::getFunction(){
	int pos = index; 
	if (text[pos] == 's'){
		if (text[pos + 1] == 'i' && text[pos + 2] == 'n'){
			index += 3; requireParen(); return sine;
		}
		else if (text[pos + 1] == 'e' && text[pos + 2] == 'c'){
			index += 3; requireParen(); return secant;
		}
		else return error;
	}
	else if (text[pos] == 'c'){
		if (text[pos + 1] == 's' && text[pos + 2] == 'c'){
			index += 3; requireParen(); return cosecant;
		}
		else if (text[pos + 1] == 'o'){
			if (text[pos + 2] == 's'){
				index += 3; requireParen(); return cosine;
			}
			else if (text[pos + 2] == 't'){
				index += 3; requireParen(); return cotangent;
			}
			else return error;
		}
		else return error;
	}
	else if (text[pos] == 't' && text[pos + 1] == 'a' && text[pos + 2] == 'n'){
		index += 3; requireParen(); return tangent;
	}
	else if (text[pos] == 'l'){
		if (text[pos + 1] == 'n'){
			index += 2; requireParen(); return naturalLog;
		}
		else if (text[pos + 1] == 'o' && text[pos + 2] == 'g'){
			index += 3; requireParen(); return handleLog();
		}
		else return error;
	}
	else return error;
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

// Break a FACTOR down into EXPONENT FACTOR1
ASTNode* Parser::factor() {
	ASTNode* exponentNode = exponent();
	ASTNode* factor1Node = factor1();

	return createNode(operatorPower, exponentNode, factor1Node);
}

// Break a FACTOR1 down into ^ EXPONENT or a number 1
ASTNode* Parser::factor1(){
	ASTNode* exponentNode;
	ASTNode* factor1Node;

	if (token.type == caret) {
		getNextToken();
		exponentNode = exponent();
		factor1Node = factor1();
		return createNode(operatorPower, factor1Node, exponentNode);
	}

	return createNumberNode(1);
}

// Break an EXPONENT down into ( EXP ) or - EXP or a number or a variable
ASTNode* Parser::exponent(){
	if (text[index] == 0) throw ParserException("Unexpected termination of expression.", index);

	ASTNode* node;
	switch (token.type) {
	case openParen:
		if (text[index] == ')'){
			std::stringstream sstr;
			sstr << "Missing expression after position: " << index - 1 << ".";
			throw ParserException(sstr.str(), index);
		}
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
	case variable:
	{
		char var = token.symbol;
		getNextToken();
		return createVariableNode(var);
	}
	case sine: 
		getNextToken();
		node = expression();
		return createNode(functionSin, node, NULL);
	case cosine: 
		getNextToken();
		node = expression();
		return createNode(functionCos, node, NULL);
	case tangent: 
		getNextToken();
		node = expression();
		return createNode(functionTan, node, NULL);
	case secant: 
		getNextToken();
		node = expression();
		return createNode(functionSec, node, NULL);
	case cosecant: 
		getNextToken();
		node = expression();
		return createNode(functionCsc, node, NULL);
	case cotangent: 
		getNextToken();
		node = expression();
		return createNode(functionCot, node, NULL);
	case unaryLog:
	{
		getNextToken();
		ASTNode *baseNode = createNumberNode(10);
		node = expression();
		return createNode(functionLog, baseNode, node);
	}
	case binaryLog:
	{
		getNextToken();
		ASTNode *baseNode = createNumberNode(getNumber());
		if (text[index] == ',') index++; // Skips the comma
		else{
			std::stringstream sstr;
			sstr << "Expected ',' at position: " << index << ".";
			throw ParserException(sstr.str(), index);
		}
		node = expression();
		return createNode(functionLog, baseNode, node);
	}
	case naturalLog:
		getNextToken();
		node = expression();
		return createNode(functionLn, node, NULL);
	default:
		std::stringstream sstr;
		sstr << "Unexpected token '" << text[index] /*token.symbol*/ << "' at position: " << index << "."; //not sure why index is sometimes 1 ahead here...
		throw ParserException(sstr.str(), index - 1);
	}
}

// Used to match parentheses
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

// Creates a node that looks like this [type]-[]-[]-[LEFT]-[RIGHT]
ASTNode* Parser::createNode(ASTNodeType type, ASTNode* left, ASTNode* right) {
	ASTNode* node = new ASTNode;
	node->type = type;
	node->left = left;
	node->right = right;
	return node;
}

// Creates a node that looks like this [unaryMinus]-[]-[]-[LEFT]-[]
ASTNode* Parser::createUnaryMinusNode(ASTNode* left) {
	ASTNode* node = new ASTNode;
	node->type = unaryMinus;
	node->left = left;
	node->right = NULL;
	return node;
}

// Creates a leaf node that looks like this [numberValue]-[value]-[]-[]-[]
ASTNode* Parser::createNumberNode(double value) {
	ASTNode* node = new ASTNode;
	node->type = numberValue;
	node->value = value;
	return node;
}

// Creates a leaf node that looks like this [variableChar]-[]-[var]-[]-[]
ASTNode* Parser::createVariableNode(char var) {
	ASTNode* node = new ASTNode;
	node->type = variableChar;
	node->var = var;
	return node;
}

// Implementation of ParserException method used to throw exceptions with custom messages
// Yes, the curly brackets are supposed to be empty
ParserException::ParserException(const std::string& message, int pos) : std::exception(message.c_str()), position(pos) {};
