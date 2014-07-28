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
#include "readNetFile.h"

using namespace std;

class SIRScenarios{
public:
	static void scenarioForErErNets();
	static void scenarioForErErNetsBody(SIRModel, string, vector<int>);
	static void scenarioForPolBlogs();
	static void scenarioForPolBlogsBody(SIRModel, vector<int>);
	static void scenarioForFlickrAndBlogCatalog();
	static void scFlickrAndBlogCatalogAndSnapBody(SIRModel, vector<int>);
	static void scenarioForSnapFiles();
	static void eighthScenarioRun(SIRModel, vector<int>);
private:
	static int GetSizeOfLargestInfectedClusterOnLayer(MultipleNetwork&, SIRModel&, network_id);
	static void DFS(MultipleNetwork&, network_id, vertex_id, vector<bool>&, int&, std::map<network_id,std::map<vertex_id,SIRVertexStatus > >&);
};


#endif /* SIRSCENARIOS_H_ */
