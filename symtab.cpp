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