#include "symtab.h"
#include <map>

using namespace std;

map<string,string> symtab;

bool add_to_symtab(std::string symbol, std::string value) {
	if ( symtab.count(symbol) )
		return false;
	symtab[symbol] = value;
	return true;
}

bool read_from_symtab(std::string symbol, std::string &value) {
	if ( !symtab.count(symbol) )
		return false;
	value = symtab[symbol];
	return true;
}

void initialize() {
	symtab["ADD"]  = "18";
	symtab["AND"]  = "40";
	symtab["COMP"] = "28";
	symtab["DIV"]  = "24";
	symtab["J"]    = "3C";
	symtab["JEQ"]  = "30";
	symtab["JGT"]  = "34";
	symtab["JLT"]  = "38";
	symtab["JSUB"] = "48";
	symtab["LDA"]  = "00";
	symtab["LDCH"] = "50";
	symtab["LDL"]  = "08";
	symtab["LDX"]  = "04";
	symtab["MUL"]  = "20";
	symtab["OR"]   = "44";
	symtab["RD"]   = "D8";
	symtab["RSUB"] = "4C";
	symtab["STA"]  = "0C";
	symtab["STCH"] = "54";
	symtab["STL"]  = "14";
	symtab["STX"]  = "10";
	symtab["SUB"]  = "1C";
	symtab["TD"]   = "E0";
	symtab["TIX"]  = "2C";
	symtab["WD"]   = "DC";
}
