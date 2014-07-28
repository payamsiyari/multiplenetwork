/*
 * run_all.cpp
 *
 * Created on: Feb 7, 2014
 * Author: matteomagnani
 * Version: 0.0.1
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#import "math.h"
#include <string>
#include <list>

#import "test.h"
#import "datastructures.h"
#import "exceptions.h"
#import "utils.h"

#include "readNetFile.h"
#include "erErModel.h"
#include "SIRModel.h"
#include "SIRScenarios.h"


using namespace std;

int main(int argn, char* argv[]) {
	SIRScenarios::scenarioForErErNets();
//	SIRScenarios::scenarioForPolBlogs();
//	SIRScenarios::scenarioForFlickrAndBlogCatalog();
//	SIRScenarios::scenarioForSnapFiles();

//	testNetwork();
//	testMultilayerNetwork();
//	testMultiplex();
	//testMultipleNetwork();
	//testIO();
	//testMeasures();
	//testModularity(); //Still testing this, but seems to work (it does for single networks)
	//testEvolution();
	//testRandom();
	//testTransformations();

	return 0;
}
