#include "ast.hpp"
#include "symbol_table.hpp"

namespace LILC{

void ProgramNode::unparse(std::ostream& out, int indent){
	myDeclList->unparse(out, indent);
}

void DeclListNode::unparse(std::ostream& out, int indent){
	for (std::list<DeclNode *>::iterator
		it=myDecls->begin();
		it != myDecls->end(); ++it){
	    DeclNode * elt = *it;
	    elt->unparse(out, indent);
	}
}

void FormalsListNode::unparse(std::ostream& out, int indent){
	bool first = true;
	for (FormalDeclNode * formal : *myFormals){
		if (first){ first = false; }
		else { out << ", "; }
		formal->unparse(out, indent);
	}
}

void FnBodyNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "\n{\n";
	myDeclList->unparse(out, indent+4);
	myStmtList->unparse(out,indent+4);
	out << "}\n";
}

void ExpListNode::unparse(std::ostream& out, int indent){
	bool first = true;
	for (std::list<ExpNode *>::iterator it=myExps->begin();
		it != myExps->end(); ++it){
	    ExpNode * elt = *it;
			if (first) { first = false; }
			else { out << ","; }
	    elt->unparse(out, indent);
	}
}

void StmtListNode::unparse(std::ostream& out, int indent){
	for (std::list<StmtNode *>::iterator it=myStmts->begin();
		it != myStmts->end(); ++it){
	    StmtNode * elt = *it;
	    elt->unparse(out, indent);
	}
}

void StmtNode::unparse(std::ostream& out, int indent){
}

void VarDeclNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myType->unparse(out, 0);
	out << " ";
	out << myDeclaredID->getString();
	out << ";\n";
}

void FnDeclNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myRetType->unparse(out, 0);
	out << " ";
	out << myId->getString();
	out << "(";
	myFormals->unparse(out, 0);
	out << ")";
	myBody->unparse(out, 0);
}

void FormalDeclNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myType->unparse(out, 0);
	out << " " << myDeclaredID->getString();
}

void StructDeclNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "struct ";
	out << myDeclaredID->getString();
	out << "\n{\n";
	myDeclList->unparse(out,indent+4);
	out << "};\n";
}

void AssignStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myAssign->unparse(out,0);
	out << ";\n";
}

void PostIncStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp->unparse(out,0);
	out << "++;\n";
}

void PostDecStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp->unparse(out,0);
	out << "--;\n";
}

void ReadStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "output >> ";
	myExp->unparse(out,0);
	out << ";\n";
}

void WriteStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "input << ";
	myExp->unparse(out,0);
	out << ";\n";
}

void IfStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "if(";
	myExp->unparse(out,0);
	out << ") {\n";
	myDecls->unparse(out,indent+4);
	myStmts->unparse(out,indent+4);
	doIndent(out, indent);
	out << "}\n";
}

void IfElseStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "if(";
	myExp->unparse(out,0);
	out << ") {\n";
	myDeclsT->unparse(out,indent+4);
	myStmtsT->unparse(out,indent+4);
	doIndent(out, indent);
	out << "}\n";
	doIndent(out, indent);
	out << "else {\n";
	myDeclsF->unparse(out,indent+4);
	myStmtsF->unparse(out,indent+4);
	doIndent(out, indent);
	out << "}\n";
}

void WhileStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "while(";
	myExp->unparse(out,0);
	out << ") {\n";
	myDecls->unparse(out,indent+4);
	myStmts->unparse(out,indent+4);
	doIndent(out, indent);
	out << "}\n";
}

void CallStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myCallExp->unparse(out,0);
	out << ";\n";
}

void ReturnStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "return ";
	if(myExp != nullptr) {
		myExp->unparse(out,0);
	}
	out << ";\n";
}

void IdNode::unparse(std::ostream& out, int indent){
	out << myStrVal;
	if(mySymbol != nullptr) {
		out << "(" << mySymbol->getTypeString() << ")";
	} else {
		throw std::runtime_error("IdNode has no symbol");
		// error!
	}
}

void IntNode::unparse(std::ostream& out, int indent){
	out << "int";
}

void BoolNode::unparse(std::ostream& out, int indent){
	out << "bool";
}

void VoidNode::unparse(std::ostream& out, int indent){
	out << "void";
}

void StructNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "struct " << this->myId->getString();
}

void IntLitNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << myInt;
}

void StrLitNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << myString;
}

void TrueNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "true";
}

void FalseNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "false";
}

void DotAccessNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp->unparse(out, 0);
	out << ".";
	myId->unparse(out, 0);
}

void AssignNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExpLHS->unparse(out, 0);
	out << " = ";
	myExpRHS->unparse(out, 0);
}

void CallExpNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myId->unparse(out, 0);
	out << "(";
	myExpList->unparse(out, 0);
	out << ")";
}

void UnaryMinusNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	out << "-";
	myExp->unparse(out, 0);
	out << ")";
}

void NotNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	out << "!";
	myExp->unparse(out, 0);
	out << ")";
}

void BinaryExpNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	myExp1->unparse(out, 0);
	out << myOp();
	myExp2->unparse(out, 0);
	out << ")";
}


} // End namespace LIL' C
