/*
 * SIRScenarios.cpp
 *
 *  Created on: May 29, 2014
 *      Author: payamsiyari
 */

#include "SIRScenarios.h"

void SIRScenarios::scenarioForErErNets(){
	//Network Generation
	const double WEEK = 0.1;
	const double STRONG = 1.5;
	const int numLayers = 2;
	int nums[numLayers] = {10000,10000};
	double avgs[numLayers] = {6,6};
	double interLayerInfo[1][3];
			interLayerInfo[0][0] = 0;
			interLayerInfo[0][1] = 1;

	//SIR Model Run
	int infectionTimeSpan = 5;//Recovery rate
	double intralayerDiffRate = 0.01;
	double interlayerDiffRate = 0.01;
	for (int counter = 1 ; counter < 2 ; counter++){
		if(counter == 0){
			interLayerInfo[0][2] = WEEK;
		}
		if(counter == 1){
			interLayerInfo[0][2] = STRONG;
		}
		MultipleNetwork mnet = erErModel::generate_ERER_MultipleNet_AvgDegreeBased(numLayers,nums,avgs,interLayerInfo,1);
		print(mnet);
		cout << "e0: " << mnet.getNetwork(0).getNumEdges() << endl;
		cout << "e1: " << mnet.getNetwork(1).getNumEdges() << endl;
		cout << "eInter: " << mnet.getNumInterlayerConnections() << endl;
		vector<int> GCCSizeInLayer(mnet.getNumNetworks());
		for (int layerNumber = 0 ; layerNumber < mnet.getNumNetworks() ; layerNumber++){
			//Find the largest connected component size in each layer
			GCCSizeInLayer[layerNumber] = mnet.getNetwork(layerNumber).getGCCSize();
			cout << "gcc" << layerNumber << ": " << GCCSizeInLayer[layerNumber] << endl;
		}
		SIRModel sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
		for(intralayerDiffRate = 0.01 ; intralayerDiffRate <= 1.00 ; intralayerDiffRate += 0.05){
			for(interlayerDiffRate = 0.01 ; interlayerDiffRate <= 1.00 ; interlayerDiffRate += 0.05){
				sirModel.setDiffRates(intralayerDiffRate, interlayerDiffRate);
				cout << "intra: " << intralayerDiffRate << ", inter: " << interlayerDiffRate << endl;
				//Find average of infected nodes in each layer
				if(counter == 0)
					SIRScenarios::scenarioForErErNetsBody(sirModel, "week", GCCSizeInLayer);
				if(counter == 1)
					SIRScenarios::scenarioForErErNetsBody(sirModel, "strong", GCCSizeInLayer);
			}
			ofstream myfile;
			string filename = "";
			if(counter == 0){
				filename = "week";
			}
			if(counter == 1){
				filename = "strong";
			}
			myfile.open ("Sc5-"+filename+"AllS.txt",ios::app);
			myfile << "\n";
			myfile.close();
			myfile.open ("Sc5-"+filename+"AllI.txt",ios::app);
			myfile << "\n";
			myfile.close();
			myfile.open ("Sc5-"+filename+"AllR.txt",ios::app);
			myfile << "\n";
			myfile.close();
			myfile.open ("Sc5-"+filename+"AllSinTime.txt",ios::app);
			myfile << "\n\n";
			myfile.close();
			myfile.open ("Sc5-"+filename+"AllIinTime.txt",ios::app);
			myfile << "\n\n";
			myfile.close();
			myfile.open ("Sc5-"+filename+"AllRinTime.txt",ios::app);
			myfile << "\n\n";
			myfile.close();
			for (int j = 0 ; j <  mnet.getNumNetworks() ; j++){
				myfile.open ("Sc5-"+filename+ std::to_string(j) +"S.txt",ios::app);
				myfile << "\n" ;
				myfile.close();
				myfile.open ("Sc5-"+filename+ std::to_string(j) +"I.txt",ios::app);
				myfile << "\n" ;
				myfile.close();
				myfile.open ("Sc5-"+filename+ std::to_string(j) +"R.txt",ios::app);
				myfile << "\n" ;
				myfile.close();
				myfile.open ("Sc5-"+filename+ std::to_string(j) +"RGccRatio.txt",ios::app);
				myfile << "\n" ;
				myfile.close();
				myfile.open ("Sc5-"+filename+ std::to_string(j) +"SinTime.txt",ios::app);
				myfile << "\n\n" ;
				myfile.close();
				myfile.open ("Sc5-"+filename+ std::to_string(j) +"IinTime.txt",ios::app);
				myfile << "\n\n" ;
				myfile.close();
				myfile.open ("Sc5-"+filename+ std::to_string(j) +"RinTime.txt",ios::app);
				myfile << "\n\n" ;
				myfile.close();
			}
		}
	}
}

