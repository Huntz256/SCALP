/*
 * Defines a Tester class that is used for testing the program.
 * The Tester class contains all the functions and test-suites that are used in main.cpp to test the current state of the project
 */

// #define guard prevents multiple inclusion; follows Google style guard naming convention (<PROJECT>_<FILE>_H_)
#ifndef SCALP_TESTER_H_
#define SCALP_TESTER_H_

#include "ast.h"

class Tester {
public:
	void test(char input[]);
	void outputAST(ASTNode* ast, int t_level);

	// Test suites
	void testArithmetic();
	void testVariables();
	void testExponents();
	void testInterpreter();
	void testFunctions();
	void testLogs();
};

#endif // SCALP_TEST_H_