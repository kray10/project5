#include "ast.hpp"
#include "symbol_table.hpp"
#include "err.hpp"

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
	    result = result && elt->typeAnalysis();
	}
	return result;
}

bool FormalsListNode::typeAnalysis(){
	bool result = true;
	for (std::list<FormalDeclNode *>::iterator
		it=myFormals->begin();
		it != myFormals->end(); ++it){
	    FormalDeclNode * elt = *it;
	    result = elt->typeAnalysis() && result;
	}
	return result;
}

bool ExpListNode::checkTypes(std::list<std::string*>* expectedTypes) {
	if (expectedTypes->size() != myExps->size()) {
		return false;
	}
	std::list<string*>::iterator it1 = expectedTypes->begin();
	std::list<ExpNode*>::iterator it2 = myExps->begin();
	bool result = true;
	for (; it1 != expectedTypes->end() && it2 != myExps->end();
				++it1, ++ it2) {
		std::string* eType = *it1;
		ExpNode* elt = *it2;
		if (*eType != elt->getType()) {
			result = false;
			Err::actualNotMatchingFormal(elt->getAssignPos());
		}
	}
	return result;
}

bool ExpListNode::typeAnalysis(){
	bool result = true;
	for (std::list<ExpNode *>::iterator
		it=myExps->begin();
		it != myExps->end(); ++it){
	    ExpNode * elt = *it;
	    result = elt->typeAnalysis() && result;
	}
	return result;
}

bool StmtListNode::typeAnalysis(){
	bool result = true;
	for (std::list<StmtNode *>::iterator
		it=myStmts->begin();
		it != myStmts->end(); ++it){
	    StmtNode * elt = *it;
	    result = elt->typeAnalysis() && result;
	}
	return result;
}

bool FnBodyNode::typeAnalysis(){
	bool result = myDeclList->typeAnalysis();
	result = myStmtList->typeAnalysis() && result;
	return result;
}

bool FnDeclNode::typeAnalysis(){
	bool result = myRetType->typeAnalysis();
	result = myId->typeAnalysis() && result;
	result = myFormals->typeAnalysis() && result;
	result = myBody->checkReturnTypes(myRetType->getTypeString()) && result;
	return result;
}

bool FormalDeclNode::typeAnalysis(){
	return myType->typeAnalysis();
}

bool StructDeclNode::typeAnalysis(){
	return myDeclList->typeAnalysis();
}

bool AssignNode::typeAnalysis(){
	bool result = myExpLHS->typeAnalysis();
	result = myExpRHS->typeAnalysis() && result;
	std::string typeL = myExpLHS->getType();
	std::string typeR = myExpRHS->getType();

	if (typeL != "" && typeR != "") {
		if (typeL != typeR) {
			Err::typeMismatch(myExpLHS->getAssignPos());
			result = false;
		} else {
			if (typeR.find("->") != std::string::npos) {
				Err::assignFunc(myExpLHS->getAssignPos());
				result = false;
			} else if (typeR == "struct") {
				Err::assignStructName(myExpLHS->getAssignPos());
				result = false;
			} else if (typeR != "int" && typeR != "bool") {
				Err::assignStructVar(myExpLHS->getAssignPos());
				result = false;
			} else {
				myType = typeR;
			}
		}
	}
	return result;
}

bool CallExpNode::typeAnalysis() {
	bool result = myExpList->typeAnalysis();
	std::string expType = myId->getType();

	if (expType == "") {
		return false;
	}

	if (expType.find("->") == std::string::npos) {
		Err::callNonFunction(getAssignPos());
		return false;
	}

	std::list<std::string*>* typeList = new std::list<std::string*>();
	std::string* type = new std::string("");
	bool reachedMyTpe = false;
	for (char c : expType) {
		if (!reachedMyTpe) {
			if (c == '-') {
				typeList->push_back(type);
				type = new std::string("");
			} else if (c == ',') {
				typeList->push_back(type);
				type = new std::string("");
			} else if (c == '>') {
				reachedMyTpe = true;
			} else {
				*type += c;
			}
		} else {
			*type += c;
		}
	}
	myType = *type;

	if (myExpList->listSize() != typeList->size()) {
		Err::callFunctionWrongNumArgs(getAssignPos());
		return false;
	}

	result = myExpList->checkTypes(typeList) && result;

	// need to check that each expression is of the right type
	return result;
}

