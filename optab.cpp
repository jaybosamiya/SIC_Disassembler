#include "optab.h"
#include "util.h"
#include <map>

using namespace std;

map<string,string> optab;
map<string,string> revoptab;

bool add_to_optab(std::string symbol, std::string value) {
	if ( optab.count(symbol) )
		return false;
	optab[symbol] = value;
	revoptab[value] = symbol;
	return true;
}

bool read_from_optab(std::string symbol, std::string &value) {
	if ( !optab.count(symbol) )
		return false;
	value = optab[symbol];
	return true;
}

bool find_from_optab(std::string &symbol, std::string value) {
	if ( !revoptab.count(value) )
		return false;
	symbol = revoptab[value];
	return true;
}

void initialize_optab() {
	optab.clear();
	revoptab.clear();
	add_to_optab("ADD" ,"18");
	add_to_optab("AND" ,"40");
	add_to_optab("COMP","28");
	add_to_optab("DIV" ,"24");
	add_to_optab("J"   ,"3C");
	add_to_optab("JEQ" ,"30");
	add_to_optab("JGT" ,"34");
	add_to_optab("JLT" ,"38");
	add_to_optab("JSUB","48");
	add_to_optab("LDA" ,"00");
	add_to_optab("LDCH","50");
	add_to_optab("LDL" ,"08");
	add_to_optab("LDX" ,"04");
	add_to_optab("MUL" ,"20");
	add_to_optab("OR"  ,"44");
	add_to_optab("RD"  ,"D8");
	add_to_optab("RSUB","4C");
	add_to_optab("STA" ,"0C");
	add_to_optab("STCH","54");
	add_to_optab("STL" ,"14");
	add_to_optab("STX" ,"10");
	add_to_optab("SUB" ,"1C");
	add_to_optab("TD"  ,"E0");
	add_to_optab("TIX" ,"2C");
	add_to_optab("WD"  ,"DC");
}