void SIRScenarios::scenarioForErErNetsBody(SIRModel sirModel, string filename, vector<int> GCCSizeInLayer){
	double avgNumberS = 0;
	vector<double> avgNumberSinLayer = vector<double>(sirModel.getNetwork().getNumNetworks(),0);
	double avgNumberI = 0;
	vector<double> avgNumberIinLayer = vector<double>(sirModel.getNetwork().getNumNetworks(),0);
	double avgNumberR = 0;
	vector<double> avgNumberRinLayer = vector<double>(sirModel.getNetwork().getNumNetworks(),0);

	vector<double> avgRGccRatioinLayer = vector<double>(sirModel.getNetwork().getNumNetworks(),0);

	std::vector<double> totalNumberSInTime;
	std::vector<std::vector<double> > totalNumberSinLayerInTime = vector<vector<double> >(sirModel.getNetwork().getNumNetworks());
	std::vector<double> totalNumberIInTime;
	std::vector<std::vector<double> > totalNumberIinLayerInTime = vector<vector<double> >(sirModel.getNetwork().getNumNetworks());
	std::vector<double> totalNumberRInTime;
	std::vector<std::vector<double> > totalNumberRinLayerInTime = vector<vector<double> >(sirModel.getNetwork().getNumNetworks());

	double numberOfRealizations = 50;
	for (int i = 0 ; i < numberOfRealizations ; i++){
		simulationStats runTimeStat = sirModel.start();
		avgNumberS += runTimeStat.S;
		avgNumberI += runTimeStat.I;
		avgNumberR += runTimeStat.R;
		for (int j = 0 ; j <  avgNumberSinLayer.size() ; j++){
			avgNumberSinLayer[j] += runTimeStat.SinLayer[j];
			avgNumberIinLayer[j] += runTimeStat.IinLayer[j];
			avgNumberRinLayer[j] += runTimeStat.RinLayer[j];
//			avgRGccRatioinLayer[j] += (double)fifthScenarioGetSizeOfLargestInfectedClusterOnLayer(sirModel.mnet, sirModel, j)/(double)runTimeStat.R;
		}
		if(runTimeStat.numberOfSusceptibleinTime.size() > totalNumberSInTime.size()){
			totalNumberSInTime.resize(runTimeStat.numberOfSusceptibleinTime.size(),0);
			totalNumberIInTime.resize(runTimeStat.numberOfInfectedinTime.size(),0);
			totalNumberRInTime.resize(runTimeStat.numberOfRemovedinTime.size(),0);
		}
		std::list<int>::iterator listItNumSuc=runTimeStat.numberOfSusceptibleinTime.begin();
		std::list<int>::iterator listItNumInf=runTimeStat.numberOfInfectedinTime.begin();
		std::list<int>::iterator listItNumRem=runTimeStat.numberOfRemovedinTime.begin();
		for (int j = 0 ; listItNumSuc!=runTimeStat.numberOfSusceptibleinTime.end(); ++listItNumSuc, ++listItNumInf, ++listItNumRem, ++j){
			totalNumberSInTime[j] += *listItNumSuc;
			totalNumberIInTime[j] += *listItNumInf;
			totalNumberRInTime[j] += *listItNumRem;
		}
		for (int j = 0 ; j < runTimeStat.SinLayer.size() ; j++){
			if(runTimeStat.numberOfSusceptibleinTimeinLayer[j].size() > totalNumberSinLayerInTime[j].size()){
				totalNumberSinLayerInTime[j].resize(runTimeStat.numberOfSusceptibleinTimeinLayer[j].size());
				totalNumberIinLayerInTime[j].resize(runTimeStat.numberOfInfectedinTimeinLayer[j].size());
				totalNumberRinLayerInTime[j].resize(runTimeStat.numberOfRemovedinTimeinLayer[j].size());
			}
			std::list<int>::iterator listItNumSucinLayer = runTimeStat.numberOfSusceptibleinTimeinLayer[j].begin();
			std::list<int>::iterator listItNumInfinLayer = runTimeStat.numberOfInfectedinTimeinLayer[j].begin();
			std::list<int>::iterator listItNumReminLayer = runTimeStat.numberOfRemovedinTimeinLayer[j].begin();
			for (int k = 0 ; listItNumSucinLayer != runTimeStat.numberOfSusceptibleinTimeinLayer[j].end() ; ++listItNumSucinLayer, ++listItNumInfinLayer, ++listItNumReminLayer, ++k){
				totalNumberSinLayerInTime[j][k] += *listItNumSucinLayer;
				totalNumberIinLayerInTime[j][k] += *listItNumInfinLayer;
				totalNumberRinLayerInTime[j][k] += *listItNumReminLayer;
			}
		}
	}
	avgNumberS /= numberOfRealizations;
	avgNumberI /= numberOfRealizations;
	avgNumberR /= numberOfRealizations;
	for (int j = 0 ; j <  avgNumberSinLayer.size() ; j++){
		avgNumberSinLayer[j] /= numberOfRealizations;
		avgNumberIinLayer[j] /= numberOfRealizations;
		avgNumberRinLayer[j] /= numberOfRealizations;
		avgRGccRatioinLayer[j] /= numberOfRealizations;
//		cout << "avgRatio " << j << ": " <<  avgRGccRatioinLayer[j] << endl;
	}
	ofstream myfile;
	myfile.open ("Sc5-"+filename+"AllS.txt",ios::app);
	myfile << avgNumberS << "\t" ;
	myfile.close();
	myfile.open ("Sc5-"+filename+"AllI.txt",ios::app);
	myfile << avgNumberI << "\t" ;
	myfile.close();
	myfile.open ("Sc5-"+filename+"AllR.txt",ios::app);
	myfile << avgNumberR << "\t" ;
	myfile.close();
	myfile.open ("Sc5-"+filename+"AllSinTime.txt",ios::app);
	for (int i = 0 ; i < totalNumberSInTime.size() ; i++)
		myfile << totalNumberSInTime[i]/numberOfRealizations << "\t" ;
	myfile << "\n" ;
	myfile.close();
	myfile.open ("Sc5-"+filename+"AllIinTime.txt",ios::app);
	for (int i = 0 ; i < totalNumberIInTime.size() ; i++)
		myfile << totalNumberIInTime[i]/numberOfRealizations << "\t" ;
	myfile << "\n" ;
	myfile.close();
	myfile.open ("Sc5-"+filename+"AllRinTime.txt",ios::app);
	for (int i = 0 ; i < totalNumberRInTime.size() ; i++)
		myfile << totalNumberRInTime[i]/numberOfRealizations << "\t" ;
	myfile << "\n" ;
	myfile.close();
	for (int j = 0 ; j <  avgNumberSinLayer.size() ; j++){
		myfile.open ("Sc5-"+filename+ std::to_string(j) +"S.txt",ios::app);
		myfile << avgNumberSinLayer[j]/(double)GCCSizeInLayer[j] << "\t" ;
		myfile.close();
		myfile.open ("Sc5-"+filename+ std::to_string(j) +"I.txt",ios::app);
		myfile << avgNumberIinLayer[j]/(double)GCCSizeInLayer[j] << "\t" ;
		myfile.close();
		myfile.open ("Sc5-"+filename+ std::to_string(j) +"R.txt",ios::app);
		myfile << avgNumberRinLayer[j]/(double)GCCSizeInLayer[j] << "\t" ;
		myfile.close();
		myfile.open ("Sc5-"+filename+ std::to_string(j) +"RGccRatio.txt",ios::app);
		myfile << avgRGccRatioinLayer[j] << "\t" ;
		myfile.close();
		myfile.open ("Sc5-"+filename+ std::to_string(j) +"SinTime.txt",ios::app);
		for (int k = 0 ; k < totalNumberSinLayerInTime[j].size() ; k++)
			myfile << totalNumberSinLayerInTime[j][k]/numberOfRealizations << "\t" ;
		myfile << "\n" ;
		myfile.close();
		myfile.open ("Sc5-"+filename+ std::to_string(j) +"IinTime.txt",ios::app);
		for (int k = 0 ; k < totalNumberIinLayerInTime[j].size() ; k++)
			myfile << totalNumberIinLayerInTime[j][k]/numberOfRealizations << "\t" ;
		myfile << "\n" ;
		myfile.close();
		myfile.open ("Sc5-"+filename+ std::to_string(j) +"RinTime.txt",ios::app);
		for (int k = 0 ; k < totalNumberRinLayerInTime[j].size() ; k++)
			myfile << totalNumberRinLayerInTime[j][k]/numberOfRealizations << "\t" ;
		myfile << "\n" ;
		myfile.close();
	}
}

