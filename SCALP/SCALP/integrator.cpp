/*
* Implements the Integrator class in integrator.h
* See comments in integrator.h for more details
*/

#include "integrator.h"
#include "evaluator.h"

const std::string TABLE_LOOKUP_FAIL = "ERROR";

ASTNode* Integrator::applySafeTransform(ASTNode* t_ast) {
	// If ast is NULL, something has gone wrong
	if (t_ast == NULL) {
		throw EvaluatorException("Abstract syntax tree is NULL");
	}

	ASTNode* ast = t_ast;
	return ast;
}

std::string Integrator::lookInTable(ASTNode* t_ast) {

	ASTNode* ast = t_ast;

	// If ast is NULL, something has gone wrong
	if (ast == NULL) {
		throw EvaluatorException("Abstract syntax tree is NULL");
	}

	// (a) If ast is 1 / x, return ln x
	else if ((ast->type == operatorDivision) && (ast->left->type == numberValue) && (ast->left->value == 1) && (ast->right->type == variableChar) && (ast->right->var != 0)) {
		return "ln(" + std::string(1, ast->right->var) + ")";
	}

	// (b) If ast is x^n, return (x^(n + 1)) / (n + 1)
	else if ((ast->type == operatorPower) && (ast->left->type == variableChar) && (ast->left->var != 0) && (ast->right->type == numberValue) && (ast->right->value != 0)) {
		int n = ast->right->value;
		return "(" + std::string(1, ast->left->var) + "^" + std::to_string(n + 1) + ")" + "/" + std::to_string(n + 1);
	}

	// (b) If ast is x, return (x^2) / (2)
	else if ((ast->type == variableChar) && (ast->var != 0) && (ast->left == NULL) && (ast->right == NULL)) {
		return "(" + std::string(1, ast->var) + "^2)/2";

	}
	// (c) If ast is cos x, return sin x
	else if ((ast->type == functionCos) && (ast->left->var != 0)) {
		return "sin(" + std::string(1, ast->left->var) + ")";
	}
	// (d) If ast is a non-zero number value, return that number * x
	else if ((ast->type == numberValue) && (ast->value != 0)) {
		return std::to_string((int)ast->value) + "x";
	}

	// If ast is a zero number value, return nothing
	else if ((ast->type == numberValue) && (ast->value == 0)) {
		return "";
	}


	// If ast is not in table, return TABLE_LOOKUP_FAIL
	else {
		return TABLE_LOOKUP_FAIL;
	}
	
}

std::string Integrator::integrate(ASTNode* t_ast) {
	ASTNode* ast = t_ast; 
	std::string solution = "";

	// If ast represents the integral of a sum such as "1+2", return the integral of the evaluated sum "3"
	// If ast reprsents the integral of a sum such as "x^2 + x", return the sum of the integrals
	if (ast->type == operatorPlus) {
		try {
			Evaluator evaluator;
			double val = evaluator.evaluate(ast);
			ASTNode* ast2 = new ASTNode; 
			ast2->type = numberValue; ast2->value = val;
			return integrate(ast2);
		}
		catch (EvaluatorException& exception) {
			return integrate(ast->left) + " + " + integrate(ast->right);
		}
	}
	// If ast reprsents the integral of a sum such as "x^2 - x", return the sum of the integrals
	else if (ast->type == operatorMinus) {
		return integrate(ast->left) + " - " + integrate(ast->right);
	}

	// If ast represents the integral of a product of x times 1, return the integral of x
	else if (ast->type == operatorMul && (ast->left->value == 1)) {
		return integrate(ast->right);
	}
	// If ast represents the integral of a product of 1 times x, return the integral of x
	else if (ast->type == operatorMul && (ast->right->value == 1)) {
		return integrate(ast->left);
	}

	// If ast represents the integral of a product of x times n, return n times the integral of x
	else if (ast->type == operatorMul && (ast->left->value > 0)) {
		return std::to_string((int)ast->left->value) + integrate(ast->right);
	}
	// If ast represents the integral of a product of n times x, return n times the integral of x
	else if (ast->type == operatorMul && (ast->right->value > 0)) {
		return std::to_string((int)ast->right->value) + integrate(ast->left);
	}

	solution = lookInTable(ast);

	return solution;
}