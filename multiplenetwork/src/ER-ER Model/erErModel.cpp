/*
 * erErModel.cpp
 *
 *  Created on: May 19, 2014
 *      Author: payamsiyari
 */

#include "erErModel.h"

using namespace std;

/**
 * @brief Returns a multipleNetwork data structure with each layer being an ER network and interlayer connections having a random Poissonian distribution
 * @param num_layers number of layers in network
 * @param num_nodes[num_layers] number of nodes in each layer
 * @param Ps[num_layers] value of p for generating ER network in each layer
 * @param interlayerInfo[X,3] each column represents the layers to be connected and the value of p for forming their connections
 */
MultipleNetwork erErModel::generate_ERER_MultipleNet_ProbBased(int num_layers, int* num_nodes, double* Ps, double interlayerInfo[][3], int size_interLayerInfo){
	MultipleNetwork mnet;
		//Intralayer connections
		for (int i = 0 ; i < num_layers ; i++){
			//Adding a network and its vertexes
			Network tmpNet;
			for (int j = 0 ; j < num_nodes[i] ; j++){
				tmpNet.addVertex();
			}
			//Forming ER model
			vertex_id v1 = 0;
			vertex_id v2 = 0;
			while (v1 < num_nodes[i]-1){
				Random r;
				double rand = r.getRandomDouble();
				int incr = 0;
				if(rand < Ps[i]){
					incr = 1;
				}else{
					incr = ceil(log(1-rand)/log(1-Ps[i]));
				}
				while (v1 < num_nodes[i]-1){
					if (incr <= num_nodes[i] - 1 - v2){
						v2 += incr;
						if(v1 != v2){
							tmpNet.addEdge(v1,v2);
						}
						break;
					}else{
						incr -= (num_nodes[i] - 1 - v2);
						v1++;
						v2 = v1;
					}
				}
			}
			mnet.addNetwork(tmpNet);
		}
		//Interlayer Connections
		for (int i = 0 ; i < size_interLayerInfo ; i++){
			int v1 = 0;
			int v2 = 0;
			int l1 = (int) interlayerInfo[i][0];
			int l2 = (int) interlayerInfo[i][1];
			while (v1 < num_nodes[l1] - 1){
				Random r;
				double rand = r.getRandomDouble();
				int incr = 0;
				double p = interlayerInfo[i][2];
				if(rand < p){
					incr = 1;
				}else{
					incr = ceil(log(1-rand)/log(1-p));
				}
				while (v1 < num_nodes[l1]){
					if (incr <= num_nodes[l2] - 1 - v2){
						mnet.newInterlayerConnection(global_vertex_id(v1,l1),global_vertex_id(v2,l2));
						v2 += incr;
						break;
					}else{
						incr -= (num_nodes[l2] - 1 - v2);
						v1++;
						v2 = v1;
					}
				}
			}
		}
		return mnet;
}

MultipleNetwork erErModel::generate_ERER_MultipleNet_AvgDegreeBased(int num_layers, int* num_nodes, double* AvgDegs, double interlayerInfo[][3], int size_interLayerInfo){
	double Ps [num_layers];
	double interlayerInfo2 [size_interLayerInfo][3];
	for (int i = 0 ; i < num_layers ; i++){
		Ps[i] = AvgDegs[i]/num_nodes[i];
	}
	for (int i = 0 ; i < size_interLayerInfo ; i++){
		interlayerInfo2[i][0] = interlayerInfo[i][0];
		interlayerInfo2[i][1] = interlayerInfo[i][1];
		interlayerInfo2[i][2] = interlayerInfo[i][2]*(num_nodes[(int)interlayerInfo[i][0]] + num_nodes[(int)interlayerInfo[i][1]])/(2 * num_nodes[(int)interlayerInfo[i][0]] * num_nodes[(int)interlayerInfo[i][1]]);
	}
	return generate_ERER_MultipleNet_ProbBased(num_layers, num_nodes, Ps, interlayerInfo2, size_interLayerInfo);
}

