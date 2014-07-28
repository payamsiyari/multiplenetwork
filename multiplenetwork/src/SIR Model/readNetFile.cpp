/*
 * readNetFile.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: payamsiyari
 */

#include "readNetFile.h"

MultipleNetwork readNetFile::readPolBlogsGMLFile(string path){
	ifstream file;
	file.open(path);
	string tmpLine;
	MultipleNetwork mnet(false);
	int numVertLayer[] = {759, 735};
	for (int i = 0 ; i < 2 ; i++){
		Network tmpNet(false,false,false);
		for (int j = 0 ; j < numVertLayer[i] ; j++){
			tmpNet.addVertex();
		}
		mnet.addNetwork(tmpNet);
	}
	while(!file.eof()){
		vertex_id v1 = 0;
		int v11 = 0;
		vertex_id v2 = 1;
		int v22 = 1;
		std::getline(file,tmpLine);
		std::getline(file,tmpLine);
//		cout << tmpLine << "\n";
		stringstream tmpSS(tmpLine.c_str());
		sscanf(tmpLine.c_str(),"    source %d", &v1);
		std::getline(file,tmpLine);
//		cout << tmpLine << "\n";
		sscanf(tmpLine.c_str(),"    target %d", &v2);
		std::getline(file,tmpLine);
//		cout << v1 << endl;
//		cout << v2 << endl;
		try{
			if(v1 < numVertLayer[0] && v2 < numVertLayer[0]){
				mnet.getNetwork(0).addEdge(v1, v2);
			}else if(v1 >= numVertLayer[0] && v2 >= numVertLayer[0]){
				v1 -= numVertLayer[0];
				v2 -= numVertLayer[0];
				mnet.getNetwork(1).addEdge(v1, v2);
			}else if(v1 < numVertLayer[0] && v2 >= numVertLayer[0]){
				v2 -= numVertLayer[0];
				mnet.newInterlayerConnection(global_vertex_id(v1,0), global_vertex_id(v2,1));
			}else if(v1 >= numVertLayer[0] && v2 < numVertLayer[0]){
				v1 -= numVertLayer[0];
				mnet.newInterlayerConnection(global_vertex_id(v1,1), global_vertex_id(v2,0));
			}
		}catch(DuplicateElementException &e){

		}
	}
	file.close();
	return mnet;
}

MultipleNetwork readNetFile::readFlickrAndBlogCatalogFile(string path){
	ifstream file;
	file.open(path);
	string tmpLine;
	MultipleNetwork mnet(false);
//	int numVertLayer[] = {1110, 1096};//BlogCatalog1000strong
//	int numVertLayer[] = {1032, 1013};//Flickr1000strong
	int numVertLayer[] = {521, 518};//Flickr500
//	int numVertLayer[] = {552, 465};//Blog500
//	int numVertLayer[] = {741, 725};//Blog700
//	int numVertLayer[] = {1023, 552};//Blog1000500
//	int numVertLayer[] = {1250, 1234};//Blog1200
	for (int i = 0 ; i < 2 ; i++){
		Network tmpNet(false,false,false);
		for (int j = 0 ; j < numVertLayer[i] ; j++){
			tmpNet.addVertex();
		}
		mnet.addNetwork(tmpNet);
	}
	while(!file.eof()){
		vertex_id v1 = -1;
		int v11;
		vertex_id v2 = -1;
		int v22;
		network_id n1 = -1;
		int n11;
		network_id n2 = -1;
		int n22;
		std::getline(file,tmpLine);
//		cout << tmpLine << endl;
		sscanf(tmpLine.c_str(),"%d,%d,%d,%d", &v11,&n11,&v22,&n22);
		v11--;v22--;n11--;n22--;
		v1=v11;v2=v22;n1=n11;n2=n22;
//		cout << "scan " << v1 << "," << v2 << "," << n1 << "," << n2 << endl;
		if(n1 == n2){
			try{
				mnet.getNetwork(n1).addEdge(v1, v2);
			}catch(DuplicateElementException e1){}catch(ElementNotFoundException e){cout << "1 " << tmpLine << endl;}
		}else{
			try{
				mnet.newInterlayerConnection(global_vertex_id(v1,n1), global_vertex_id(v2,n2));
			}catch(DuplicateElementException e){}catch(ElementNotFoundException e){cout << "2 " << tmpLine << endl;}
		}
	}
	file.close();
	return mnet;
}

Network readNetFile::readSnapNetFile(string path, int numVert){
	Network tmpNet(false,false,false);
	for (int j = 0 ; j < numVert ; j++){
		tmpNet.addVertex();
	}
	ifstream file;
	file.open(path);
	string tmpLine;

	std::getline(file,tmpLine);
	std::getline(file,tmpLine);
	std::getline(file,tmpLine);
	std::getline(file,tmpLine);
	while(!file.eof()){
		vertex_id v1 = -1;
		int v11;
		vertex_id v2 = -1;
		int v22;
		std::getline(file,tmpLine);
//		cout << tmpLine << endl;
		sscanf(tmpLine.c_str(),"%d\t%d", &v11,&v22);
		v1=v11;v2=v22;
		try{
			tmpNet.addEdge(v1, v2);
		}catch(DuplicateElementException e){}
	}
	file.close();
	return tmpNet;
}

