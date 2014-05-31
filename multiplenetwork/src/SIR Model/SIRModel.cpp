/*
 * SIRModel.cpp
 *
 *  Created on: May 20, 2014
 *      Author: payamsiyari
 */

#include "SIRModel.h"
#include <string>
#include <iostream>
#include <limits>
#include "exceptions.h"

SIRModel::SIRModel(MultipleNetwork _mnet, double _intra_diffusion_rate, double _inter_diffusion_rate, unsigned int _infection_timespan){
	mnet = _mnet;

	std::set<network_id> mnetNets;
	mnet.getNetworks(mnetNets);
	for (std::set<network_id>::iterator itNet=mnetNets.begin(); itNet!=mnetNets.end(); ++itNet){
		std::set<vertex_id> tmpVertexes;
		mnet.getNetwork(*itNet).getVertexes(tmpVertexes);
		for (std::set<vertex_id>::iterator itVert=tmpVertexes.begin(); itVert!=tmpVertexes.end(); ++itVert){
			defaultVertexesStatus[*itNet].insert(std::pair<vertex_id,SIRVertexStatus>(*itVert,SUSCEPTIBLE));
		}
	}

	intra_diffusion_rate = _intra_diffusion_rate;
	inter_diffusion_rate = _inter_diffusion_rate;
	infection_timespan = _infection_timespan;
}

void SIRModel::reset(){
	clock = 0;
	queue = SIREventQueue();
	vertexesStatus = defaultVertexesStatus;
	stat.S = mnet.getNumVertexes(); stat.I = 0; stat.R = 0;
	const int numNets = mnet.getNumNetworks();
	stat.SinLayer.clear(); stat.SinLayer.resize(mnet.getNumNetworks(),0);
	stat.IinLayer.clear(); stat.IinLayer.resize(mnet.getNumNetworks(),0);
	stat.RinLayer.clear(); stat.RinLayer.resize(mnet.getNumNetworks(),0);
	for (int i = 0 ; i < numNets ; i++){
		stat.SinLayer[i] = mnet.getNetwork((network_id) i).getNumVertexes();
		stat.IinLayer[i] = 0;
		stat.RinLayer[i] = 0;
	}
	stat.numberOfSusceptibleinTime = std::list<int>();
	stat.numberOfSusceptibleinTimeinLayer.clear();
	stat.numberOfSusceptibleinTimeinLayer.resize(mnet.getNumNetworks());
	stat.numberOfInfectedinTime = std::list<int>();
	stat.numberOfInfectedinTimeinLayer.clear();
	stat.numberOfInfectedinTimeinLayer.resize(mnet.getNumNetworks());
	stat.numberOfRemovedinTime = std::list<int>();
	stat.numberOfRemovedinTimeinLayer.clear();
	stat.numberOfRemovedinTimeinLayer.resize(mnet.getNumNetworks());
	stat.timeStamp = std::list<int>();
}

global_vertex_id SIRModel::seedSelection(){
	Random r;
	network_id randNet = r.getRandomInt(mnet.getNumNetworks());
	return global_vertex_id(r.getRandomInt(mnet.getNetwork(randNet).getNumVertexes()),randNet);
}

global_vertex_id SIRModel::seedSelection(network_id layer){
	Random r;
	if(!mnet.containsNetwork(layer)) throw ElementNotFoundException("Network ID " + std::to_string(layer));
	return global_vertex_id(r.getRandomInt(mnet.getNetwork(layer).getNumVertexes()),layer);
}

simulationStats SIRModel::start(){
	return start(false,(network_id)-1);
}