bool UnaryMinusNode::typeAnalysis() {
	bool result = myExp->typeAnalysis();
	myType = "int";
	if (myExp->getType() != "int" && myExp->getType() != "") {
		Err::nonArthimetic(myExp->getAssignPos());
		result = false;
		myType = "";
	}
	return result;
}

bool NotNode::typeAnalysis() {
	bool result = myExp->typeAnalysis();
	myType = "bool";
	if (myExp->getType() != "bool" && myExp->getType() != "") {
		Err::nonLogical(myExp->getAssignPos());
		result = false;
		myType = "";
	}
	return result;
}



bool PlusNode::typeAnalysis(){
	bool result = myExp1->typeAnalysis();
	result = myExp2->typeAnalysis() && result;
	myType = "int";
	if(myExp1->getType() != "int" && myExp1->getType() != "")
	{
		Err::nonArthimetic(myExp1->getAssignPos());
		result = false;
		myType = "";
	}
	if(myExp2->getType() != "int" && myExp2->getType() != "")
	{
		Err::nonArthimetic(myExp2->getAssignPos());
		result = false;
		myType = "";
	}
	return result;
}

bool MinusNode::typeAnalysis(){
	bool result = myExp1->typeAnalysis();
	result = myExp2->typeAnalysis() && result;
	myType = "int";
	if(myExp1->getType() != "int" && myExp1->getType() != "")
	{
		Err::nonArthimetic(myExp1->getAssignPos());
		result = false;
		myType = "";
	}
	if(myExp2->getType() != "int" && myExp2->getType() != "")
	{
		Err::nonArthimetic(myExp2->getAssignPos());
		result = false;
		myType = "";
	}
	return result;
}

bool TimesNode::typeAnalysis(){
	bool result = myExp1->typeAnalysis();
	result = myExp2->typeAnalysis() && result;
	myType = "int";
	if(myExp1->getType() != "int" && myExp1->getType() != "")
	{
		Err::nonArthimetic(myExp1->getAssignPos());
		result = false;
		myType = "";
	}
	if(myExp2->getType() != "int" && myExp2->getType() != "")
	{
		Err::nonArthimetic(myExp2->getAssignPos());
		result = false;
		myType = "";
	}
	return result;
}
bool DivideNode::typeAnalysis(){
	bool result = myExp1->typeAnalysis();
	result = myExp2->typeAnalysis() && result;
	myType = "int";
	if(myExp1->getType() != "int" && myExp1->getType() != "")
	{
		Err::nonArthimetic(myExp1->getAssignPos());
		result = false;
		myType = "";
	}
	if(myExp2->getType() != "int" && myExp2->getType() != "")
	{
		Err::nonArthimetic(myExp2->getAssignPos());
		result = false;
		myType = "";
	}
	return result;
}

bool AndNode::typeAnalysis(){
	bool result = myExp1->typeAnalysis();
	result = myExp2->typeAnalysis() && result;
	myType = "bool";
	if(myExp1->getType() != "bool" && myExp1->getType() != "")
	{
		Err::nonLogical(myExp1->getAssignPos());
		result = false;
		myType = "";
	}
	if(myExp2->getType() != "bool" && myExp2->getType() != "")
	{
		Err::nonLogical(myExp2->getAssignPos());
		result = false;
		myType = "";
	}
	return result;
}

bool OrNode::typeAnalysis(){
	bool result = myExp1->typeAnalysis();
	result = myExp2->typeAnalysis() && result;
	myType = "bool";
	if(myExp1->getType() != "bool" && myExp1->getType() != "")
	{
		Err::nonLogical(myExp1->getAssignPos());
		result = false;
		myType = "";
	}
	if(myExp2->getType() != "bool" && myExp2->getType() != "")
	{
		Err::nonLogical(myExp2->getAssignPos());
		result = false;
		myType = "";
	}
	return result;
}

