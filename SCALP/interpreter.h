/* 
 * Defines a group of functions that makes the raw input equation more manageable for the parser
 * Originally defined in main.cpp but moved to separate file to keep main.cpp cleaner
*/

// #define guard prevents multiple inclusion; follows Google style guard naming convention (<PROJECT>_<FILE>_H_)
#ifndef SCALP_INTERPRETER_H_
#define SCALP_INTERPRETER_H_

// Using a namespace to group the functions
// Simply for organizational purposes in case the codebase gets too large
namespace Interpreter {
	void interpret(char text[]); // Turns the text into something more manageable by the parser

	// Helper functions called by interpret()
	// See implementation for more details
	bool is3CharFunction(int i, char text[]);
	bool isLnFunction(int i, char text[]);
	int getSize(char text[]);
}

#endif // SCALP_INTERPRETER_H_