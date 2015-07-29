/*
* Implements the Tester class in test.h
*/

#include "interpreter.h"
#include "parser.h"
#include "tester.h"
#include <iostream>

const bool OUTPUT_AST_TREE = true;
std::string astTypes[17] = { "UNDEF", "+", "-", "*", "/", "^", "-()", "NUM", "VAR", "sin()", "cos()", "tan()", "sec()", "csc()", "cot()", "log()", "ln()" };
Interpreter interpreter;

// Outputs a representation of an AST tree to console
void Tester::outputDetailedAST(ASTNode* ast, int t_level) {
	int level = t_level + 1;
	std::cout << ast << ":[" << astTypes[ast->type] << "]-[" << ast->value << "]-[" << ast->var << "]-[" << ast->left << "]-[" << ast->right << "]\n";

	if (ast->left != NULL) {
		for (int i = 0; i < level; i++) { std::cout << " "; }
		outputDetailedAST(ast->left, level);
	}
	if (ast->right != NULL) {
		for (int i = 0; i < level; i++) { std::cout << " "; }
		outputDetailedAST(ast->right, level);
	}
}

// Outputs a representation of an AST tree to console
void Tester::outputAST(ASTNode* ast, int t_level) {
	int level = t_level + 1;
	if (astTypes[ast->type] == "NUM") {
		std::cout << " [" << ast->value << "]\n";
	}
	else if (astTypes[ast->type] == "VAR") {
		std::cout << " [" << ast->var << "]\n";
	}
	else {
		std::cout << " [" << astTypes[ast->type] << "]\n";
	}

	if (ast->left != NULL) {
		for (int i = 0; i < level; i++) { std::cout << "  "; }
		//std::cout << "L: ";
		outputAST(ast->left, level);
	}
	if (ast->right != NULL) {
		for (int i = 0; i < level; i++) { std::cout << "  "; }
		//std::cout << "R: ";
		outputAST(ast->right, level);
	}
}

// Tests if the expression text is a valid expression
// Outputs "VALID" to console if valid, outputs "INVALID: (exception message)" to console if invalid
void Tester::test(char input[]) {
	int size = interpreter.getSize(input);

	std::cout << "\"" << input << "\"\n"; // Prints out the original input string
	std::cout << "Char length: " << size << std::endl;
	if (size > 42){
		std::cout << "Input exceeds character limit of 42. Cannot compute.";
		return;
	}

	Parser parser;
	ASTNode* ast = NULL; // It's good practice to always initialize pointers to NULL (or so folks on the internet say)
	char text[42]; // Picked 42 as an arbitrary number; could be extended later on to accomodate longer equations

	strcpy_s(text, input); // Copies the input char array into an explicitly defined one so that it can be modified
	interpreter.interpret(text); // Directly modifies the text array to take out whitespace, add '*', etc.

	// Main try/catch block that processes each equation
	try {
		ast = parser.parse(text);
		if (OUTPUT_AST_TREE) { 
			std::cout << "AST Tree:\n";
			outputAST(ast, 0); 
			std::cout << "";
		}
		std::cout << "Input interpreted as: " << text << "\nResult: VALID" << "\n\n";
	}
	catch (const ParserException& e) {
		std::cout << "Input interpreted as: " << text << "\nResult: INVALID - " << e.what() << "\n\n";
	}
}

////////////// TEST SUITES ////////////////
void Tester::testArithmetic(){
	std::cout << "These should be valid:\n\n";

	test("1+1");
	test("1 + 2 + 3 + 5");
	test("1 * 2 * 3 * 5");
	test("1 - 2 - 3 - 5");
	test("1 / 2 / 3 / 5");
	test("8.99 * 10 + 8.85 * 1.60");
	test("(1.67 + 9.11) * (1.26 + 1.67)");
	test("(1.26) + 8.99 - 67789");
	test("-300 + (-3.0554) * 141292");
	test("256256256256");

	std::cout << "These should not be valid:\n\n";

	test("1 ++ 3");
	test(" *1 / 42.5");
	test("/52");
	test("42 ** 8");
	test("((1.26 + 8.99");
	test("A: heheh A: go left");
	test("x**5");
}

void Tester::testVariables(){
	std::cout << "These should be valid:\n\n";

	test("x+1");
	test("1 + h + 3 + 5");
	test("F * K * b * 5");
	test("a - 2 - 3 - 5");
	test("1 / k / D / 5");
	test("8.99 * g + k * x");
	test("(y + 9.11) * (1.26 + 1.67)");
	test("(n) + 8.99 - 67789");
	test("-r + (-f) * 141292");
	test("5x");
	test("5Xy");
	test("12x5");
	test("x(x+2)");
	test("x/(5+H)");
	test("24 + x");
	test("25 x");

	std::cout << "These should not be valid:\n\n";

}

void Tester::testExponents(){
	std::cout << "These should be valid:\n\n";

	test("2^8");
	test("2 ^ 3");
	test("x^5");
	test("x ^ y");
	test("5 + y ^ 21 x");

	std::cout << "These should not be valid:\n\n";

	test("x ^^ y");
}

void Tester::testInterpreter(){
	std::cout << "These should be valid:\n\n";

	test("(5 ( x + 2))");
	test("(5*(x+2))");
	test("(7x)");
	test("(7*x)");
	test("(sin(x))");
	test("(x+2y)5");
	test("(x+2y)x");
	test("sin(x)5");
	test("sin(x)y");
	
	std::cout << "These should not be valid:\n\n";

}

void Tester::testFunctions(){
	std::cout << "These should be valid:\n\n";

	test("sin(5)"); test("sin(x)"); test("sin(x + 5)");
	test("cos(5)"); test("cos(x)"); test("cos(x^2+5)");
	test("tan(5)"); test("tan(x)"); test("tan(x+78)");
	test("sec(5)"); test("sec(x)"); test("sec(x+5y)");
	test("csc(5)"); test("csc(x)");
	test("csc(8z)");
	test("cot(5)"); test("cot(x)"); test("cot(x^(6+y))");
	test("ln(5)"); test("ln(x)"); test("ln(x * 7h)");
	test("Sin(x)");
	test("xsec(x)");
	test("(xsec(x))");
	test("5sin(x)");
	test("x^2ln(x)");

	std::cout << "These should not be valid:\n\n";

	test("sin*(x)");
	test("sinch(x)");
	test("sinx");
	test("cost(x)");
}

void Tester::testLogs(){
	std::cout << "These should be valid:\n\n";

	test("log(5)");
	test("log(x)");
	test("log(x^2+5)");
	test("log(5, x)");
	test("xlog(9, x^2y)");

	std::cout << "These should not be valid:\n\n";

	test("log()");
	test("logu()");
	test("log(");
	test("log(5");
	test("log(5, ");
	test("log(5,)");
	test("log(x,y)");
	test("log(x,5)");
}