bool EqualsNode::typeAnalysis() {
	bool result = myExp1->typeAnalysis();
	result = myExp2->typeAnalysis() && result;
	std::string type1 = myExp1->getType();
	std::string type2 = myExp2->getType();
	if (type1 != "" && type2 != "") {
		myType = "bool";
		if (type1 != type2) {
			Err::typeMismatch(myExp1->getAssignPos());
			result = false;
			myType = "";
		} else {
			if (type1 == "void") {
				Err::equalOnVoid(myExp1->getAssignPos());
				result = false;
				myType = "";
			} else if (type1.find("->") != std::string::npos) {
				Err::equalOnFunction(myExp1->getAssignPos());
				result = false;
				myType = "";
			} else if (type1 == "struct") {
				Err::equalOnStructName(myExp1->getAssignPos());
				result = false;
				myType = "";
			} else if (type1 != "int" && type1 != "bool") {
				Err::equalOnStructVar(myExp1->getAssignPos());
				result = false;
				myType = "";
			}
		}
	}
	return result;
}

bool NotEqualsNode::typeAnalysis() {
	bool result = myExp1->typeAnalysis();
	result = myExp2->typeAnalysis() && result;
	std::string type1 = myExp1->getType();
	std::string type2 = myExp2->getType();
	if (type1 != "" && type2 != "") {
		myType = "bool";
		if (type1 != type2) {
			Err::typeMismatch(myExp1->getAssignPos());
			result = false;
			myType = "";
		} else {
			if (type1 == "void") {
				Err::equalOnVoid(myExp1->getAssignPos());
				result = false;
				myType = "";
			} else if (type1.find("->") != std::string::npos) {
				Err::equalOnFunction(myExp1->getAssignPos());
				result = false;
				myType = "";
			} else if (type1 == "struct") {
				Err::equalOnStructName(myExp1->getAssignPos());
				result = false;
				myType = "";
			} else if (type1 != "int" && type1 != "bool") {
				Err::equalOnStructVar(myExp1->getAssignPos());
				result = false;
				myType = "";
			}
		}
	}
	return result;
}

bool LessNode::typeAnalysis(){
	bool result = myExp1->typeAnalysis();
	result = myExp2->typeAnalysis() && result;
	myType = "bool";
	if(myExp1->getType() != "int" && myExp1->getType() != "")
	{
		Err::nonRelational(myExp1->getAssignPos());
		result = false;
		myType = "";
	}
	if(myExp2->getType() != "int" && myExp2->getType() != "")
	{
		Err::nonRelational(myExp2->getAssignPos());
		result = false;
		myType = "";
	}
	return result;
}

bool GreaterNode::typeAnalysis(){
	bool result = myExp1->typeAnalysis();
	result = myExp2->typeAnalysis() && result;
	myType = "bool";
	if(myExp1->getType() != "int" && myExp1->getType() != "")
	{
		Err::nonRelational(myExp1->getAssignPos());
		result = false;
		myType = "";
	}
	if(myExp2->getType() != "int" && myExp2->getType() != "")
	{
		Err::nonRelational(myExp2->getAssignPos());
		result = false;
		myType = "";
	}
	return result;
}

bool LessEqNode::typeAnalysis(){
	bool result = myExp1->typeAnalysis();
	result = myExp2->typeAnalysis() && result;
	myType = "bool";
	if(myExp1->getType() != "int" && myExp1->getType() != "")
	{
		Err::nonRelational(myExp1->getAssignPos());
		result = false;
		myType = "";
	}
	if(myExp2->getType() != "int" && myExp2->getType() != "")
	{
		Err::nonRelational(myExp2->getAssignPos());
		result = false;
		myType = "";
	}
	return result;
}

