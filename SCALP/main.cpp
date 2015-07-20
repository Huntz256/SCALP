#include <iostream>
#include "parser.h"

// Tests if the expression text is a valid expression
// Outputs "VALID" to console if valid, outputs "INVALID" to console if invalid
void test(const char* text) {
	Parser parser;
	try {
		parser.parse(text);
		std::cout << "\"" << text << "\"" << "  VALID" << "\n\n";
	}
	catch (char* exception) {
		std::cout << "\"" << text << "\"" << "  INVALID" << "\n\n";
	}
}

int main() {

	//Test whether the following are valid (they should be)
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


	//Test whether the following are valid (they should not be)
	test("1 ++ 3");
	test(" *1 / 42.5");
	test("42 ** 8");
	test("((1.26 + 8.99");
	test("MINH hahahaha");
	test("A: heheh A: go left");

	//Pause the program
	int x; std::cin >> x;
	return 0;
}