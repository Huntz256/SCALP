#include <iostream>
#include "parser.h"
#include "evaluator.h"

// Evaluates the expression given, outputs "INVALID: (exception message)" to console if invalid
void test(const char* text) {
	Parser parser; 
	
	try {
		ASTNode* ast = parser.parse(text);

		try {
			Evaluator eval;
			double value = eval.evaluate(ast);

			std::cout << text << " = " << value << "\n\n";
		}
		catch (EvaluatorException& exception) {
			std::cout << text << " t " << exception.what() << "\n\n";
		}

		delete ast;
	}
	catch (ParserException& exception1) {
		std::cout << "\"" << text << "\"" << "  INVALID: " << exception1.what() << "\n\n";
	}
}

int main() {

	//Attempt to evaluate the following valid expressions
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

	//Attempt to evaluate the following invalid expressions
	test("1 ++ 3");
	test(" *1 / 42.5");
	test("/52");
	test("42 ** 8");
	test("((1.26 + 8.99");
	test("MINH hahahaha");
	test("A: heheh A: go left");

	//Pause the program
	system("PAUSE");
	return 0;
}