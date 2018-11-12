#include <iostream>
#include <cstdlib>
#include <cstring>

#include "lilc_compiler.hpp"
#include "ast.hpp"

using namespace LILC;

int
main( const int argc, const char **argv )
{
   if (argc != 3){
	std::cout << "Usage: P5 <infile> <outfile>" << std::endl;
	return 1;
   }

   LILC::LilC_Compiler compiler;
   if (compiler.typeAnalysis(argv[1])){
     compiler.unparse(argv[2]);
	return 1;
   }
   return 0;
}
