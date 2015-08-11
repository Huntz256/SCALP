#include <iostream>
#include "tester.h"

int main() {
	// All implementations have been moved to other files in an attempt to keep main.cpp clean
	// The test function can still be called directly from here
	// Oh and FYI, the parser exception positions correspond to the interpreted equation, not the original input
	
	Tester tester;
	tester.testIntergationI();
	//tester.testLogs();
	//tester.testArithmetic();
	//tester.testVariables();
	//tester.testExponents();
	//tester.testInterpreter();
	//tester.testFunctions();

	/*std::cout << 
		"       [2]\n" <<
		"    [+]\n" << 
		"   |   [3]\n" << 
		"[*]\n" << 
		"   |   [5]\n" << 
		"    [+]\n" <<
		"       [7]\n" <<
		std::endl;*/
	
	// Pause the program; std::cin.get() is a more cross-platform solution than system("PAUSE"), which is Windows-only *cough* Hung
	std::cout << "Press enter to continue...";
	std::cin.get(); 

	return 0;
}