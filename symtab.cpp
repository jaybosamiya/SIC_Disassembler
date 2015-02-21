#include "symtab.h"
#include <map>

using namespace std;

map<string,string> symtab;
map<string,string> revsymtab;
int label_count = 0;

bool add_to_symtab(std::string symbol, std::string value) {
	if ( symtab.count(symbol) )
		return false;
	symtab[symbol] = value;
	revsymtab[value] = symbol;
	return true;
}

bool read_from_symtab(std::string symbol, std::string &value) {
	if ( !symtab.count(symbol) )
		return false;
	value = symtab[symbol];
	return true;
}

bool find_from_symtab(std::string &symbol, std::string value) {
	if ( !revsymtab.count(value) )
		return false;
	symbol = revsymtab[value];
	return true;
}

bool give_label(std::string &label, std::string location) {
	if ( find_from_symtab(label,location) )
		return true;
	label = "label" + label_count;
	label_count++;
	add_to_symtab(label,location);
	return false;
}

bool give_label(std::string location) {
	string temp;
	return give_label(temp,location);
}

void initialize_symtab() {
	symtab.clear();
	revsymtab.clear();
	label_count = 0;
	add_to_symtab("ADD" ,"18");
	add_to_symtab("AND" ,"40");
	add_to_symtab("COMP","28");
	add_to_symtab("DIV" ,"24");
	add_to_symtab("J"   ,"3C");
	add_to_symtab("JEQ" ,"30");
	add_to_symtab("JGT" ,"34");
	add_to_symtab("JLT" ,"38");
	add_to_symtab("JSUB","48");
	add_to_symtab("LDA" ,"00");
	add_to_symtab("LDCH","50");
	add_to_symtab("LDL" ,"08");
	add_to_symtab("LDX" ,"04");
	add_to_symtab("MUL" ,"20");
	add_to_symtab("OR"  ,"44");
	add_to_symtab("RD"  ,"D8");
	add_to_symtab("RSUB","4C");
	add_to_symtab("STA" ,"0C");
	add_to_symtab("STCH","54");
	add_to_symtab("STL" ,"14");
	add_to_symtab("STX" ,"10");
	add_to_symtab("SUB" ,"1C");
	add_to_symtab("TD"  ,"E0");
	add_to_symtab("TIX" ,"2C");
	add_to_symtab("WD"  ,"DC");
}

bool are_same_in_symtab(std::string symbol, std::string value) {
	string temp;
	if ( !read_from_symtab(symbol,temp) ) {
		return false;
	}
	return (value==temp);
}