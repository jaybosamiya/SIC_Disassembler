#ifndef UTIL_H
#define UTIL_H

#include <string>

extern bool show_addresses;

void status(std::string message);
void header();
void usage(std::string progname);
void error(std::string err);
void fatal(std::string err, int code = -1);

bool is_hex_digit(char c);
bool is_hex_string(std::string s);
void make_upper_case(std::string &s);
int hexchar2int(char c);
int hex2int(std::string s);
std::string byte2hex(int c);
std::string int2hex(int c, int bytes = 3);

std::string int2str(int c);

#endif
