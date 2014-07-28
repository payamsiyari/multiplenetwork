/*
 * readNetFile.h
 *
 *  Created on: Jun 10, 2014
 *      Author: payamsiyari
 */

#ifndef READNETFILE_H_
#define READNETFILE_H_

#include "datastructures.h"
#include "exceptions.h"
#include "stdlib.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
 #include <cstdlib>

using namespace std;

class readNetFile{
public:
	static MultipleNetwork readPolBlogsGMLFile(string);
	static MultipleNetwork readFlickrAndBlogCatalogFile(string);
	static Network readSnapNetFile(string,int);
};


#endif /* READNETFILE_H_ */
