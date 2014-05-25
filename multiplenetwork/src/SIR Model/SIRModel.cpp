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

SIRModel::SIRModel(MultipleNetwork _mnet, double _intra_diffusion_rate, double _inter_diffusion_rate, unsigned int _infection_timespan, double _initial_infection_percentage){
	clock = 0;
	queue = SIREventQueue();

	mnet = _mnet;

	intra_diffusion_rate = _intra_diffusion_rate;
	inter_diffusion_rate = _inter_diffusion_rate;
	infection_timespan = _infection_timespan;
	initial_infection_percentage = _initial_infection_percentage;
}

void SIRModel::start(){
	Random r;
	//Filling initial vertexStatus and initial events of queue
	std::set<network_id> mnetNets;
	mnet.getNetworks(mnetNets);
	for (std::set<network_id>::iterator itNet=mnetNets.begin(); itNet!=mnetNets.end(); ++itNet){
		std::set<vertex_id> tmpVertexes;
		mnet.getNetwork(*itNet).getVertexes(tmpVertexes);
		for (std::set<vertex_id>::iterator itVert=tmpVertexes.begin(); itVert!=tmpVertexes.end(); ++itVert){
			if(r.getRandomDouble() > initial_infection_percentage){
				vertexesStatus[*itNet].insert(std::pair<vertex_id,SIRVertexStatus>(*itVert,SUSCEPTIBLE));
			}else{
				vertexesStatus[*itNet].insert(std::pair<vertex_id,SIRVertexStatus>(*itVert,INFECTED));
				queue.push(SIREvent(SPREAD,clock,global_vertex_id(*itVert,*itNet)));
				queue.push(SIREvent(REMOVE,clock+infection_timespan,global_vertex_id(*itVert,*itNet)));
			}
		}
	}
	while(!queue.empty()){
//		waitForUserandPrintInfo();

		SIREvent imminentEvent = queue.top();
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
}

void SIRModel::eventHandlerINFECT(SIREvent e){
	//Check to avoid double status change
	if(vertexesStatus[e.getVertexID().network][e.getVertexID().vid] != INFECTED){
		vertexesStatus[e.getVertexID().network].erase(e.getVertexID().vid);
		vertexesStatus[e.getVertexID().network].insert(std::pair<vertex_id,SIRVertexStatus>(e.getVertexID().vid,INFECTED));
		queue.push(SIREvent(REMOVE,clock+infection_timespan,e.getVertexID()));
	}
}

void SIRModel::eventHandlerREMOVE(SIREvent e){
	//Check to avoid double status change
	if(vertexesStatus[e.getVertexID().network][e.getVertexID().vid] != REMOVED){
		vertexesStatus[e.getVertexID().network].erase(e.getVertexID().vid);
		vertexesStatus[e.getVertexID().network].insert(std::pair<vertex_id,SIRVertexStatus>(e.getVertexID().vid,REMOVED));
	}
}

void SIRModel::eventHandlerSPREAD(SIREvent e){
	Random r;
	double tmpRandom = r.getRandomDouble();
	if(vertexesStatus[e.getVertexID().network][e.getVertexID().vid] != REMOVED){
		//Finding intralayer neighbors
		std::set<vertex_id> intraNeighs;
		mnet.getNetwork(e.getVertexID().network).getOutNeighbors(e.getVertexID().vid, intraNeighs);
		for (std::set<vertex_id>::iterator itNeigh=intraNeighs.begin(); itNeigh!=intraNeighs.end(); ++itNeigh){
			if(vertexesStatus[e.getVertexID().network][*itNeigh] == SUSCEPTIBLE){
				if(tmpRandom < intra_diffusion_rate){
					queue.push(SIREvent(INFECT,clock+1,global_vertex_id(*itNeigh,e.getVertexID().network)));
				}
			}
		}
		std::set<global_vertex_id> interNeighs;
		mnet.getOutInterlayerNeighbors(e.getVertexID(), interNeighs);
		for (std::set<global_vertex_id>::iterator itNeigh=interNeighs.begin(); itNeigh!=interNeighs.end(); ++itNeigh){
			if(vertexesStatus[(*itNeigh).network][(*itNeigh).vid] == SUSCEPTIBLE){
				if(tmpRandom < inter_diffusion_rate){
					queue.push(SIREvent(INFECT,clock+1,*itNeigh));
				}
			}
		}
		queue.push(SIREvent(SPREAD,clock+1,e.getVertexID()));
	}
}

void SIRModel::waitForUserandPrintInfo(){
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
