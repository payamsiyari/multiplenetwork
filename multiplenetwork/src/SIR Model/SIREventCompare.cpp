/*
 * SIREventCompare.h
 *
 *  Created on: May 20, 2014
 *      Author: payamsiyari
 */

#include "SIREventCompare.h"


bool SIREventCompare::operator() (const SIREvent& lhs, const SIREvent& rhs) const{
	if (lhs.getStartTime() < rhs.getStartTime()){
		return false;
	}else if (lhs.getStartTime() == rhs.getStartTime()){
		if (lhs.getType() < rhs.getType()){
			return false;
		}else{
			return true;
		}
	}else{
		return true;
	}
}
