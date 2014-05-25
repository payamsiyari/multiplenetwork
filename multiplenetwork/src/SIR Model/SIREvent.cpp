/*
 * SIREvent.cpp
 *
 *  Created on: May 20, 2014
 *      Author: payamsiyari
 */

#include "SIREvent.h"

SIREvent::SIREvent(SIREventType _type, unsigned int _startTime, global_vertex_id _vertexID){
	type = _type;
	startTime = _startTime;
	vertexID = _vertexID;
}

SIREventType SIREvent::getType() const{
	return type;
}

unsigned int SIREvent::getStartTime() const{
	return startTime;
}

global_vertex_id SIREvent::getVertexID() const{
	return vertexID;
}


