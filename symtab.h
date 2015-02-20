#ifndef SYMTAB_H
#define SYMTAB_H

#include <string>

bool add_to_symtab(std::string symbol, std::string value);
bool read_from_symtab(std::string symbol, std::string &value);

#endif
