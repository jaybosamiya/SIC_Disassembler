#include <fstream>
#include "disassembler.h"
#include "messages.h"

using namespace std;

int main(int argc, char ** argv) {
	if ( argc != 3 ) {
		usage(argv[0]);
	}

  header();

	ifstream ifile(argv[1]);
	if ( !ifile ) {
		fatal("Couldn't open input file");
	}

	ofstream ofile(argv[2]);
	if ( !ofile ) {
		fatal("Couldn't open output file");
	}

  status("Opened files");

	if ( !disassemble(ifile,ofile) ) {
	  fatal("Error in disassembly.");
	}

  ifile.close();
  ofile.close();

  status("Disassembly done");

	return 0;
}
