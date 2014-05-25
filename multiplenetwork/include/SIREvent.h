/*
 * SIREvent.h
 *
 *  Created on: May 20, 2014
 *      Author: payamsiyari
 */

#include "datastructures.h"
#include "SIREventType.h"

#ifndef SIREVENT_H_
#define SIREVENT_H_

class SIREvent{
private:
	SIREventType type;
	unsigned int startTime;
	global_vertex_id vertexID;
public:
	SIREvent(SIREventType, unsigned int, global_vertex_id);
	SIREventType getType() const;
	unsigned int getStartTime() const;
	global_vertex_id getVertexID() const;
};


#endif /* SIREVENT_H_ */