bool GreaterEqNode::typeAnalysis(){
	bool result = myExp1->typeAnalysis();
	result = myExp2->typeAnalysis() && result;
	myType = "bool";
	if(myExp1->getType() != "int" && myExp1->getType() != "")
	{
		Err::nonRelational(myExp1->getAssignPos());
		result = false;
		myType = "";
	}
	if(myExp2->getType() != "int" && myExp2->getType() != "")
	{
		Err::nonRelational(myExp2->getAssignPos());
		result = false;
		myType = "";
	}
	return result;
}

bool PostIncStmtNode::typeAnalysis(){
	bool result = myExp->typeAnalysis();
	if (myExp->getType() != "int" && myExp->getType() != "") {
		Err::nonArthimetic(getAssignPos());
		result = false;
	}
	return false;
}

bool PostDecStmtNode::typeAnalysis(){
	bool result = myExp->typeAnalysis();
	if (myExp->getType() != "int" && myExp->getType() != "") {
		Err::nonArthimetic(getAssignPos());
		result = false;
	}
	return false;
}

bool ReadStmtNode::typeAnalysis() {
	bool result = myExp->typeAnalysis();
	std::string expType = myExp->getType();
	if (expType != "int" && expType != "bool" && expType != "") {
		if (expType.find("->") != std::string::npos) {
			Err::readFunction(myExp->getAssignPos());
			result = false;
		} else if (expType == "struct") {
			Err::readStructName(myExp->getAssignPos());
			result = false;
		} else {
			Err::readStructVar(myExp->getAssignPos());
			result = false;
		}
	}
	return result;
}

bool WriteStmtNode::typeAnalysis() {
	bool result = myExp->typeAnalysis();
	std::string expType = myExp->getType();
	if (expType != "int" && expType != "string" && expType != "bool" && expType != "") {
		if (expType.find("->") != std::string::npos) {
			Err::writeFunction(myExp->getAssignPos());
			result = false;
		} else if (expType == "struct") {
			Err::writeStructName(myExp->getAssignPos());
			result = false;
		} else if (expType == "void") {
			Err::writeVoidResult(myExp->getAssignPos());
			result = false;
		} else {
			Err::writeStructVar(myExp->getAssignPos());
			result = false;
		}
	}
	return result;
}

bool IfStmtNode::typeAnalysis() {
	bool result = myExp->typeAnalysis();
	result = myDecls->typeAnalysis() && result;
	result = myStmts->typeAnalysis() && result;
	if (myExp->getType() != "bool" && myExp->getType() != "") {
		Err::nonBoolInIf(myExp->getAssignPos());
		result = false;
	}
	return result;
}

bool IfElseStmtNode::typeAnalysis() {
	bool result = myExp->typeAnalysis();
	result = myDeclsT->typeAnalysis() && result;
	result = myStmtsT->typeAnalysis() && result;
	result = myDeclsF->typeAnalysis() && result;
	result = myStmtsF->typeAnalysis() && result;

	if (myExp->getType() != "bool" && myExp->getType() != "") {
		Err::nonBoolInIf(myExp->getAssignPos());
		result = false;
	}
	return result;
}

bool WhileStmtNode::typeAnalysis() {
	bool result = myExp->typeAnalysis();
	result = myDecls->typeAnalysis() && result;
	result = myStmts->typeAnalysis() && result;
	if (myExp->getType() != "bool" && myExp->getType() != "") {
		Err::nonBoolInWhile(myExp->getAssignPos());
		result = false;
	}
	return result;
}

bool ReturnStmtNode::checkReturnType(std::string funcType) {
	if (myExp == nullptr && funcType != "void") {
		Err::returnVoidFromFunc("0:0");
		return false;
	}

	if (myExp != nullptr && funcType == "void") {
		Err::returnValueFromVoidFunc(myExp->getAssignPos());
		return false;
	}

	if (myExp != nullptr && funcType != myExp->getType()) {
		Err::returnBadValue(myExp->getAssignPos());
		return false;
	}

	return true;
}

bool ReturnStmtNode::typeAnalysis() {
	if (myExp != nullptr) {
		return myExp->typeAnalysis();
	} else {
		return true;
	}
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
