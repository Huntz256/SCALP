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

ASTNode* Integrator::lookInTable(ASTNode* t_ast) {

	ASTNode* ast = t_ast;
	ASTNode* newAst = new ASTNode();

	// If ast is NULL, something has gone wrong
	if (ast == NULL) {
		throw EvaluatorException("Abstract syntax tree is NULL");
	}

	// (a) If ast is 1 / x, return ln x
	else if ((ast->type == operatorDivision) && (ast->left->type == numberValue) && (ast->left->value == 1) && (ast->right->type == variableChar) && (ast->right->var != 0)) {
		newAst->left = new ASTNode; newAst->left = ast->right; 
		newAst->right = new ASTNode; newAst->right = NULL; newAst->type = functionLn;
	}

	// (b) If ast is x^n, return (x^(n + 1)) / (n + 1)
	else if ((ast->type == operatorPower) && (ast->left->type == variableChar) && (ast->left->var != 0) && (ast->right->type == numberValue) && (ast->right->value != 0)) {
		newAst->type = operatorDivision; 
		newAst->left = new ASTNode; newAst->left->type = operatorPower; 
		newAst->right = new ASTNode; newAst->right->type = numberValue; newAst->right->value = ast->right->value + 1;
		newAst->left->left = new ASTNode; newAst->left->left->type = variableChar; newAst->left->left->var = ast->left->var;
		newAst->left->right = new ASTNode; newAst->left->right->type = numberValue; newAst->left->right->value = ast->right->value + 1;
	}

	// (b) If ast is x, return (x^2) / (2)
	else if ((ast->type == variableChar) && (ast->var != 0) && (ast->left == NULL) && (ast->right == NULL)) {
		newAst->type = operatorDivision;
		newAst->left = new ASTNode; newAst->left->type = operatorPower;
		newAst->right = new ASTNode; newAst->right->type = numberValue; newAst->right->value = 2;
		newAst->left->left = new ASTNode; newAst->left->left->type = variableChar; newAst->left->left->var = ast->var;
		newAst->left->right = new ASTNode; newAst->left->right->type = numberValue; newAst->left->right->value = 2;
	}
	// (c) If ast is cos x, return sin x
	else if ((ast->type == functionCos) && (ast->left->var != 0)) {
		newAst->type = functionSin;
		newAst->left = new ASTNode; newAst->left->type = variableChar; newAst->left->var = ast->left->var;
	}
	// (d) If ast is a non-zero number value, return that number * x
	else if ((ast->type == numberValue) && (ast->value != 0)) {
		newAst->type = operatorMul;
		newAst->left = new ASTNode; newAst->left->type = numberValue; newAst->left->value = ast->value;
		newAst->right = new ASTNode; newAst->right->type = variableChar; newAst->right->var = 'x';
	}

	// If ast is a zero number value, return zero
	else if ((ast->type == numberValue) && (ast->value == 0)) {
		newAst->type = numberValue;
		newAst->value = 0;
	}

	// If none of the above is done, return the original ast passed into this function
	else {
		return ast;
	}

	return newAst;
}

ASTNode* Integrator::integrate(ASTNode* t_ast) {
	ASTNode* ast = t_ast; ASTNode* solution = new ASTNode();

	// If ast represents the integral of a sum such as "1+2", return the integral of the evaluated sum "3"
	// If ast reprsents the integral of a sum such as "x^2 + x", return the sum of the integrals
	if (ast->type == operatorPlus) {
		try {
			Evaluator evaluator;
			double val = evaluator.evaluate(ast);
			ASTNode* ast2 = new ASTNode(); 
			ast2->type = numberValue; ast2->value = val;
			return integrate(ast2);
		}
		catch (EvaluatorException& exception) {
			solution->type = operatorPlus; 
			solution->value = 0;
			solution->var = 0;
			solution->left = integrate(ast->left);
			solution->right = integrate(ast->right);
			return solution;
		}
	}
	// If ast reprsents the integral of a sum such as "x^2 - x", return the sum of the integrals
	else if (ast->type == operatorMinus) {
		solution->type = operatorMinus;
		solution->value = 0;
		solution->var = 0;
		solution->left = integrate(ast->left);
		solution->right = integrate(ast->right);
		return solution;
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
		solution->type = operatorMul;
		solution->value = 0;
		solution->var = 0;
		solution->left = new ASTNode;
		solution->left->type = numberValue;
		solution->left->value = ast->left->value;
		solution->right = integrate(ast->right);
		return solution;
	}
	// If ast represents the integral of a product of n times x, return n times the integral of x
	else if (ast->type == operatorMul && (ast->right->value > 0)) {
		solution->type = operatorMul;
		solution->value = 0;
		solution->var = 0;
		solution->left = integrate(ast->left);
		solution->right = new ASTNode;
		solution->right->type = numberValue; solution->right->value = ast->right->value;
		return solution;
	}

	solution = lookInTable(ast);

	return solution;
}