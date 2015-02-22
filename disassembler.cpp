#include "disassembler.h"
#include "util.h"
#include "symtab.h"
#include "optab.h"

#include <vector>
#include <cctype>

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
	unsigned char memory[65536];
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
	return true;
}

string read_columns(ifstream &ifile, int col_begin, int col_end, char record_type = 'a') { // inclusive of both
	string ret;
	char t;
	for ( int col = col_begin ; col <= col_end ; col++ ) {
		t = ifile.get();
		if ( t == EOF || t == '\n' ) {
			string errstr;
			errstr += "Unexpected end of ";
			errstr += record_type;
			errstr += " record";
			fatal(errstr);
		}
		ret += t;
	}
	return ret;
}

string read_hex_columns(ifstream &ifile, int col_begin, int col_end, char record_type = 'a') { // inclusive of both
	string ret;
	ret = read_columns(ifile,col_begin,col_end,record_type);
	make_upper_case(ret);
	if ( !is_hex_string(ret) ) {
		string errstr;
		errstr += "Unexpected non-hexadecimal character found in ";
		errstr += record_type;
		errstr += " record";
		fatal(errstr);
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
			record += read_columns(ifile,2,7,'H');
			record += read_hex_columns(ifile,8,19,'H');
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
			{
				string errstr = "Unknown record type ";
				errstr += t;
				fatal(errstr);
			}
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
			{
				string errstr;
				errstr += "Unknown record type ";
				errstr += *c_record;
				fatal(errstr);
			}
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
				if ( p.byte_type_guess[location] != UNKNOWN &&
					 p.byte_type_guess[location] != UNINITIALIZED ) {
					break;
				}
				if ( p.byte_type_guess[location] == UNINITIALIZED )
					btg = UNINITIALIZED_CHAR_DATA;
				p.byte_type_guess[location] = btg;
				break;
			}
		case WORD_DATA:
		case UNINITIALIZED_WORD_DATA:
			{
				if ( p.byte_type_guess[location] != UNKNOWN &&
					 p.byte_type_guess[location] != UNINITIALIZED ) {
					break;
				}
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
				if ( p.byte_type_guess[location] == CODE ) {
					break;
				}
				p.byte_type_guess[location+0] = btg;
				p.byte_type_guess[location+1] = btg;
				p.byte_type_guess[location+2] = btg;

				string opcode_val = byte2hex(p.memory[location]);
				string opcode;
				string operand = byte2hex(p.memory[location+1])+byte2hex(p.memory[location+2]);
				operand = int2hex(hex2int(operand)&0x7FFF); // remove index flag
				if ( ! find_from_optab(opcode,opcode_val) ) {
					string errstr;
					errstr += "Unknown opcode ";
					errstr += opcode_val;
					errstr += " at location ";
					errstr += int2hex(location);
					fatal(errstr);
				}

				if ( opcode == "ADD"  ||
					 opcode == "AND"  ||
					 opcode == "COMP" ||
					 opcode == "DIV"  ||
					 opcode == "LDA"  ||
					 opcode == "LDL"  ||
					 opcode == "LDX"  ||
					 opcode == "MUL"  ||
					 opcode == "OR"   ||
					 opcode == "STA"  ||
					 opcode == "STL"  ||
					 opcode == "STX"  ||
					 opcode == "SUB"  ||
					 opcode == "TIX" ) {
					give_label(operand,"VARW");
					p.is_labelled[hex2int(operand)] = true;
					mark_code_data(p,hex2int(operand),WORD_DATA);
				}

				if ( opcode == "LDCH" ||
					 opcode == "STCH" ||
					 opcode == "RD"   ||
					 opcode == "TD"   ||
					 opcode == "WD" ) {
					give_label(operand,"VARC");
					p.is_labelled[hex2int(operand)] = true;
					mark_code_data(p,hex2int(operand),CHAR_DATA);
				}

				if ( opcode != "J" &&
					 opcode != "RSUB" ) {
					mark_code_data(p,location+3,btg);
				}

				if ( opcode == "J"    ||
					 opcode == "JEQ"  ||
					 opcode == "JGT"  ||
					 opcode == "JLT"  ||
					 opcode == "JSUB" ) {
					give_label(operand,"INS");
					p.is_labelled[hex2int(operand)] = true;
					mark_code_data(p,hex2int(operand),CODE);
				}

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
	initialize_symtab();
	initialize_optab();
	add_to_symtab("FIRST",p.first_executable_instruction);
	p.is_labelled[hex2int(p.first_executable_instruction)] = true;
	mark_code_data(p,hex2int(p.first_executable_instruction),CODE);
}

string asm_to_line(string label, string opcode, string operand, bool is_indexed) {
	const int labellength = 8;
	const int opcodelength = 6;
	string ret = "";
	ret += label + string(labellength-label.length(),' ');
	ret += opcode + string(opcodelength-opcode.length(),' ');
	ret += operand;
	if ( is_indexed ) {
		ret += ",X";
	}
	ret += '\n';
	return ret;
}

void write_assembly(const program &p, ofstream &ofile) {
	const char * start_address = p.starting_address.c_str();
	while ( *start_address == '0' ) {
		start_address++;
	}
	ofile << asm_to_line(p.name,"START",string(start_address),false);
	int start_of_program = hex2int(p.starting_address);
	int end_of_program = start_of_program + p.length_of_program;

	for ( int locctr = start_of_program ; locctr < end_of_program ; ) {
		string label = "";
		string opcode = "";
		string operand = "";
		bool is_indexed = false;

		if ( p.is_labelled[locctr] ) {
			if ( !find_from_symtab(label,int2hex(locctr)) ) {
				string errstr;
				errstr += "Label not created for location ";
				errstr += int2hex(locctr);
				error(errstr);
			}
		}

		if ( p.byte_type_guess[locctr] == CODE ) {
			if ( ! find_from_optab(opcode,byte2hex(p.memory[locctr])) ) {
				string errstr;
				errstr += "Unknown opcode ";
				errstr += opcode;
				errstr += " at location ";
				errstr += int2hex(locctr);
				fatal(errstr);
			}
			operand = byte2hex(p.memory[locctr+1])+byte2hex(p.memory[locctr+2]);
			is_indexed = (hex2int(operand)&0x8000);
			operand = int2hex(hex2int(operand)&0x7FFF); // remove index flag

			if ( opcode != "RSUB" ) {
				if ( !find_from_symtab(operand,operand) ) {
					string errstr;
					errstr += "Label not created for operand ";
					errstr += operand;
					error(errstr);
				}
			}

			if ( opcode == "RSUB" ) {
				operand = "";
			}

			locctr += 3;
		} else if ( p.byte_type_guess[locctr] == CHAR_DATA ||
					p.byte_type_guess[locctr] == UNKNOWN ) {
			if ( p.byte_type_guess[locctr] == UNKNOWN ) {
				label = "UNUSED"; // This should not happen in a well written code
			}

			vector<unsigned char> byte_list;
			bool type_c = true;
			do {
				byte_list.push_back(p.memory[locctr]);
				if ( !isprint(p.memory[locctr]) ) {
					type_c = false;
				}
				locctr++;
			} while ( p.byte_type_guess[locctr] == CHAR_DATA && !p.is_labelled[locctr] );

			opcode = "CHAR";

			operand += (type_c?"C":"X");
			operand += "'";
			for ( int i = 0 ; i < (int)byte_list.size() ; i++ ) {
				if ( type_c ) {
					operand += byte_list[i];
				} else {
					operand += byte2hex(byte_list[i]);
				}
			}
			operand += "'";
		} else if ( p.byte_type_guess[locctr] == WORD_DATA ) {
			opcode = "WORD";
			operand = int2str(
					p.memory[locctr+0]*256*256+
					p.memory[locctr+1]*256+
					p.memory[locctr+2]
				);
			locctr += 3;
		} else if ( p.byte_type_guess[locctr] == UNINITIALIZED_CHAR_DATA ||
		            p.byte_type_guess[locctr] == UNINITIALIZED ) {
			opcode = "RESB";
			int buf_size = 0;
			do {
				buf_size++;
				locctr++;
			} while ( (p.byte_type_guess[locctr] == UNINITIALIZED_CHAR_DATA
				    || p.byte_type_guess[locctr] == UNINITIALIZED)
			        && !p.is_labelled[locctr]
			        && locctr < end_of_program);
			if ( locctr >= end_of_program ) {
				buf_size--;
			}
			operand = int2str(buf_size);
		} else if ( p.byte_type_guess[locctr] == UNINITIALIZED_WORD_DATA ) {
			opcode = "RESW";
			int buf_size = 0;
			do {
				buf_size++;
				locctr+=3;
			} while ( (p.byte_type_guess[locctr] == UNINITIALIZED_WORD_DATA
				    || p.byte_type_guess[locctr] == UNINITIALIZED)
					&& !p.is_labelled[locctr]
					&& locctr < end_of_program);
			if ( locctr >= end_of_program ) {
				buf_size--;
			}
			operand = int2str(buf_size);
		} else {
			fatal("Reached part of decompiler that should not be reached");
		}
		ofile << asm_to_line(label,opcode,operand,is_indexed);
	}
	ofile << asm_to_line("","END","FIRST",false);
}