#include <iostream>
#include "test.h"

int main() {
	// All implementations have been moved to other files in an attempt to keep main.cpp clean
	// The test function can still be called directly from here
	// Oh and FYI, the parser exception positions correspond to the interpreted equation, not the original input
	
	testLogs();
	
	// Pause the program
	std::cout << "Press enter to continue...";
	std::cin.get(); // More cross-platform solution than system("PAUSE") which looks nice but is Windows-only *cough* Hung

	return 0;
}