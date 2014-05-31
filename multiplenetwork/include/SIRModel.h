/*
 * SIRModel.h
 *
 *  Created on: May 20, 2014
 *      Author: payamsiyari
 */

#include "SIREvent.h"
#include "SIREventCompare.h"
#include "SIRVertexStatus.h"
#include "datastructures.h"
#include "utils.h"
#include <queue>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <iostream>

typedef std::priority_queue<SIREvent, std::vector<SIREvent>,SIREventCompare> SIREventQueue;

#ifndef SIRMODEL_H_
#define SIRMODEL_H_

struct simulationStats{
	std::list<int> timeStamp;
	std::list<int> numberOfSusceptibleinTime;
	std::vector<std::list<int> > numberOfSusceptibleinTimeinLayer;
	std::list<int> numberOfInfectedinTime;
	std::vector<std::list<int> > numberOfInfectedinTimeinLayer;
	std::list<int> numberOfRemovedinTime;
	std::vector<std::list<int> > numberOfRemovedinTimeinLayer;

	int S;
	std::vector<int> SinLayer;
	int I;
	std::vector<int> IinLayer;
	int R;
	std::vector<int> RinLayer;
};

class SIRModel{
private:
	unsigned int clock;
	SIREventQueue queue;

	MultipleNetwork mnet;

	std::map<network_id,std::map<vertex_id,SIRVertexStatus > > vertexesStatus;

	double intra_diffusion_rate;
	double inter_diffusion_rate;
	unsigned int infection_timespan;

	simulationStats stat;

	std::map<network_id,std::map<vertex_id,SIRVertexStatus > > defaultVertexesStatus;//For speeding up the reset() method

	void reset();
	global_vertex_id seedSelection();
	global_vertex_id seedSelection(network_id);
	void GatherInfo();
	void PrintInfo();
public:
	SIRModel(MultipleNetwork, double, double, unsigned int);
	simulationStats start();
	simulationStats start(bool, network_id);
	void eventHandlerINFECT(SIREvent);
	void eventHandlerREMOVE(SIREvent);
	void eventHandlerSPREAD(SIREvent);
	MultipleNetwork getNetwork();
	void setDiffRates(double, double);
	double getIntraDiffRate();
	double getInterDiffRate();
};

#endif /* SIRMODEL_H_ */
