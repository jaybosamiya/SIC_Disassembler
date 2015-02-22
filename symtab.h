#ifndef SYMTAB_H
#define SYMTAB_H

#include <string>

void initialize_symtab();
bool add_to_symtab(std::string symbol, std::string value);
bool find_from_symtab(std::string &symbol, std::string value); // Finds symbol which matches value

bool give_label(std::string location, std::string label_type);
// Gives a label to the location, creating a new label if necessary
// Returns true if label already existed

void mark_as_readable_data(std::string location);
void mark_as_writable_data(std::string location);

#endif
