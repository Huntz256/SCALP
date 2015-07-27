#include <iostream>
#include <vector>
#include "parser.h"

const bool OUTPUT_AST_TREE = false;
std::string astTypes[17] = { "UNDEF", "+", "-", "*", "/", "^", "-()", "NUM", "VAR", "sin()", "cos()", "tan()", "sec()", "csc()", "cot()", "log()", "ln()" };

// Outputs a representation of an AST tree to console
void outputAST(ASTNode* ast, int t_level) {
	int level = t_level + 1;
	std::cout << ast << ":[" << astTypes[ast->type] << "]-[" << ast->value << "]-[" << ast->var << "]-[" << ast->left << "]-[" << ast->right << "]\n";
	
	if (ast->left != NULL) {
		for (int i = 0; i < level; i++) { std::cout << " "; }
		outputAST(ast->left, level);
	}
	if (ast->right != NULL) {
		for (int i = 0; i < level; i++) { std::cout << " "; }
		outputAST(ast->right, level);
	}
}

// Helper function called by interpret() in order to identify functions within the input text
// Based off of the Parser class's getFunction() method
bool is3CharFunction(int i, char text[]){
	if (text[i] == 's'){
		if (text[i + 1] == 'i' && text[i + 2] == 'n') return true;
		else if (text[i + 1] == 'e' && text[i + 2] == 'c') return true;
		else return false;
	}
	else if (text[i] == 'c'){
		if (text[i + 1] == 's' && text[i + 2] == 'c') return true;
		else if (text[i + 1] == 'o'){
			if (text[i + 2] == 's') return true;
			else if (text[i + 2] == 't') return true;
			else return false;
		}
		else return false;
	}
	else if (text[i] == 't' && text[i + 1] == 'a' && text[i + 2] == 'n') return true;
	else if (text[i] == 'l' && text[i + 1] == 'o' && text[i + 2] == 'g') return true;
	else return false;
}

bool isLnFunction(int i, char text[]){
	return text[i] == 'l' && text[i + 1] == 'n';
}

// Simple interpreter (prototype solution)
// Currently takes care of implicit multiplication by adding in '*' where needed
// Example: turns 5(x+2) into 5*(x+2)
// Example2: turns 7x into 7*x
// Ignores the parentheses following a function such as sin(x)
void interpret(int size, char text[]){
	// Remove whitespace, shift stuff down to close the gap
	// And change everything to lowercase
	for (int i = 0; i < size; i++){
		if (isalpha(text[i])) text[i] = tolower(text[i]);
		else if (isspace(text[i])){
			for (int j = i; j < size; j++){
				text[j] = text[j + 1];
			}
		}
	}
	
	// Recalculate new size to account for removed whitespace
	size = 0;
	while (text[size] != 0) size++;

	// Add '*' where necessary
	for (int i = 1; i < size; i++){
		// Calls the helper function defined above to skip any function tokens and their opening parentheses
		if (is3CharFunction(i - 1, text)) i += 4;
		else if (isLnFunction(i - 1, text)) i += 3;
		// Check for familiar patters indicating implied multiplication
		else if (isdigit(text[i - 1]) && isalpha(text[i]) || // 5x -> 5*x
				isdigit(text[i - 1]) && text[i] == '(' || // 5(x) -> 5*(x)
				isalpha(text[i - 1]) && isdigit(text[i]) || // x5 -> x*5
				isalpha(text[i - 1]) && text[i] == '(' || // x(2) -> x*(2)
				isdigit(text[i - 1]) && is3CharFunction(i, text) || // 5sin(x) -> 5*sin(x)
				isdigit(text[i - 1]) && isLnFunction(i, text) || // 5ln(x) -> 5*ln(x)
				isalpha(text[i - 1]) && is3CharFunction(i, text) || // xsin(x) -> x*sin(x)
				isalpha(text[i - 1]) && isLnFunction(i, text) || // xln(x) -> x*ln(x)
				text[i - 1] == ')' && isdigit(text[i]) || // (x)5 = (x)*5
				text[i - 1] == ')' && isalpha(text[i])  // (2)x = (2)*x
				){ 
			// Iterate backwards to shift the entire string up so there's room to stick the '*' in
			for (int j = size + 1; j > i && j > 0; j--){
				text[j] = text[j - 1];
			}
			text[i] = '*'; // Stick the asterisk in there
			size++; // String got longer
		}
	}
}

// Tests if the expression text is a valid expression
// Outputs "VALID" to console if valid, outputs "INVALID: (exception message)" to console if invalid
void test(char input[]) {
	// Get input string size by incrementing up to the string termination character '\0'
	int size = 0;
	while (input[size] != 0) size++;

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
	interpret(size, text); // Directly modifies the text array to take out whitespace, add '*', etc.

	// Main try/catch block that processes each equation
	try {
		ast = parser.parse(text);
		if(OUTPUT_AST_TREE) outputAST(ast, 4);
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

	/*test("(5 ( x + 2))");
	test("(5*(x+2))");
	test("(7x)");
	test("(7*x)");
	test("(sin(x))");*/
	
	/*test("sin(5)"); test("sin(x)"); test("sin(x + 5)");
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
	test("x^2ln(x)");*/

	/*test("(x+2y)5");
	test("(x+2y)x");
	test("sin(x)5");
	test("sin(x)y");*/



	// Test whether the following are valid (they should not be)
	std::cout << "These should not be valid:\n\n";
	
	/*test("1 ++ 3");
	test(" *1 / 42.5");
	test("/52");
	test("42 ** 8");
	test("((1.26 + 8.99");
	test("A: heheh A: go left");
	test("x**5");
	test("x ^^ y");*/

	/*test("sin*(x)");
	test("sinch(x)");
	test("sinx");
	test("cost(x)");*/

	test("sin()");
	test("()");
	test("5()");

	// Pause the program
	std::cout << "Press enter to continue...";
	std::cin.get(); // More cross-platform solution than system("PAUSE") which looks nice but is Windows-only *cough* Hung
	return 0;
}