simulationStats SIRModel::start(bool enableSeedLayerSelection, network_id seedLayer){
	//Resetting clock, queue, initial stats and initial vertexStatus
	reset();
	Random r;
	//Infecting the seed node and inserting initial events of queue
	global_vertex_id tmpVert;
	if(!enableSeedLayerSelection){
		tmpVert = seedSelection();
	}else{
		tmpVert = seedSelection(seedLayer);
	}
	network_id randomNetworkID = tmpVert.network;
	vertex_id randomVertexID = tmpVert.vid;
	vertexesStatus[randomNetworkID].erase(randomVertexID);
	stat.S--; stat.SinLayer[randomNetworkID]--;
	vertexesStatus[randomNetworkID].insert(std::pair<vertex_id,SIRVertexStatus>(randomVertexID,INFECTED));
	stat.I++; stat.IinLayer[randomNetworkID]++;
	queue.push(SIREvent(SPREAD,clock,global_vertex_id(randomVertexID,randomNetworkID)));
	queue.push(SIREvent(REMOVE,clock+infection_timespan,global_vertex_id(randomVertexID,randomNetworkID)));

	while(!queue.empty()){
		SIREvent imminentEvent = queue.top();
		if(imminentEvent.getStartTime() > clock)
			GatherInfo();
		clock = imminentEvent.getStartTime();
		if(imminentEvent.getType() == INFECT){
			eventHandlerINFECT(imminentEvent);
		}else if(imminentEvent.getType() == REMOVE){
			eventHandlerREMOVE(imminentEvent);
		}else if(imminentEvent.getType() == SPREAD){
			eventHandlerSPREAD(imminentEvent);
		}
		queue.pop();
	}
	GatherInfo();
	return stat;
}

void SIRModel::eventHandlerINFECT(SIREvent e){
	//Check to avoid double status change
	if(vertexesStatus[e.getVertexID().network][e.getVertexID().vid] != INFECTED){
		vertexesStatus[e.getVertexID().network].erase(e.getVertexID().vid);
		stat.S--; stat.SinLayer[e.getVertexID().network]--;
		vertexesStatus[e.getVertexID().network].insert(std::pair<vertex_id,SIRVertexStatus>(e.getVertexID().vid,INFECTED));
		stat.I++; stat.IinLayer[e.getVertexID().network]++;
		queue.push(SIREvent(SPREAD,clock,e.getVertexID()));
		queue.push(SIREvent(REMOVE,clock+infection_timespan,e.getVertexID()));
	}
}

void SIRModel::eventHandlerREMOVE(SIREvent e){
	//Check to avoid double status change
	if(vertexesStatus[e.getVertexID().network][e.getVertexID().vid] != REMOVED){
		vertexesStatus[e.getVertexID().network].erase(e.getVertexID().vid);
		stat.I--; stat.IinLayer[e.getVertexID().network]--;
		vertexesStatus[e.getVertexID().network].insert(std::pair<vertex_id,SIRVertexStatus>(e.getVertexID().vid,REMOVED));
		stat.R++; stat.RinLayer[e.getVertexID().network]++;
	}
}

void SIRModel::eventHandlerSPREAD(SIREvent e){
	Random r;
	double tmpRandom = r.getRandomDouble();
	if(vertexesStatus[e.getVertexID().network][e.getVertexID().vid] != REMOVED){
		//Finding intralayer neighbors
		std::set<vertex_id> intraNeighs;
		mnet.getNetwork(e.getVertexID().network).getOutNeighbors(e.getVertexID().vid, intraNeighs);

//		std::cout << "IntraNeighs: " ;
		for (std::set<vertex_id>::iterator itNeigh=intraNeighs.begin(); itNeigh!=intraNeighs.end(); ++itNeigh){
//			std::cout << *itNeigh;
			if(vertexesStatus[e.getVertexID().network][*itNeigh] == SUSCEPTIBLE){
				if(tmpRandom < intra_diffusion_rate){
					queue.push(SIREvent(INFECT,clock+1,global_vertex_id(*itNeigh,e.getVertexID().network)));
//					std::cout << "I";
				}
			}
//			std::cout << " ";
		}
//		std::cout << "\n" ;

//		std::cout << "InterNeighs: " ;
		std::set<global_vertex_id> interNeighs;
		mnet.getOutInterlayerNeighbors(e.getVertexID(), interNeighs);
		for (std::set<global_vertex_id>::iterator itNeigh=interNeighs.begin(); itNeigh!=interNeighs.end(); ++itNeigh){
//			std::cout << (*itNeigh).vid << "-" << (*itNeigh).network;
			if(vertexesStatus[(*itNeigh).network][(*itNeigh).vid] == SUSCEPTIBLE){
				if(tmpRandom < inter_diffusion_rate){
					queue.push(SIREvent(INFECT,clock+1,*itNeigh));
//					std::cout << "I";
				}
			}
//			std::cout << " ";
		}
//		std::cout << "\n";
		queue.push(SIREvent(SPREAD,clock+1,e.getVertexID()));
	}
}

