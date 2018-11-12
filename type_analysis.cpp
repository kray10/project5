#include "ast.hpp"
#include "symbol_table.hpp"

namespace LILC{

bool ASTNode::typeAnalysis(){
	throw std::runtime_error("Internal Error: \n"
		"We should never see this, as it\n"
		"is supposed to be overridden in any\n"
		"subclass at which it is encountered");
}

bool ProgramNode::typeAnalysis(){
	return myDeclList->typeAnalysis();
}

bool DeclListNode::typeAnalysis(){
	bool result = true;
	for (std::list<DeclNode *>::iterator
		it=myDecls->begin();
		it != myDecls->end(); ++it){
	    DeclNode * elt = *it;
	    result = result && it->typeAnalysis();
	}
	return result;
}

bool IdNode::typeAnalysis(){
	return true;
}

bool FormalsListNode::typeAnalysis(){
	bool result = true;
	for (std::list<FormalDeclNode *>::iterator
		it=myFormals->begin();
		it != myFormals->end(); ++it){
	    FormalDeclNode * elt = *it;
	    result = result && it->typeAnalysis();
	}
	return result;
}

bool ExpListNode::typeAnalysis(){
	bool result = true;
	for (std::list<ExpNode *>::iterator
		it=myExps->begin();
		it != myExps->end(); ++it){
	    ExpNode * elt = *it;
	    result = result && it->typeAnalysis();
	}
	return result;
}

bool StmtListNode::typeAnalysis(){
	bool result = true;
	for (std::list<StmtNode *>::iterator
		it=myEmyStmtsxps->begin();
		it != myStmts->end(); ++it){
	    StmtNode * elt = *it;
	    result = result && it->typeAnalysis();
	}
	return result;
}

bool FnBodyNode::typeAnalysis(){
	return (myDeclList->typeAnalysis() && myStmtList->typeAnalysis());
}

bool FnDeclNode::typeAnalysis(){
	return (myRetType->typeAnalysis()
					&& myId->typeAnalysis()
					&& myFormals->typeAnalysis()
					&& myBody->nameAnalysis());
}

bool FormalDeclNode::typeAnalysis(){
	return myRetType->typeAnalysis();
}

bool StructDeclNode::typeAnalysis(){
	return myDeclList->typeAnalysis();
}



/*
* Creates a comma-separated string listing the types of formals.
* This function mostly serves as a helper for
* FnDeclNode::getTypeString() in building a function
* type signature for unparsing.
*/
std::string FormalsListNode::getTypeString(){
	std::string res = "";
	bool first = true;
	for (FormalDeclNode * decl : *myFormals){
		if (first){ first = false; }
		else { res += ","; }
		res += decl->getTypeString();
	}
	return res;
}

/*
* Create a string representing the type signature of a
* function.  This function is used for building a function
* type signature for unparsing.
*/
std::string FnDeclNode::getTypeString(){
	return myFormals->getTypeString()
		+ "->"
		+ myRetType->getTypeString();
}

/*
* The type of a declaration is that of the
* type it declares
*/
std::string FormalDeclNode::getTypeString(){
	return myType->getTypeString();
}

} // End namespace LILC

