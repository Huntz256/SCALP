/*
* Declares an Integrator class, which is the main class that handles the actual integration process.
* Its purpose is to take in a AST that represents something to be integrated, such as the integral of 2x, and
* to output a symbolic result, such as x^2. It will require help from other classes in order to do this, but it is 
* the "guy in charge."
*/

//#define guard prevents multiple inclusion
#ifndef SCALP_INTEGRATOR_H_
#define SCALP_INTEGRATOR_H_

#include "ast.h"
#include <string>

class Integrator
{
	ASTNode* applySafeTransform(ASTNode* t_ast);
	ASTNode* applyHeuristicTransform(ASTNode* t_ast);
	std::string lookInTable(ASTNode* t_ast);
public:
	std::string integrate(ASTNode* t_ast);
};

#endif //SCALP_INTEGRATOR_H_