int SIRScenarios::GetSizeOfLargestInfectedClusterOnLayer(MultipleNetwork& mnet, SIRModel& sirModel, network_id layerNumber){
	vector<bool> vertexTraverseCheck(mnet.getNetwork(layerNumber).getNumVertexes(),false);
	int infectedGccSize = 0;
	for (int i = 0 ; i < vertexTraverseCheck.size() ; i++){
		if(!vertexTraverseCheck[i]){
			int tmpComponentSize = 0;
			DFS(mnet, layerNumber, i, vertexTraverseCheck, tmpComponentSize, sirModel.vertexesStatus);
			if(tmpComponentSize > infectedGccSize)
				infectedGccSize = tmpComponentSize;
		}
	}
	return infectedGccSize;
}

void SIRScenarios::DFS(MultipleNetwork& mnet, network_id layerNumber, vertex_id vid, vector<bool>& vertexTraverseCheck, int& componentSize, std::map<network_id,std::map<vertex_id,SIRVertexStatus > >& vertexesStatus){
	if(!vertexTraverseCheck[vid]){
		vertexTraverseCheck[vid] = true;
		if(vertexesStatus[layerNumber][vid] == REMOVED){
			componentSize++;
			set<vertex_id> neighs;
			mnet.getNetwork(layerNumber).getOutNeighbors(vid, neighs);
			for(set<vertex_id>::iterator vertIt = neighs.begin() ; vertIt != neighs.end() ; ++vertIt){
				DFS(mnet, layerNumber, *vertIt, vertexTraverseCheck, componentSize, vertexesStatus);
			}
		}
	}
}

void SIRScenarios::scenarioForPolBlogs(){
	//Network Generation
	MultipleNetwork mnet = readNetFile::readPolBlogsGMLFile("polblogs.gml");
	print(mnet);
//	cout << "edges 0: " << mnet.getNetwork(0).getNumEdges() << endl;
//	cout << "edges 1: " << mnet.getNetwork(1).getNumEdges() << endl;

	//SIR Model Run
	int infectionTimeSpan = 5;//Recovery rate
	double intralayerDiffRate = 0.01;
	double interlayerDiffRate = 0.01;
	cout << "e0: " << mnet.getNetwork(0).getNumEdges() << endl;
	cout << "e1: " << mnet.getNetwork(1).getNumEdges() << endl;
	cout << "eInter: " << mnet.getNumInterlayerConnections() << endl;
	vector<int> GCCSizeInLayer(mnet.getNumNetworks());
	for (int layerNumber = 0 ; layerNumber < mnet.getNumNetworks() ; layerNumber++){
		//Find the largest connected component size in each layer
		GCCSizeInLayer[layerNumber] = mnet.getNetwork(layerNumber).getGCCSize();
		cout << "gcc" << layerNumber << ": " << GCCSizeInLayer[layerNumber] << endl;
	}
	SIRModel sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
	for(intralayerDiffRate = 0.01 ; intralayerDiffRate <= 1.00 ; intralayerDiffRate += 0.05){
		for(interlayerDiffRate = 0.01 ; interlayerDiffRate <= 1.00 ; interlayerDiffRate += 0.05){
			sirModel.setDiffRates(intralayerDiffRate, interlayerDiffRate);
			cout << "intra: " << intralayerDiffRate << ", inter: " << interlayerDiffRate << endl;
			//Find average of infected nodes in each layer
			SIRScenarios::scenarioForPolBlogsBody(sirModel, GCCSizeInLayer);
		}
		ofstream myfile;
		myfile.open ("Sc6-AllS.txt",ios::app);
		myfile << "\n";
		myfile.close();
		myfile.open ("Sc6-AllI.txt",ios::app);
		myfile << "\n";
		myfile.close();
		myfile.open ("Sc6-AllR.txt",ios::app);
		myfile << "\n";
		myfile.close();
		myfile.open ("Sc6-AllSinTime.txt",ios::app);
		myfile << "\n\n";
		myfile.close();
		myfile.open ("Sc6-AllIinTime.txt",ios::app);
		myfile << "\n\n";
		myfile.close();
		myfile.open ("Sc6-AllRinTime.txt",ios::app);
		myfile << "\n\n";
		myfile.close();
		for (int j = 0 ; j <  mnet.getNumNetworks() ; j++){
			myfile.open ("Sc6-"+ std::to_string(j) +"S.txt",ios::app);
			myfile << "\n" ;
			myfile.close();
			myfile.open ("Sc6-"+ std::to_string(j) +"I.txt",ios::app);
			myfile << "\n" ;
			myfile.close();
			myfile.open ("Sc6-"+ std::to_string(j) +"R.txt",ios::app);
			myfile << "\n" ;
			myfile.close();
			myfile.open ("Sc6-"+ std::to_string(j) +"RGccRatio.txt",ios::app);
			myfile << "\n" ;
			myfile.close();
			myfile.open ("Sc6-"+ std::to_string(j) +"SinTime.txt",ios::app);
			myfile << "\n\n" ;
			myfile.close();
			myfile.open ("Sc6-"+ std::to_string(j) +"IinTime.txt",ios::app);
			myfile << "\n\n" ;
			myfile.close();
			myfile.open ("Sc6-"+ std::to_string(j) +"RinTime.txt",ios::app);
			myfile << "\n\n" ;
			myfile.close();
		}
	}
}

