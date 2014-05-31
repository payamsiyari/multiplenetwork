/*
 * SIRScenarios.h
 *
 *  Created on: May 29, 2014
 *      Author: payamsiyari
 */

#ifndef SIRSCENARIOS_H_
#define SIRSCENARIOS_H_

#include <algorithm>
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#import "math.h"
#include <string>
#include <vector>
#include "utils.h"
#include "math.h"

#include "datastructures.h"
#import "exceptions.h"
#import "utils.h"
#include "erErModel.h"
#include "SIRModel.h"

using namespace std;

class SIRScenarios{
public:
	static void firstScenario();
	static void firstScenarioRun(SIRModel, string);
	static void secondScenario();
	static void secondScenarioRun(SIRModel,string);
	static void thirdScenario();
	static void fourthScenario();
	static void fourthScenarioRun(SIRModel, string, vector<int>);
};


#endif /* SIRSCENARIOS_H_ */
