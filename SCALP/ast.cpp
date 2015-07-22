/*
* Implements the ASTNode class in ast.h
* See comments in ast.h for more details
*/

#include "ast.h"
#include <iostream>

//Constructor
ASTNode::ASTNode() {
	type = undefined;
	value = 0;
	left = NULL;
	right = NULL;
}

//Destructor
ASTNode::~ASTNode() {
	delete left;
	delete right;
}