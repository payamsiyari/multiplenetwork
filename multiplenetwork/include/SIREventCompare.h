/*
 * SIREventCompare.h
 *
 *  Created on: May 20, 2014
 *      Author: payamsiyari
 */

#include "SIREvent.h"

#ifndef SIREVENTCOMPARE_H_
#define SIREVENTCOMPARE_H_

class SIREventCompare{
public:
	bool operator() (const SIREvent& lhs, const SIREvent& rhs) const;
};


#endif /* SIREVENTCOMPARE_H_ */