void SIRScenarios::scenarioForPolBlogsBody(SIRModel sirModel, vector<int> GCCSizeInLayer){
	double avgNumberS = 0;
	vector<double> avgNumberSinLayer = vector<double>(sirModel.getNetwork().getNumNetworks(),0);
	double avgNumberI = 0;
	vector<double> avgNumberIinLayer = vector<double>(sirModel.getNetwork().getNumNetworks(),0);
	double avgNumberR = 0;
	vector<double> avgNumberRinLayer = vector<double>(sirModel.getNetwork().getNumNetworks(),0);

	vector<double> avgRGccRatioinLayer = vector<double>(sirModel.getNetwork().getNumNetworks(),0);

	std::vector<double> totalNumberSInTime;
	std::vector<std::vector<double> > totalNumberSinLayerInTime = vector<vector<double> >(sirModel.getNetwork().getNumNetworks());
	std::vector<double> totalNumberIInTime;
	std::vector<std::vector<double> > totalNumberIinLayerInTime = vector<vector<double> >(sirModel.getNetwork().getNumNetworks());
	std::vector<double> totalNumberRInTime;
	std::vector<std::vector<double> > totalNumberRinLayerInTime = vector<vector<double> >(sirModel.getNetwork().getNumNetworks());

	double numberOfRealizations = 100;
	for (int i = 0 ; i < numberOfRealizations ; i++){
		simulationStats runTimeStat = sirModel.start();
		avgNumberS += runTimeStat.S;
		avgNumberI += runTimeStat.I;
		avgNumberR += runTimeStat.R;
		for (int j = 0 ; j <  avgNumberSinLayer.size() ; j++){
			avgNumberSinLayer[j] += runTimeStat.SinLayer[j];
			avgNumberIinLayer[j] += runTimeStat.IinLayer[j];
			avgNumberRinLayer[j] += runTimeStat.RinLayer[j];
//			avgRGccRatioinLayer[j] += (double)fifthScenarioGetSizeOfLargestInfectedClusterOnLayer(sirModel.mnet, sirModel, j)/(double)runTimeStat.R;
		}
		if(runTimeStat.numberOfSusceptibleinTime.size() > totalNumberSInTime.size()){
			totalNumberSInTime.resize(runTimeStat.numberOfSusceptibleinTime.size(),0);
			totalNumberIInTime.resize(runTimeStat.numberOfInfectedinTime.size(),0);
			totalNumberRInTime.resize(runTimeStat.numberOfRemovedinTime.size(),0);
		}
		std::list<int>::iterator listItNumSuc=runTimeStat.numberOfSusceptibleinTime.begin();
		std::list<int>::iterator listItNumInf=runTimeStat.numberOfInfectedinTime.begin();
		std::list<int>::iterator listItNumRem=runTimeStat.numberOfRemovedinTime.begin();
		for (int j = 0 ; listItNumSuc!=runTimeStat.numberOfSusceptibleinTime.end(); ++listItNumSuc, ++listItNumInf, ++listItNumRem, ++j){
			totalNumberSInTime[j] += *listItNumSuc;
			totalNumberIInTime[j] += *listItNumInf;
			totalNumberRInTime[j] += *listItNumRem;
		}
		for (int j = 0 ; j < runTimeStat.SinLayer.size() ; j++){
			if(runTimeStat.numberOfSusceptibleinTimeinLayer[j].size() > totalNumberSinLayerInTime[j].size()){
				totalNumberSinLayerInTime[j].resize(runTimeStat.numberOfSusceptibleinTimeinLayer[j].size());
				totalNumberIinLayerInTime[j].resize(runTimeStat.numberOfInfectedinTimeinLayer[j].size());
				totalNumberRinLayerInTime[j].resize(runTimeStat.numberOfRemovedinTimeinLayer[j].size());
			}
			std::list<int>::iterator listItNumSucinLayer = runTimeStat.numberOfSusceptibleinTimeinLayer[j].begin();
			std::list<int>::iterator listItNumInfinLayer = runTimeStat.numberOfInfectedinTimeinLayer[j].begin();
			std::list<int>::iterator listItNumReminLayer = runTimeStat.numberOfRemovedinTimeinLayer[j].begin();
			for (int k = 0 ; listItNumSucinLayer != runTimeStat.numberOfSusceptibleinTimeinLayer[j].end() ; ++listItNumSucinLayer, ++listItNumInfinLayer, ++listItNumReminLayer, ++k){
				totalNumberSinLayerInTime[j][k] += *listItNumSucinLayer;
				totalNumberIinLayerInTime[j][k] += *listItNumInfinLayer;
				totalNumberRinLayerInTime[j][k] += *listItNumReminLayer;
			}
		}
	}
	avgNumberS /= numberOfRealizations;
	avgNumberI /= numberOfRealizations;
	avgNumberR /= numberOfRealizations;
	for (int j = 0 ; j <  avgNumberSinLayer.size() ; j++){
		avgNumberSinLayer[j] /= numberOfRealizations;
		avgNumberIinLayer[j] /= numberOfRealizations;
		avgNumberRinLayer[j] /= numberOfRealizations;
		avgRGccRatioinLayer[j] /= numberOfRealizations;
//		cout << "avgRatio " << j << ": " <<  avgRGccRatioinLayer[j] << endl;
	}
	ofstream myfile;
	myfile.open ("Sc6-AllS.txt",ios::app);
	myfile << avgNumberS << "\t" ;
	myfile.close();
	myfile.open ("Sc6-AllI.txt",ios::app);
	myfile << avgNumberI << "\t" ;
	myfile.close();
	myfile.open ("Sc6-AllR.txt",ios::app);
	myfile << avgNumberR << "\t" ;
	myfile.close();
	myfile.open ("Sc6-AllSinTime.txt",ios::app);
	for (int i = 0 ; i < totalNumberSInTime.size() ; i++)
		myfile << totalNumberSInTime[i]/numberOfRealizations << "\t" ;
	myfile << "\n" ;
	myfile.close();
	myfile.open ("Sc6-AllIinTime.txt",ios::app);
	for (int i = 0 ; i < totalNumberIInTime.size() ; i++)
		myfile << totalNumberIInTime[i]/numberOfRealizations << "\t" ;
	myfile << "\n" ;
	myfile.close();
	myfile.open ("Sc6-AllRinTime.txt",ios::app);
	for (int i = 0 ; i < totalNumberRInTime.size() ; i++)
		myfile << totalNumberRInTime[i]/numberOfRealizations << "\t" ;
	myfile << "\n" ;
	myfile.close();
	for (int j = 0 ; j <  avgNumberSinLayer.size() ; j++){
		myfile.open ("Sc6-"+ std::to_string(j) +"S.txt",ios::app);
		myfile << avgNumberSinLayer[j]/(double)GCCSizeInLayer[j] << "\t" ;
		myfile.close();
		myfile.open ("Sc6-"+ std::to_string(j) +"I.txt",ios::app);
		myfile << avgNumberIinLayer[j]/(double)GCCSizeInLayer[j] << "\t" ;
		myfile.close();
		myfile.open ("Sc6-"+ std::to_string(j) +"R.txt",ios::app);
		myfile << avgNumberRinLayer[j]/(double)GCCSizeInLayer[j] << "\t" ;
		myfile.close();
		myfile.open ("Sc6-"+ std::to_string(j) +"RGccRatio.txt",ios::app);
		myfile << avgRGccRatioinLayer[j] << "\t" ;
		myfile.close();
		myfile.open ("Sc6-"+ std::to_string(j) +"SinTime.txt",ios::app);
		for (int k = 0 ; k < totalNumberSinLayerInTime[j].size() ; k++)
			myfile << totalNumberSinLayerInTime[j][k]/numberOfRealizations << "\t" ;
		myfile << "\n" ;
		myfile.close();
		myfile.open ("Sc6-"+ std::to_string(j) +"IinTime.txt",ios::app);
		for (int k = 0 ; k < totalNumberIinLayerInTime[j].size() ; k++)
			myfile << totalNumberIinLayerInTime[j][k]/numberOfRealizations << "\t" ;
		myfile << "\n" ;
		myfile.close();
		myfile.open ("Sc6-"+ std::to_string(j) +"RinTime.txt",ios::app);
		for (int k = 0 ; k < totalNumberRinLayerInTime[j].size() ; k++)
			myfile << totalNumberRinLayerInTime[j][k]/numberOfRealizations << "\t" ;
		myfile << "\n" ;
		myfile.close();
	}
}

