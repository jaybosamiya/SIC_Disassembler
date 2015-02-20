#include <iostream>
#include <cstdlib>
#include "util.h"

using namespace std;

void status(string message) {
  cout << "[+] " << message << "\n";
}

void header() {
  cout << "SIC-Disassembler by Jay Bosamiya\n"
          "--------------------------------\n\n";
}

void usage(string progname) {
  cerr << "Usage: " << progname << " input_object_file output_assembly_file\n";
  exit(-2);
}

void error(string err) {
  cerr << "[!] Error: " << err << "\n";
}

void fatal(string err, int code) {
  error(err);
  exit(code);
}

bool is_hex_digit(char c) {
	return
		(c >= 'A' && c <= 'F') ||
		(c >= 'a' && c <= 'f') ||
		(c >= '0' && c <= '9');
}

int hexchar2int(char c) {
	int ret = -1;
	if ( c >= '0' && c <= '9' ) {
		ret = (c-'0');
	} else if ( c >= 'a' && c <= 'f' ) {
		ret = (c-'a')+10;
	} else if ( c >= 'A' && c <= 'F' ) {
		ret = (c-'A')+10;
	}
	return ret;
}

int hex2int(string s) {
	int ret = 0;
	for ( const char * cc = s.c_str() ; *cc ; cc++ ) {
		ret *= 16;
		const char &c = *cc;
		if ( is_hex_digit(c) ) {
			ret += hexchar2int(c);
		} else {
			fatal("Hex values need to be 0-9 or a-f");
		}
	}
}