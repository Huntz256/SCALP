#include <iostream>
#include <vector>
#include "parser.h"

// Helper function called by interpret() in order to identify functions within the input text
// Based off of the Parser class's getFunction() method
void isFunction(int& i, char text[]){
	if (text[i-1] == 's'){
		if (text[i] == 'i' && text[i+1] == 'n') i += 4;
		else if (text[i] == 'e' && text[i+1] == 'c') i += 4;
	}
	else if (text[i-1] == 'c'){
		if (text[i] == 's' && text[i+1] == 'c') i += 4;
		else if (text[i] == 'o'){
			if (text[i+1] == 's') i += 4;
			else if (text[i+1] == 't') i += 4;
		}
	}
	else if (text[i-1] == 't' && text[i] == 'a' && text[i+1] == 'n') i += 4;
	else if (text[i-1] == 'l'){
		if (text[i] == 'n') i += 3;
		else if (text[i] == 'o' && text[i+1] == 'g') i += 4;
	}
}

// Simple interpreter (prototype solution)
// Currently takes care of implicit multiplication by adding in '*' where needed
// Example: turns 5(x+2) into 5*(x+2)
// Example2: turns 7x into 7*x
// Ignores the parentheses following a function such as sin(x)
void interpret(int size, char text[]){
	std::vector<int> flags; // For storing the indexes where '*' needs to be inserted

	// Remove whitespace
	for (int i = 0; i < size; i++){
		if (isspace(text[i])){
			for (int j = i; j < size; j++){
				text[j] = text[j + 1];
			}
		}
	}
	
	// Recalculate new size
	size = 0;
	while (text[size] != 0) size++;

	// Add '*' where necessary
	for (int i = 1; text[i] != 0; i++){
		isFunction(i, text);
		if (isdigit(text[i-1]) && isalpha(text[i]) ||
			isdigit(text[i-1]) && text[i] == '(' ||
			isalpha(text[i-1]) && isdigit(text[i]) ||
			isalpha(text[i-1]) && text[i] == '('){
			for (int j = size + 1; j > i && j > 0; j--){
				text[j] = text[j - 1];
			}
			text[i] = '*';
			size++;
		}
	}
}

// Tests if the expression text is a valid expression
// Outputs "VALID" to console if valid, outputs "INVALID: (exception message)" to console if invalid
void test(char input[]) {
	// Get input string size
	int size = 0;
	while (input[size] != 0) size++;

	std::cout << "\"" << input << "\"\n";
	std::cout << "Char length: " << size << std::endl;
	if (size > 42){
		std::cout << "Input exceeds character limit of 42. Cannot compute.";
		return;
	}

	Parser parser; ASTNode* ast = NULL; 
	char text[42];

	strcpy_s(text, input);
	interpret(size, text);

	try {
		ast = parser.parse(text);
		std::cout << "Input interpreted as: " << text << "\nResult: VALID" << "\n\n";
	}
	catch (const ParserException& e) {
		std::cout << "Input interpreted as: " << text << "\nResult: INVALID - " << e.what() << "\n\n";
	}
}

int main() {
	// Test whether the following are valid (they should be)
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

	/*test("5x");
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
	test("5 + y ^ 21 x");*/

	test("(5 ( x + 2))");
	test("(5*(x+2))");
	test("(7x)");
	test("(7*x)");
	test("(sin(x))");
	
	test("sin(5)"); test("sin(x)"); test("sin(x + 5)");
	test("cos(5)"); test("cos(x)"); test("cos(x^2+5)");
	test("tan(5)"); test("tan(x)"); test("tan(x+78)");
	test("sec(5)"); test("sec(x)"); test("sec(x+5y)");
	test("csc(5)"); test("csc(x)"); 
	test("csc(8z)");
	test("cot(5)"); test("cot(x)"); test("cot(x^(6+y))");
	test("ln(5)"); test("ln(x)"); test("ln(x * 7h)");

	// Test whether the following are valid (they should not be)
	/*std::cout << "These should not be valid:\n\n";
	test("1 ++ 3");
	test(" *1 / 42.5");
	test("/52");
	test("42 ** 8");
	test("((1.26 + 8.99");
	test("A: heheh A: go left");
	test("x**5");
	test("x ^^ y");*/

	// Pause the program
	std::cout << "Press enter to continue...";
	std::cin.get(); // More cross-platform solution than system("PAUSE") which looks nice but is Windows-only *cough* Hung
	return 0;
}