void SIRScenarios::scenarioForFlickrAndBlogCatalog(){
	//Network Generation
//	MultipleNetwork mnet = readPolBlogsGML::readBlogsFile("blog1000.txt");
//	MultipleNetwork mnet = readPolBlogsGML::readBlogsFile("flickr1000.txt");
	MultipleNetwork mnet = readNetFile::readFlickrAndBlogCatalogFile("flickr500.txt");
//	MultipleNetwork mnet = readPolBlogsGML::readBlogsFile("blog500.txt");
//	MultipleNetwork mnet = readPolBlogsGML::readBlogsFile("blog700.txt");
//	MultipleNetwork mnet = readPolBlogsGML::readBlogsFile("blog1000500.txt");
//	MultipleNetwork mnet = readPolBlogsGML::readBlogsFile("blog1200.txt");
	print(mnet);

	//SIR Model Run
	int infectionTimeSpan = 5;//Recovery rate
	double intralayerDiffRate = 0.01;
	double interlayerDiffRate = 0.01;
	cout << "v0: " << mnet.getNetwork(0).getNumVertexes() << endl;
	cout << "e0: " << mnet.getNetwork(0).getNumEdges() << endl;
	cout << "v1: " << mnet.getNetwork(1).getNumVertexes() << endl;
	cout << "e1: " << mnet.getNetwork(1).getNumEdges() << endl;
	cout << "eInter: " << mnet.getNumInterlayerConnections() << endl;
	vector<int> GCCSizeInLayer(mnet.getNumNetworks());
	for (int layerNumber = 0 ; layerNumber < mnet.getNumNetworks() ; layerNumber++){
		//Find the largest connected component size in each layer
		GCCSizeInLayer[layerNumber] = mnet.getNetwork(layerNumber).getGCCSize();
		cout << "gcc" << layerNumber << ": " << GCCSizeInLayer[layerNumber] << endl;
	}
	SIRModel sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
	for(intralayerDiffRate = 0.01 ; intralayerDiffRate <= 1.00 ; intralayerDiffRate += 0.05){
		for(interlayerDiffRate = 0.01 ; interlayerDiffRate <= 1.00 ; interlayerDiffRate += 0.05){
			sirModel.setDiffRates(intralayerDiffRate, interlayerDiffRate);
			cout << "intra: " << intralayerDiffRate << ", inter: " << interlayerDiffRate << endl;
			//Find average of infected nodes in each layer
			SIRScenarios::scFlickrAndBlogCatalogAndSnapBody(sirModel, GCCSizeInLayer);
		}
		ofstream myfile;
		myfile.open ("Sc7-AllS.txt",ios::app);
		myfile << "\n";
		myfile.close();
		myfile.open ("Sc7-AllI.txt",ios::app);
		myfile << "\n";
		myfile.close();
		myfile.open ("Sc7-AllR.txt",ios::app);
		myfile << "\n";
		myfile.close();
		myfile.open ("Sc7-AllSinTime.txt",ios::app);
		myfile << "\n\n";
		myfile.close();
		myfile.open ("Sc7-AllIinTime.txt",ios::app);
		myfile << "\n\n";
		myfile.close();
		myfile.open ("Sc7-AllRinTime.txt",ios::app);
		myfile << "\n\n";
		myfile.close();
		for (int j = 0 ; j <  mnet.getNumNetworks() ; j++){
			myfile.open ("Sc7-"+ std::to_string(j) +"S.txt",ios::app);
			myfile << "\n" ;
			myfile.close();
			myfile.open ("Sc7-"+ std::to_string(j) +"I.txt",ios::app);
			myfile << "\n" ;
			myfile.close();
			myfile.open ("Sc7-"+ std::to_string(j) +"R.txt",ios::app);
			myfile << "\n" ;
			myfile.close();
			myfile.open ("Sc7-"+ std::to_string(j) +"RGccRatio.txt",ios::app);
			myfile << "\n" ;
			myfile.close();
			myfile.open ("Sc7-"+ std::to_string(j) +"SinTime.txt",ios::app);
			myfile << "\n\n" ;
			myfile.close();
			myfile.open ("Sc7-"+ std::to_string(j) +"IinTime.txt",ios::app);
			myfile << "\n\n" ;
			myfile.close();
			myfile.open ("Sc7-"+ std::to_string(j) +"RinTime.txt",ios::app);
			myfile << "\n\n" ;
			myfile.close();
		}
	}
}

