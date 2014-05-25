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
#include <iostream>

typedef std::priority_queue<SIREvent, std::vector<SIREvent>,SIREventCompare> SIREventQueue;

#ifndef SIRMODEL_H_
#define SIRMODEL_H_

class SIRModel{
private:
	unsigned int clock;
	SIREventQueue queue;

	MultipleNetwork mnet;

	std::map<network_id,std::map<vertex_id,SIRVertexStatus > > vertexesStatus;

	double intra_diffusion_rate;
	double inter_diffusion_rate;
	unsigned int infection_timespan;
	double initial_infection_percentage;

	void waitForUserandPrintInfo();
public:
	SIRModel(MultipleNetwork, double, double, unsigned int, double);
	void start();
	void eventHandlerINFECT(SIREvent);
	void eventHandlerREMOVE(SIREvent);
	void eventHandlerSPREAD(SIREvent);
};


#endif /* SIRMODEL_H_ */
