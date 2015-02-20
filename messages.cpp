#include <iostream>
#include <cstdlib>
#include "messages.h"

using namespace std;

void status(string message) {
  cout << "[+] " << message << "\n";
}

void header() {
  cout << "SIC-Disassembler by Jay H. Bosamiya\n"
          "-----------------------------------\n\n";
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
