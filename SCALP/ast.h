/*
* Declares a ASTNode class, which represents a node in an abstract syntax tree.
* Inner nodes represent operators and leafs are numerical values.
*/

// #define guard prevents multiple inclusion; follows Google style guard naming convention (<PROJECT>_<FILE>_H_)
#ifndef SCALP_AST_H_
#define SCALP_AST_H_

// Each node in an AST has a TYPE that describes what the node represents
enum ASTNodeType
{
	undefined,
	operatorPlus,
	operatorMinus,
	operatorMul,
	operatorDivision,
	operatorPower,
	unaryMinus,
	numberValue,
	variableChar,

	// All functions are unary except for Log, which also requires a base
	functionSin,
	functionCos,
	functionTan,
	functionSec,
	functionCsc,
	functionCot,
	functionLog,
	functionLn
};

// A single node in our AST can be represented as such:
//     [TYPE]-[VALUE]-[CHAR]-[LEFT]-[RIGHT]
// 
// A complete AST contains multiple nodes.
// Here is a representation of a AST representing the expression "x+6*3":
//             [+]-[]-[]-[LEFT]-[RIGHT]
//                         |       |
//         [var]-[]-[x]-[]-[]     [*]-[]-[]-[LEFT]-[RIGHT]
//                                         |       |
//                        [num]-[6]-[]-[]-[]       [num]-[3]-[]-[]-[]
class ASTNode
{
public:
	ASTNodeType type;
	double value;
	char var;
	ASTNode* left;
	ASTNode* right;

	ASTNode();
	~ASTNode();
};

#endif //SCALP_AST_H_