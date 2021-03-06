/*
 * logging.cpp
 *
 * Created on: Feb 8, 2014
 * Author: matteomagnani
 * Version: 0.0.1
 */

#include "utils.h"
#include <iostream>

void log(std::string s) {
	log(s,2,true);
}

void log(std::string s, bool new_line) {
	log(s,2,new_line);
}

void log(std::string s, verbosity v) {
	log(s,v,true);
}

void log(std::string s, verbosity v, bool new_line) {
	if (VERBOSITY >= v) {
		std::cout << s;
		if (new_line) std::cout << std::endl;
	}
}

