/*
 * erErModel.h
 *
 *  Created on: May 19, 2014
 *      Author: payamsiyari
 */
#ifndef ERERMODEL_H_
#define ERERMODEL_H_

#include "datastructures.h"
#include <algorithm>
#include "utils.h"
#include "math.h"

class erErModel{
public:
	static MultipleNetwork generate_ERER_MultipleNet_ProbBased(int, int*, double*, double[][3], int);
	static MultipleNetwork generate_ERER_MultipleNet_AvgDegreeBased(int, int*, double*, double[][3], int);
};


#endif /* ERERMODEL_H_ */
