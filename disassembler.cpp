#include "disassembler.h"
#include "util.h"
#include "symtab.h"

using namespace std;

enum ByteTypeGuess {
	UNINITIALIZED,
	UNKNOWN,
	CHAR_DATA,
	UNINITIALIZED_CHAR_DATA,
	WORD_DATA,
	UNINITIALIZED_WORD_DATA,
	CODE,
};

struct program {
	ByteTypeGuess byte_type_guess[65536];
	char memory[65536];
	bool is_labelled[65536];

	string name;
	string starting_address;
	int length_of_program;
	string first_executable_instruction;
	program() {
		for ( int i = 0 ; i < 65536 ; i++ ) {
			byte_type_guess[i] = UNINITIALIZED;
			memory[i] = 0;
			is_labelled[i] = false;
		}
		name = "";
		starting_address = "";
		length_of_program = 0;
		first_executable_instruction = "";
	}
};

bool read_record(ifstream &ifile, string &record);
void record_to_memory(const string record, program &p);
void analyze_code_data(program &p);
void write_assembly(const program &p, ofstream &ofile);

bool disassemble(ifstream &ifile, ofstream &ofile) {
	string record;
	program p;
	while ( read_record(ifile, record) ) {
		record_to_memory(record, p);
	}
	status("Done reading records");
	analyze_code_data(p);
	status("Done analyzing program for code and data");
	write_assembly(p, ofile);
	status("Done writing output file");
}

string read_hex_columns(ifstream &ifile, int col_begin, int col_end, char record_type = 'a') { // inclusive of both
	string ret;
	char t;
	for ( int col = col_begin ; col <= col_end ; col++ ) {
		t = ifile.get();
		if ( t == EOF ) {
			fatal("Unexpected end of " + record_type + string(" record"));
		} else if ( !is_hex_digit(t) ) {
			fatal("Unexpected character " + t + string(" in ") + record_type + string(" record"));
		}
		ret += t;
	}
	return ret;
}

bool read_record(ifstream &ifile, string &record) {
	int temp_int;
	string temp_str;
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
			record += read_hex_columns(ifile,2,19,'H');
			break;
		case 'T': // Text
			record += read_hex_columns(ifile,2,7,'T');
			temp_str = read_hex_columns(ifile,8,9,'T');
			temp_int = hex2int(temp_str);
			record += temp_str;
			record +=  read_hex_columns(ifile,10,9+2*temp_int,'T');
			break;
		case 'E': // End
			record += read_hex_columns(ifile,2,7,'E');
			break;
		default:
			fatal("Unknown record type " + t);
	}
	return true;
}

void record_to_memory(const string record, program &p) {
	const char * c_record = record.c_str();
	switch (*c_record) {
		case 'H': // Header
			if ( p.name.length() != 0 ) {
				fatal("Multiple H records");
			}
			p.name = record.substr(1,6);
			p.starting_address = record.substr(7,6);
			p.length_of_program = hex2int(record.substr(13,6));
			break;
		case 'T': // Text
			{
				int text_start = hex2int(record.substr(1,6));
				int bit_length = hex2int(record.substr(7,2));
				for ( int i = 0 ; i < bit_length ; i++ ) {
					p.memory[i+text_start] = hex2int(record.substr(9+2*i,2));
					p.byte_type_guess[i+text_start] = UNKNOWN;
				}
			}
			break;
		case 'E': // End
			if ( p.first_executable_instruction.length() != 0 ) {
				fatal("Multiple E records");
			}
			p.first_executable_instruction = record.substr(1,6);
			break;
		default:
			fatal("Unknown record type " + *c_record);
	}
}

void mark_code_data(program &p, int location, ByteTypeGuess btg) {
	if ( location >= hex2int(p.starting_address) + p.length_of_program ) {
		return;
	}
	switch (btg) {
		case UNINITIALIZED:
		case UNKNOWN:
			break;
		case CHAR_DATA:
		case UNINITIALIZED_CHAR_DATA:
			{
				if ( p.byte_type_guess[location] == UNINITIALIZED )
					btg = UNINITIALIZED_CHAR_DATA;
				p.byte_type_guess[location] = btg;
				break;
			}
		case WORD_DATA:
		case UNINITIALIZED_WORD_DATA:
			{
				if ( p.byte_type_guess[location] == UNINITIALIZED )
					btg = UNINITIALIZED_WORD_DATA;
				p.byte_type_guess[location+0] = btg;
				p.byte_type_guess[location+1] = btg;
				p.byte_type_guess[location+2] = btg;
				break;
			}
		case CODE:
			{
				if ( p.byte_type_guess[location] == UNINITIALIZED ) {
					fatal("Attempting to use uninitialized section as code");
				}
				p.byte_type_guess[location+0] = btg;
				p.byte_type_guess[location+1] = btg;
				p.byte_type_guess[location+2] = btg;
				// TODO: Handle conditional and unconditional jumps here
				// TODO: Handle data marking here
				mark_code_data(p,location+3,btg);
				break;
			}
	}
}

void analyze_code_data(program &p) {
	if ( p.name.length() == 0 ) {
		fatal("No Header record");
	}
	if ( p.first_executable_instruction.length() == 0 ) {
		fatal("No End record");
	}
	add_to_symtab("FIRST",p.first_executable_instruction);
	p.is_labelled[hex2int(p.first_executable_instruction)] = true;
	mark_code_data(p,hex2int(p.first_executable_instruction),CODE);
}

void write_assembly(const program &p, ofstream &ofile) {
	// TODO
}