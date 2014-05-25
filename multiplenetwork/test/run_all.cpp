/*
 * run_all.cpp
 *
 * Created on: Feb 7, 2014
 * Author: matteomagnani
 * Version: 0.0.1
 */

#include <stdlib.h>
#include <iostream>
#import "test.h"
#import "datastructures.h"
#import "exceptions.h"
#import "utils.h"
#import "math.h"
#include "SIRModel.h"
using namespace std;

MultipleNetwork generate_ERER_MultipleNet(int num_layers, int* num_nodes, double* Ps, double interlayerInfo[][3], int size_interLayerInfo){
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

int main(int argn, char* argv[]) {
	int nums[2] = {10,10};
	double ps[2] = {.1,.2};
	double intLayer[1][3];
			intLayer[0][0] = 0;
			intLayer[0][1] = 1;
			intLayer[0][2] = 0.5;
	MultipleNetwork mnet = generate_ERER_MultipleNet(2,nums,ps,intLayer,1);
	print(mnet);
	SIRModel sirModel = SIRModel(mnet, 0.5, 0.5, 3, 0.5);
	sirModel.start();
//	testNetwork();
//	testMultilayerNetwork();
//	testMultiplex();
	//testMultipleNetwork();
	//testIO();
	//testMeasures();
	//testModularity(); //Still testing this, but seems to work (it does for single networks)
	//testEvolution();
	//testRandom();
	//testTransformations();

	return 0;
}
