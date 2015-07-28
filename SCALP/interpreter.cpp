#include "interpreter.h"
#include <iostream>

// Helper function called by interpret() in order to identify functions within the input text
// Based off of the Parser class's getFunction() method
bool Interpreter::is3CharFunction(int i, char text[]){
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

bool Interpreter::isLnFunction(int i, char text[]){
	return text[i] == 'l' && text[i + 1] == 'n';
}

// Simple interpreter (prototype solution)
// Currently takes care of implicit multiplication by adding in '*' where needed
// Example: turns 5(x+2) into 5*(x+2)
// Example2: turns 7x into 7*x
// Ignores the parentheses following a function such as sin(x)
void Interpreter::interpret(char text[]){
	int size = getSize(text);

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

// Parameter must be a string-formatted character array (ie. one that ends with a '\0' termination character)
int Interpreter::getSize(char input[]){
	// Get input string size by incrementing up to the string termination character '\0'
	int size = 0;
	while (input[size] != 0) size++;
	return size;
}