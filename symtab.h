#ifndef SYMTAB_H
#define SYMTAB_H

#include <string>

void initialize_symtab();
bool add_to_symtab(std::string symbol, std::string value);
bool read_from_symtab(std::string symbol, std::string &value); // Gets value which matches symbol
bool find_from_symtab(std::string &symbol, std::string value); // Finds symbol which matches value

bool are_same_in_symtab(std::string symbol, std::string value);

bool give_label(std::string &label, std::string location);
// Gives a label to the location, creating a new label if necessary
// Returns true if label already existed

#endif
