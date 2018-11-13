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
};

} //End namespace LILC

#endif