void SIRScenarios::scenarioForSnapFiles(){
	//Network Generation
//	int numVert = 500;
//	Network n1 = readPolBlogsGML::readSnapNetFile("SF02.txt", numVert);
//	Network n2 = readPolBlogsGML::readSnapNetFile("SF05.txt", numVert);
	int numVert = 1500;
	Network n1 = readNetFile::readSnapNetFile("p21.txt", numVert);
	Network n2 = readNetFile::readSnapNetFile("p29.txt", numVert);
	MultipleNetwork mnet(false);
	mnet.addNetwork(n1);
	mnet.addNetwork(n2);
//	double avgDegreeInter = 1.5;//Weekly
//	double avgDegreeInter = 3;//Strongly
//	double avgDegreeInter = 6;//Strongly For Power Laws
	double avgDegreeInter = 0.5;//Weekly For Power Laws
	double pInter = avgDegreeInter*(numVert + numVert)/(2 * numVert * numVert);
	int v1 = 0;
	int v2 = 0;
	//Forming up Interlayer Connections
	while (v1 < numVert - 1){
		Random r;
		double rand = r.getRandomDouble();
		int incr = 0;
		if(rand < pInter){
			incr = 1;
		}else{
			incr = ceil(log(1-rand)/log(1-pInter));
		}
		while (v1 < numVert){
			if (incr <= numVert - 1 - v2){
				mnet.newInterlayerConnection(global_vertex_id(v1,0),global_vertex_id(v2,1));
				v2 += incr;
				break;
			}else{
				incr -= (numVert - 1 - v2);
				v1++;
				v2 = v1;
			}
		}
	}
	print(mnet);

	//SIR Model Run
	int infectionTimeSpan = 5;//Recovery rate
	double intralayerDiffRate = 0.01;
	double interlayerDiffRate = 0.01;
	cout << "v0: " << mnet.getNetwork(0).getNumVertexes() << endl;
	cout << "e0: " << mnet.getNetwork(0).getNumEdges() << endl;
	cout << "v1: " << mnet.getNetwork(1).getNumVertexes() << endl;
	cout << "e1: " << mnet.getNetwork(1).getNumEdges() << endl;
	cout << "eInter: " << mnet.getNumInterlayerConnections() << endl;
	vector<int> GCCSizeInLayer(mnet.getNumNetworks());
	for (int layerNumber = 0 ; layerNumber < mnet.getNumNetworks() ; layerNumber++){
		//Find the largest connected component size in each layer
		GCCSizeInLayer[layerNumber] = mnet.getNetwork(layerNumber).getGCCSize();
		cout << "gcc" << layerNumber << ": " << GCCSizeInLayer[layerNumber] << endl;
	}
	SIRModel sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
	for(intralayerDiffRate = 0.01 ; intralayerDiffRate <= 1.00 ; intralayerDiffRate += 0.05){
		for(interlayerDiffRate = 0.01 ; interlayerDiffRate <= 1.00 ; interlayerDiffRate += 0.05){
			sirModel.setDiffRates(intralayerDiffRate, interlayerDiffRate);
			cout << "intra: " << intralayerDiffRate << ", inter: " << interlayerDiffRate << endl;
			//Find average of infected nodes in each layer
			SIRScenarios::scFlickrAndBlogCatalogAndSnapBody(sirModel, GCCSizeInLayer);
		}
		ofstream myfile;
		myfile.open ("Sc7-AllS.txt",ios::app);
		myfile << "\n";
		myfile.close();
		myfile.open ("Sc7-AllI.txt",ios::app);
		myfile << "\n";
		myfile.close();
		myfile.open ("Sc7-AllR.txt",ios::app);
		myfile << "\n";
		myfile.close();
		myfile.open ("Sc7-AllSinTime.txt",ios::app);
		myfile << "\n\n";
		myfile.close();
		myfile.open ("Sc7-AllIinTime.txt",ios::app);
		myfile << "\n\n";
		myfile.close();
		myfile.open ("Sc7-AllRinTime.txt",ios::app);
		myfile << "\n\n";
		myfile.close();
		for (int j = 0 ; j <  mnet.getNumNetworks() ; j++){
			myfile.open ("Sc7-"+ std::to_string(j) +"S.txt",ios::app);
			myfile << "\n" ;
			myfile.close();
			myfile.open ("Sc7-"+ std::to_string(j) +"I.txt",ios::app);
			myfile << "\n" ;
			myfile.close();
			myfile.open ("Sc7-"+ std::to_string(j) +"R.txt",ios::app);
			myfile << "\n" ;
			myfile.close();
			myfile.open ("Sc7-"+ std::to_string(j) +"RGccRatio.txt",ios::app);
			myfile << "\n" ;
			myfile.close();
			myfile.open ("Sc7-"+ std::to_string(j) +"SinTime.txt",ios::app);
			myfile << "\n\n" ;
			myfile.close();
			myfile.open ("Sc7-"+ std::to_string(j) +"IinTime.txt",ios::app);
			myfile << "\n\n" ;
			myfile.close();
			myfile.open ("Sc7-"+ std::to_string(j) +"RinTime.txt",ios::app);
			myfile << "\n\n" ;
			myfile.close();
		}
	}
}

