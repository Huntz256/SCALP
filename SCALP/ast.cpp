/*
* Implements the ASTNode class in ast.h
* See comments in ast.h for more details
*/

#include "ast.h"
#include <iostream>

// Constructor
ASTNode::ASTNode() {
	this->type = undefined;
	this->value = 0;
	this->var = 0;
	this->left = NULL;
	this->right = NULL;
}

// Destructor
ASTNode::~ASTNode() {
	delete left;
	delete right;
}