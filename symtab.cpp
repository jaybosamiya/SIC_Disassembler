#include "symtab.h"
#include "util.h"
#include <map>

using namespace std;

map<string,string> symtab;
map<string,string> revsymtab;
map<string,int> label_count;

map<string,int> read_write;
const int READ = 1;
const int WRITE = 2;

bool add_to_symtab(std::string symbol, std::string value) {
	if ( symtab.count(symbol) )
		return false;
	symtab[symbol] = value;
	revsymtab[value] = symbol;
	return true;
}

bool find_from_symtab(std::string &symbol, std::string value) {
	if ( !revsymtab.count(value) && !read_write.count(value) )
		return false;
	if ( read_write.count(value) ) {
		int x = read_write[value];
		read_write.erase(value);
		if ( x & WRITE ) {
			give_label(value,"VAR");
		} else {
			give_label(value,"CONST");
		}
	}
	symbol = revsymtab[value];
	return true;
}

bool give_label(std::string location, std::string label_type) {
	string label;
	if ( find_from_symtab(label,location) )
		return true;
	label = label_type + int2str(label_count[label_type]++);
	add_to_symtab(label,location);
	return false;
}

void initialize_symtab() {
	symtab.clear();
	revsymtab.clear();
	label_count.clear();
	read_write.clear();
}

void mark_as_readable_data(std::string location) {
	read_write[location] |= READ;
}

void mark_as_writable_data(std::string location) {
	read_write[location] |= WRITE;
}