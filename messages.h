#ifndef MESSAGES_H
#define MESSAGES_H

#include <string>

void status(std::string message);
void header();
void usage(std::string progname);
void error(std::string err);
void fatal(std::string err, int code = -1);

#endif
