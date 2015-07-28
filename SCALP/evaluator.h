/*
* Declares a Evaluator class, which takes in a AST and splits out the evaluated form of the expression
* that the AST represents.
*/

//#define guard prevents multiple inclusion
#ifndef SCALP_EVALUATOR_H_
#define SCALP_EVALUATOR_H_

#include "ast.h"
#include <iostream>

class Evaluator
{
	double evaluateSubtree(ASTNode* ast);
public:
	double evaluate(ASTNode* ast);
};

class EvaluatorException : public std::exception
{
public:
	EvaluatorException(const std::string& message);
};

#endif //SCALP_EVALUATOR_H_