MultipleNetwork SIRModel::getNetwork(){
	return mnet;
}

void SIRModel::setDiffRates(double _intralayer, double _interlayer){
	intra_diffusion_rate = _intralayer;
	inter_diffusion_rate = _interlayer;
}

double SIRModel::getIntraDiffRate(){
	return intra_diffusion_rate;
}

double SIRModel::getInterDiffRate(){
	return inter_diffusion_rate;
}

void SIRModel::GatherInfo(){
	stat.numberOfSusceptibleinTime.push_back(stat.S);
	stat.numberOfInfectedinTime.push_back(stat.I);
	stat.numberOfRemovedinTime.push_back(stat.R);
	for(int i = 0 ; i < mnet.getNumNetworks() ; i++){
		stat.numberOfSusceptibleinTimeinLayer[i].push_back(stat.SinLayer[i]);
		stat.numberOfInfectedinTimeinLayer[i].push_back(stat.IinLayer[i]);
		stat.numberOfRemovedinTimeinLayer[i].push_back(stat.RinLayer[i]);
	}
	stat.timeStamp.push_back(clock);

	//Printing Queue status and vertexes status
//	PrintInfo();
}

void SIRModel::PrintInfo(){
	std::cout << "eventQueue top element is: \n\t\t TYPE: " ;
	if(queue.top().getType() == 0)
		std::cout << "INFECT" << std::endl;
	else if(queue.top().getType() == 1)
		std::cout << "REMOVE" << std::endl;
	else if(queue.top().getType() == 2)
		std::cout << "SPREAD" << std::endl;
	std::cout << "\t\t TIME: " << queue.top().getStartTime() << "\n\t\t VID:  " << queue.top().getVertexID().vid << "\n\t\t NID:  " << queue.top().getVertexID().network << std::endl;

	int S = 0, I = 0, R = 0;

	std::set<network_id> mnetNets;
	mnet.getNetworks(mnetNets);
	for (std::set<network_id>::iterator itNet=mnetNets.begin(); itNet!=mnetNets.end(); ++itNet){
		std::set<vertex_id> tmpVertexes;
		mnet.getNetwork(*itNet).getVertexes(tmpVertexes);
		std::cout << "N" << (*itNet) << " " ;
		for (std::set<vertex_id>::iterator itVert=tmpVertexes.begin(); itVert!=tmpVertexes.end(); ++itVert){
			if(vertexesStatus[*itNet][*itVert] == SUSCEPTIBLE){
				std::cout << *itVert << "S" << " ";
				S++;
			}
			if(vertexesStatus[*itNet][*itVert] == INFECTED){
				std::cout << *itVert << "I" << " ";
				I++;
			}
			if(vertexesStatus[*itNet][*itVert] == REMOVED){
				std::cout << *itVert << "R" << " ";
				R++;
			}


		}
		std::cout << std::endl;
	}

	std::cout << "S: " << S << " I: " << I << " R: " << R << std::endl;

	std::cout << "To Continue, Press Enter..." << std::endl;
	std::string s;
	std::getline(std::cin, s);
}