void SIRScenarios::scFlickrAndBlogCatalogAndSnapBody(SIRModel sirModel, vector<int> GCCSizeInLayer){
	double avgNumberS = 0;
	vector<double> avgNumberSinLayer = vector<double>(sirModel.getNetwork().getNumNetworks(),0);
	double avgNumberI = 0;
	vector<double> avgNumberIinLayer = vector<double>(sirModel.getNetwork().getNumNetworks(),0);
	double avgNumberR = 0;
	vector<double> avgNumberRinLayer = vector<double>(sirModel.getNetwork().getNumNetworks(),0);

	vector<double> avgRGccRatioinLayer = vector<double>(sirModel.getNetwork().getNumNetworks(),0);

	std::vector<double> totalNumberSInTime;
	std::vector<std::vector<double> > totalNumberSinLayerInTime = vector<vector<double> >(sirModel.getNetwork().getNumNetworks());
	std::vector<double> totalNumberIInTime;
	std::vector<std::vector<double> > totalNumberIinLayerInTime = vector<vector<double> >(sirModel.getNetwork().getNumNetworks());
	std::vector<double> totalNumberRInTime;
	std::vector<std::vector<double> > totalNumberRinLayerInTime = vector<vector<double> >(sirModel.getNetwork().getNumNetworks());

	double numberOfRealizations = 50;
	for (int i = 0 ; i < numberOfRealizations ; i++){
		simulationStats runTimeStat = sirModel.start();
		avgNumberS += runTimeStat.S;
		avgNumberI += runTimeStat.I;
		avgNumberR += runTimeStat.R;
		for (int j = 0 ; j <  avgNumberSinLayer.size() ; j++){
			avgNumberSinLayer[j] += runTimeStat.SinLayer[j];
			avgNumberIinLayer[j] += runTimeStat.IinLayer[j];
			avgNumberRinLayer[j] += runTimeStat.RinLayer[j];
//			avgRGccRatioinLayer[j] += (double)fifthScenarioGetSizeOfLargestInfectedClusterOnLayer(sirModel.mnet, sirModel, j)/(double)runTimeStat.R;
		}
		if(runTimeStat.numberOfSusceptibleinTime.size() > totalNumberSInTime.size()){
			totalNumberSInTime.resize(runTimeStat.numberOfSusceptibleinTime.size(),0);
			totalNumberIInTime.resize(runTimeStat.numberOfInfectedinTime.size(),0);
			totalNumberRInTime.resize(runTimeStat.numberOfRemovedinTime.size(),0);
		}
		std::list<int>::iterator listItNumSuc=runTimeStat.numberOfSusceptibleinTime.begin();
		std::list<int>::iterator listItNumInf=runTimeStat.numberOfInfectedinTime.begin();
		std::list<int>::iterator listItNumRem=runTimeStat.numberOfRemovedinTime.begin();
		for (int j = 0 ; listItNumSuc!=runTimeStat.numberOfSusceptibleinTime.end(); ++listItNumSuc, ++listItNumInf, ++listItNumRem, ++j){
			totalNumberSInTime[j] += *listItNumSuc;
			totalNumberIInTime[j] += *listItNumInf;
			totalNumberRInTime[j] += *listItNumRem;
		}
		for (int j = 0 ; j < runTimeStat.SinLayer.size() ; j++){
			if(runTimeStat.numberOfSusceptibleinTimeinLayer[j].size() > totalNumberSinLayerInTime[j].size()){
				totalNumberSinLayerInTime[j].resize(runTimeStat.numberOfSusceptibleinTimeinLayer[j].size());
				totalNumberIinLayerInTime[j].resize(runTimeStat.numberOfInfectedinTimeinLayer[j].size());
				totalNumberRinLayerInTime[j].resize(runTimeStat.numberOfRemovedinTimeinLayer[j].size());
			}
			std::list<int>::iterator listItNumSucinLayer = runTimeStat.numberOfSusceptibleinTimeinLayer[j].begin();
			std::list<int>::iterator listItNumInfinLayer = runTimeStat.numberOfInfectedinTimeinLayer[j].begin();
			std::list<int>::iterator listItNumReminLayer = runTimeStat.numberOfRemovedinTimeinLayer[j].begin();
			for (int k = 0 ; listItNumSucinLayer != runTimeStat.numberOfSusceptibleinTimeinLayer[j].end() ; ++listItNumSucinLayer, ++listItNumInfinLayer, ++listItNumReminLayer, ++k){
				totalNumberSinLayerInTime[j][k] += *listItNumSucinLayer;
				totalNumberIinLayerInTime[j][k] += *listItNumInfinLayer;
				totalNumberRinLayerInTime[j][k] += *listItNumReminLayer;
			}
		}
	}
	avgNumberS /= numberOfRealizations;
	avgNumberI /= numberOfRealizations;
	avgNumberR /= numberOfRealizations;
	for (int j = 0 ; j <  avgNumberSinLayer.size() ; j++){
		avgNumberSinLayer[j] /= numberOfRealizations;
		avgNumberIinLayer[j] /= numberOfRealizations;
		avgNumberRinLayer[j] /= numberOfRealizations;
		avgRGccRatioinLayer[j] /= numberOfRealizations;
//		cout << "avgRatio " << j << ": " <<  avgRGccRatioinLayer[j] << endl;
	}
	ofstream myfile;
	myfile.open ("Sc7-AllS.txt",ios::app);
	myfile << avgNumberS << "\t" ;
	myfile.close();
	myfile.open ("Sc7-AllI.txt",ios::app);
	myfile << avgNumberI << "\t" ;
	myfile.close();
	myfile.open ("Sc7-AllR.txt",ios::app);
	myfile << avgNumberR << "\t" ;
	myfile.close();
	myfile.open ("Sc7-AllSinTime.txt",ios::app);
	for (int i = 0 ; i < totalNumberSInTime.size() ; i++)
		myfile << totalNumberSInTime[i]/numberOfRealizations << "\t" ;
	myfile << "\n" ;
	myfile.close();
	myfile.open ("Sc7-AllIinTime.txt",ios::app);
	for (int i = 0 ; i < totalNumberIInTime.size() ; i++)
		myfile << totalNumberIInTime[i]/numberOfRealizations << "\t" ;
	myfile << "\n" ;
	myfile.close();
	myfile.open ("Sc7-AllRinTime.txt",ios::app);
	for (int i = 0 ; i < totalNumberRInTime.size() ; i++)
		myfile << totalNumberRInTime[i]/numberOfRealizations << "\t" ;
	myfile << "\n" ;
	myfile.close();
	for (int j = 0 ; j <  avgNumberSinLayer.size() ; j++){
		myfile.open ("Sc7-"+ std::to_string(j) +"S.txt",ios::app);
		myfile << avgNumberSinLayer[j]/(double)GCCSizeInLayer[j] << "\t" ;
		myfile.close();
		myfile.open ("Sc7-"+ std::to_string(j) +"I.txt",ios::app);
		myfile << avgNumberIinLayer[j]/(double)GCCSizeInLayer[j] << "\t" ;
		myfile.close();
		myfile.open ("Sc7-"+ std::to_string(j) +"R.txt",ios::app);
		myfile << avgNumberRinLayer[j]/(double)GCCSizeInLayer[j] << "\t" ;
		myfile.close();
		myfile.open ("Sc7-"+ std::to_string(j) +"RGccRatio.txt",ios::app);
		myfile << avgRGccRatioinLayer[j] << "\t" ;
		myfile.close();
		myfile.open ("Sc7-"+ std::to_string(j) +"SinTime.txt",ios::app);
		for (int k = 0 ; k < totalNumberSinLayerInTime[j].size() ; k++)
			myfile << totalNumberSinLayerInTime[j][k]/numberOfRealizations << "\t" ;
		myfile << "\n" ;
		myfile.close();
		myfile.open ("Sc7-"+ std::to_string(j) +"IinTime.txt",ios::app);
		for (int k = 0 ; k < totalNumberIinLayerInTime[j].size() ; k++)
			myfile << totalNumberIinLayerInTime[j][k]/numberOfRealizations << "\t" ;
		myfile << "\n" ;
		myfile.close();
		myfile.open ("Sc7-"+ std::to_string(j) +"RinTime.txt",ios::app);
		for (int k = 0 ; k < totalNumberRinLayerInTime[j].size() ; k++)
			myfile << totalNumberRinLayerInTime[j][k]/numberOfRealizations << "\t" ;
		myfile << "\n" ;
		myfile.close();
	}
}
