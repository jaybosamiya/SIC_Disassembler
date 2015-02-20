#ifndef UTIL_H
#define UTIL_H

#include <string>

void status(std::string message);
void header();
void usage(std::string progname);
void error(std::string err);
void fatal(std::string err, int code = -1);

int hex2int(std::string s);

#endif
