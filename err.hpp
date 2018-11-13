#ifndef LILC_ERROR_REPORTING_HH
#define LILC_ERROR_REPORTING_HH

#include <iostream>

namespace LILC{

class Err{
	public:
	static void report(std::string pos, std::string msg){
		std::cerr << pos
			<< " ***ERROR*** " << msg << std::endl;
	}

	static bool multiDecl(std::string pos){
		report(pos, "Multiply declared identifiers");
		return false;
	}

	static bool undeclaredID(std::string pos){
		report(pos, "Undeclared identifier");
		return false;
	}

	static bool undefType(std::string pos){
		report(pos, "Undefined type");
		return false;
	}

	static bool badVoid(std::string pos){
		report(pos, "Non-function declared void");
		return false;
	}

	static bool badDotLHS(std::string pos){
		report(pos, "Dot-access of non-struct type");
	}

	static bool badDotRHS(std::string pos){
		report(pos, "Invalid struct field name");
	}

	static bool typeMismatch(std::string pos) {
		report(pos, "Type mismatch");
	}

	static bool nonArthimetic(std::string pos) {
		report (pos, "Arithmetic operator applied to non-numeric operand");
	}
	static bool nonLogical(std::string pos) {
		report (pos, "Logical operator applied to non-bool operand");
	}
	static bool nonRelational(std::string pos) {
		report (pos, "Relational operator applied to non-numeric operand");
	}
	static bool readFunction(std::string pos) {
		report(pos, "Attempt to read a function");
	}

	static bool readStructName(std::string pos) {
		report (pos, "Attempt to read a struct name");
	}

	static bool readStructVar(std::string pos) {
		report(pos, "Attempt to read a struct variable");
	}

	static bool writeFunction(std::string pos) {
		report (pos, "Attempt to write a function");
	}

	static bool writeStructName(std::string pos) {
		report (pos, "Attempt to write a struct name");
	}

	static bool writeStructVar(std::string pos) {
		report (pos, "Attempt to write a struct variable");
	}

	static bool writeVoidResult(std::string pos) {
		report (pos, "Attempt to write void");
	}

	static bool callNonFunction(std::string pos) {
		report(pos, "Attempt to call a non-function");
	}

	static bool callFunctionWrongNumArgs(std::string pos) {
		report(pos, "Function call with wrong number of args");
	}

	static bool actualNotMatchingFormal(std::string pos) {
		report(pos, "Type of actual does not match type of formal");
	}
};

} //End namespace LILC

#endif
