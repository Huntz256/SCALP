/*
 * Defines a Tester class that is used for testing the program.
 * The Tester class contains all the functions and test-suites that are used in main.cpp to test the current state of the project
 */

// #define guard prevents multiple inclusion; follows Google style guard naming convention (<PROJECT>_<FILE>_H_)
#ifndef SCALP_TESTER_H_
#define SCALP_TESTER_H_

#include "ast.h"
#include "integrator.h"
#include <vector>

class Tester {
public:
	void test(char input[]);
	void test1(char input[]);
	void outputAST(ASTNode* ast, int t_level);
	void outputDetailedAST(ASTNode* ast, int t_level);
	void outputGraphicalAST(ASTNode* ast);
	void generateGraphicalAST(std::vector<std::string>& nodes, ASTNode* ast, int t_level, bool leftNode, int t_maxIndent, int t_vecPos);

	// Test suites
	void testArithmetic();
	void testVariables();
	void testExponents();
	void testInterpreter();
	void testFunctions();
	void testLogs();
};

#endif // SCALP_TEST_H_