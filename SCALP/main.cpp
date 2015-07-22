#include <iostream>
#include "parser.h"

// Tests if the expression text is a valid expression
// Outputs "VALID" to console if valid, outputs "INVALID: (exception message)" to console if invalid
void test(const char* text) {
	Parser parser; ASTNode* ast;
	try {
		ast = parser.parse(text);
		std::cout << "\"" << text << "\"" << "  VALID" << "\n\n";
	}
	catch (const ParserException& e) {
		std::cout << "\"" << text << "\"" << "  INVALID: " << e.what() << "\n\n";
	}
}

int main() {

	//Test whether the following are valid (they should be)
	std::cout << "These should be valid:\n\n";
	/*test("1+1");
	test("1 + 2 + 3 + 5");
	test("1 * 2 * 3 * 5");
	test("1 - 2 - 3 - 5");
	test("1 / 2 / 3 / 5");
	test("8.99 * 10 + 8.85 * 1.60");
	test("(1.67 + 9.11) * (1.26 + 1.67)");
	test("(1.26) + 8.99 - 67789");
	test("-300 + (-3.0554) * 141292");
	test("256256256256");

	test("x+1");
	test("1 + h + 3 + 5");
	test("F * K * b * 5");
	test("a - 2 - 3 - 5");
	test("1 / k / D / 5");
	test("8.99 * g + k * x");
	test("(y + 9.11) * (1.26 + 1.67)");
	test("(n) + 8.99 - 67789");
	test("-r + (-f) * 141292");*/

	test("5x");
	test("5Xy");
	test("12x5");
	test("x(x+2)");
	test("x/(5+H)");
	test("24 + x");
	test("25 x");

	test("2^8");
	test("2 ^ 3");
	test("x^5");
	test("x ^ y");
	test("5 + y ^ 21 x");

	//Test whether the following are valid (they should not be)
	std::cout << "These should not be valid:\n\n";
	test("1 ++ 3");
	test(" *1 / 42.5");
	test("/52");
	test("42 ** 8");
	test("((1.26 + 8.99");
	test("A: heheh A: go left");
	test("x**5");
	test("x ^^ y");

	//Pause the program
	system("PAUSE");
	return 0;
}