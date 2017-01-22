/* 
 * Defines an Interpretor class which helps the Parser out. 
 * The Interpretor contains a group of functions that makes the raw input equation more manageable for the parser
*/

// #define guard prevents multiple inclusion; follows Google style guard naming convention (<PROJECT>_<FILE>_H_)
#ifndef SCALP_INTERPRETER_H_
#define SCALP_INTERPRETER_H_

class Interpreter 
{
public:
	// Helper functions called by interpret()
	// See implementation for more details
	bool is3CharFunction(int i, char text[]);
	bool isLnFunction(int i, char text[]);
	int getSize(char text[]);

	// Turns the text into something more manageable by the parser
	void interpret(char text[]);
};

#endif // SCALP_INTERPRETER_H_