/*
* Declares a ASTNode class, which represents a node in an abstract syntax tree.
* Inner nodes represent operators and leafs are numerical values.
*/

//#define guard prevents multiple inclusion
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
	unaryMinus,
	numberValue
};

// A single node in our AST can be represented as such:
//     [TYPE]-[VALUE]-[LEFT]-[RIGHT]
// 
// A complete AST contains multiple nodes.
// Here is a representation of a AST representing the expression "5+6*3":
//             [+]-[]-[LEFT]-[RIGHT]
//                      |         |
//        [num]-[5]-[]-[]         [*]-[]-[LEFT]-[RIGHT]
//                                         |       |
//                           [num]-[6]-[]-[]       [num]-[3]-[]-[]
class ASTNode
{
public:
	ASTNodeType type;
	double value;
	ASTNode* left;
	ASTNode* right;

	ASTNode();
	~ASTNode();
};

#endif //SCALP_AST_H_