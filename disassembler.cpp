#include "disassembler.h"
#include "util.h"

using namespace std;

struct program {
	char name[7];
	//TODO
	program() {
		for ( int i = 0 ; i < 7 ; i++ ) {
			name[i] = 0;
		}
		//TODO
	}
};

bool read_record(ifstream &ifile, string &record);
void record_to_memory(const string record, program &p);
void write_assembly(const program &p, ofstream &ofile);

bool disassemble(ifstream &ifile, ofstream &ofile) {
	string record;
	program p;
	while ( read_record(ifile, record) ) {
		record_to_memory(record, p);
	}
	status("Done reading records");
	write_assembly(p, ofile);
	status("Done writing output file");
}

bool read_record(ifstream &ifile, string &record) {
	char t;
	do {
		t = ifile.get();
		if ( t == EOF ) {
			return false;
		}
	} while ( t == '\n' || t == ' ' );
	record = "";
	record += t;
	switch (t) {
		case 'H': // Header
			for ( int col = 2 ; col <= 19 ; col++ ) {
				t = ifile.get();
				if ( t == EOF ) {
					fatal("Unexpected end of Header Record");
				}
				record += t;
			}
			break;
		case 'T': // Text
			//TODO
			break;
		case 'E': // End
			//TODO
			break;
		default:
			fatal("Unknown record type " + t);
	}
	return true;
}

void record_to_memory(const string record, program &p) {
	// TODO
}

void write_assembly(const program &p, ofstream &ofile) {
	// TODO
}