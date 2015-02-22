#include "symtab.h"
#include "util.h"
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
	label = "label" + int2str(label_count++);
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
}

bool are_same_in_symtab(std::string symbol, std::string value) {
	string temp;
	if ( !read_from_symtab(symbol,temp) ) {
		return false;
	}
	return (value==temp);
}