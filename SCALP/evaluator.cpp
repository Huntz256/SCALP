/*
* Implements the Evaluator class in evaluator.h
*/

#include "evaluator.h"

// This is a recursive function that traverses the abstract syntax tree
// that is passed in and returns a double that is the evaluation of the 
// expression that the abstract syntax tree represent.
double Evaluator::evaluateSubtree(ASTNode* ast) {

	// If ast is NULL, something has gone wrong
	if (ast == NULL) {
		throw EvaluatorException("Abstract syntax tree is NULL");
	}

	// If ast is a number, simply return that number
	if (ast->type == numberValue) {
		return ast->value;
	}

	// If ast is a unaryMinus type, traverse down the tree
	// by calling this function for ast->left
	// Return the value that function returns, negated
	else if (ast->type == unaryMinus) {
		return -evaluateSubtree(ast->left);
	}

	// Otherwise, traverse down the tree
	// by calling this function for ast->left and ast->right.
	else {
		double value1 = evaluateSubtree(ast->left);
		double value2 = evaluateSubtree(ast->right);
		switch (ast->type) {
		case operatorPlus: 
			return value1 + value2;
		case operatorMinus:
			return value1 - value2;
		case operatorMul:
			return value1 * value2;
		case operatorDivision:
			return value1 / value2;
		}
	}

	throw EvaluatorException("Incorrect syntax tree.");
}
// The main method of the evaluator class
double Evaluator::evaluate(ASTNode* ast) {
	if (ast == NULL) {
		throw EvaluatorException("Abstract syntax tree is NULL");
	}
	return evaluateSubtree(ast);
}

// EvaluatorException derived from the base exception class defined in the standard library
EvaluatorException::EvaluatorException(const std::string& message) : std::exception(message.c_str()) {

}
