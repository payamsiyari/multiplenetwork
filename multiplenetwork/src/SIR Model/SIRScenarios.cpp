/*
 * SIRScenarios.cpp
 *
 *  Created on: May 29, 2014
 *      Author: payamsiyari
 */

#include "SIRScenarios.h"

void SIRScenarios::firstScenario(){
	//Network Generation
	const int numLayers = 2;
	int nums[numLayers] = {6000,6000};
	double avgs[numLayers] = {1.5,6};
	double intLayer[1][3];
			intLayer[0][0] = 0;
			intLayer[0][1] = 1;

	//SIR Model Run
	int infectionTimeSpan = 2;//Recovery rate
	double intralayerDiffRate = 0; double interlayerDiffRate = 0;

	intLayer[0][2] = 1.5;//Strongly Connected
	MultipleNetwork mnet = erErModel::generate_ERER_MultipleNet_AvgDegreeBased(numLayers,nums,avgs,intLayer,1);
	print(mnet);
	intralayerDiffRate = 0.9;  interlayerDiffRate = 0.1;//Difference close to 1 case
	SIRModel sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
	firstScenarioRun(sirModel, "s10");
	cout << "s10\n";
	intralayerDiffRate = 0.7;  interlayerDiffRate = 0.2;//Difference close to 1 case
	sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
	firstScenarioRun(sirModel, "s05");
	cout << "s05\n";
	intralayerDiffRate = 0.5;  interlayerDiffRate = 0.5;//Difference close to 1 case
	sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
	firstScenarioRun(sirModel, "s00");
	cout << "s00\n";

	intLayer[0][2] = 0.5;//Weekly Connected
	mnet = erErModel::generate_ERER_MultipleNet_AvgDegreeBased(numLayers,nums,avgs,intLayer,1);
	print(mnet);
	intralayerDiffRate = 0.9;  interlayerDiffRate = 0.1;//Difference close to 1 case
	sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
	firstScenarioRun(sirModel, "w10");
	cout << "w10\n";
	intralayerDiffRate = 0.7;  interlayerDiffRate = 0.2;//Difference close to 1 case
	sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
	firstScenarioRun(sirModel, "w05");
	cout << "w05\n";
	intralayerDiffRate = 0.5;  interlayerDiffRate = 0.5;//Difference close to 1 case
	sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
	firstScenarioRun(sirModel, "w00");
	cout << "w00\n";
}

void SIRScenarios::firstScenarioRun(SIRModel sirModel, string filename){
	vector<vector<int> > avgNumberOfInfectedinLayerPerDiffRate(sirModel.getNetwork().getNumNetworks());
	for(double i = -0.09 ; i <= 0.09 ; i+=0.01){
		cout << "i: " << i << " ";
		sirModel.setDiffRates(sirModel.getIntraDiffRate()+i, sirModel.getInterDiffRate()+i);
		int numberOfRealizations = 100;
		std::vector<int> avgNumberOfInfectedinLayer(sirModel.getNetwork().getNumNetworks());
		for (int i = 0 ; i < numberOfRealizations ; i++){
			simulationStats runTimeStat = sirModel.start();
			for(int j = 0 ; j < avgNumberOfInfectedinLayer.size() ; j++){
				avgNumberOfInfectedinLayer[j] += runTimeStat.RinLayer[j];
			}
		}
		for(int j = 0 ; j < avgNumberOfInfectedinLayer.size() ; j++){
			avgNumberOfInfectedinLayer[j] /= numberOfRealizations;
		}
		for (int j = 0 ; j < sirModel.getNetwork().getNumNetworks() ; j++){
			avgNumberOfInfectedinLayerPerDiffRate[j].push_back(avgNumberOfInfectedinLayer[j]);
		}
	}
	cout << endl;
	ofstream myfile;
	myfile.open ("Sc1-" + filename + ".txt");
	for (int i = 0 ; i < sirModel.getNetwork().getNumNetworks() ; i++){
		for (int j = 0 ; j < avgNumberOfInfectedinLayerPerDiffRate[i].size() ; j++)
			myfile << avgNumberOfInfectedinLayerPerDiffRate[i][j] << "\t";
		myfile << "\n";
	}
	for (int j = 0 ; j < avgNumberOfInfectedinLayerPerDiffRate[0].size() ; j++){
		int tmpAvg = 0;
		for (int i = 0 ; i < sirModel.getNetwork().getNumNetworks() ; i++)
			tmpAvg += avgNumberOfInfectedinLayerPerDiffRate[i][j];
		myfile << tmpAvg << "\t";
	}
	myfile << "\n";
	myfile.close();
}

void SIRScenarios::secondScenario(){
	//Network Generation
	const int numLayers = 2;
	int nums[numLayers] = {6000,6000};
	double avgs[numLayers] = {1.5,6};
	double intLayer[1][3];
			intLayer[0][0] = 0;
			intLayer[0][1] = 1;

	//SIR Model Run
	int infectionTimeSpan = 2;//Recovery rate
	double intralayerDiffRate; double interlayerDiffRate;

	intLayer[0][2] = 1.5;//Strongly Connected
	MultipleNetwork mnet = erErModel::generate_ERER_MultipleNet_AvgDegreeBased(numLayers,nums,avgs,intLayer,1);
	print(mnet);
	intralayerDiffRate = 0.9;  interlayerDiffRate = 0.1;//Difference close to 1 case
	SIRModel sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
	secondScenarioRun(sirModel,"s10");
	cout << "s10" << endl;
	intralayerDiffRate = 0.7;  interlayerDiffRate = 0.2;//Difference close to 0.5 case
	sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
	secondScenarioRun(sirModel,"s05");
	cout << "s05" << endl;
	intralayerDiffRate = 0.5;  interlayerDiffRate = 0.5;//Difference close to 0 case
	sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
	secondScenarioRun(sirModel,"s00");
	cout << "s00" << endl;

	intLayer[0][2] = 0.5;//Weekly Connected
	mnet = erErModel::generate_ERER_MultipleNet_AvgDegreeBased(numLayers,nums,avgs,intLayer,1);
	print(mnet);
	intralayerDiffRate = 0.9;  interlayerDiffRate = 0.1;//Difference close to 1 case
	sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
	secondScenarioRun(sirModel,"w10");
	cout << "w10" << endl;
	intralayerDiffRate = 0.7;  interlayerDiffRate = 0.2;//Difference close to 0.5 case
	sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
	secondScenarioRun(sirModel,"w05");
	cout << "w05" << endl;
	intralayerDiffRate = 0.5;  interlayerDiffRate = 0.5;//Difference close to 0 case
	sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
	secondScenarioRun(sirModel,"w00");
	cout << "w00" << endl;
}

void SIRScenarios::secondScenarioRun(SIRModel sirModel, std::string filename){
	std::vector<int> totalNumberS; std::vector<int> varianceNumberS;
	std::vector<std::vector<int> > totalNumberSinLayer = vector<vector<int> >(sirModel.getNetwork().getNumNetworks());
	std::vector<std::vector<int> > varianceNumberSinLayer = vector<vector<int> >(sirModel.getNetwork().getNumNetworks());
	std::vector<int> totalNumberI; std::vector<int> varianceNumberI;
	std::vector<std::vector<int> > totalNumberIinLayer = vector<vector<int> >(sirModel.getNetwork().getNumNetworks());
	std::vector<std::vector<int> > varianceNumberIinLayer = vector<vector<int> >(sirModel.getNetwork().getNumNetworks());
	std::vector<int> totalNumberR; std::vector<int> varianceNumberR;
	std::vector<std::vector<int> > totalNumberRinLayer = vector<vector<int> >(sirModel.getNetwork().getNumNetworks());
	std::vector<std::vector<int> > varianceNumberRinLayer = vector<vector<int> >(sirModel.getNetwork().getNumNetworks());
//	std::vector<int> timesPresent;
	int numberOfRealizations = 100;
	for (int i = 0 ; i < numberOfRealizations ; i++){
		simulationStats runTimeStat = sirModel.start();
		if(runTimeStat.numberOfSusceptibleinTime.size() > totalNumberS.size()){
			totalNumberS.resize(runTimeStat.numberOfSusceptibleinTime.size(),0);
			varianceNumberS.resize(runTimeStat.numberOfSusceptibleinTime.size(),0);
			totalNumberI.resize(runTimeStat.numberOfInfectedinTime.size(),0);
			varianceNumberI.resize(runTimeStat.numberOfInfectedinTime.size(),0);
			totalNumberR.resize(runTimeStat.numberOfRemovedinTime.size(),0);
			varianceNumberR.resize(runTimeStat.numberOfRemovedinTime.size(),0);
//			timesPresent.resize(runTimeStat.timeStamp.size(),0);
		}
		std::list<int>::iterator listItNumSuc=runTimeStat.numberOfSusceptibleinTime.begin();
		std::list<int>::iterator listItNumInf=runTimeStat.numberOfInfectedinTime.begin();
		std::list<int>::iterator listItNumRem=runTimeStat.numberOfRemovedinTime.begin();
//		std::list<int>::iterator listItTime=runTimeStat.timeStamp.begin();
		for (int j = 0 ; listItNumSuc!=runTimeStat.numberOfSusceptibleinTime.end(); ++listItNumSuc, ++listItNumInf, ++listItNumRem, ++j){
			totalNumberS[j] += *listItNumSuc;
			varianceNumberS[j] += (int)pow((double)*listItNumSuc,2);
			totalNumberI[j] += *listItNumInf;
			varianceNumberI[j] += (int)pow((double)*listItNumInf,2);
			totalNumberR[j] += *listItNumRem;
			varianceNumberR[j] += (int)pow((double)*listItNumRem,2);
//			timesPresent[j]++;
		}
		for (int j = 0 ; j < runTimeStat.SinLayer.size() ; j++){
			if(runTimeStat.numberOfSusceptibleinTimeinLayer[j].size() > totalNumberSinLayer[j].size()){
				totalNumberSinLayer[j].resize(runTimeStat.numberOfSusceptibleinTimeinLayer[j].size());
				varianceNumberSinLayer[j].resize(runTimeStat.numberOfSusceptibleinTimeinLayer[j].size());
				totalNumberIinLayer[j].resize(runTimeStat.numberOfInfectedinTimeinLayer[j].size());
				varianceNumberIinLayer[j].resize(runTimeStat.numberOfInfectedinTimeinLayer[j].size());
				totalNumberRinLayer[j].resize(runTimeStat.numberOfRemovedinTimeinLayer[j].size());
				varianceNumberRinLayer[j].resize(runTimeStat.numberOfRemovedinTimeinLayer[j].size());
			}
			std::list<int>::iterator listItNumSucinLayer = runTimeStat.numberOfSusceptibleinTimeinLayer[j].begin();
			std::list<int>::iterator listItNumInfinLayer = runTimeStat.numberOfInfectedinTimeinLayer[j].begin();
			std::list<int>::iterator listItNumReminLayer = runTimeStat.numberOfRemovedinTimeinLayer[j].begin();
			for (int k = 0 ; listItNumSucinLayer != runTimeStat.numberOfSusceptibleinTimeinLayer[j].end() ; ++listItNumSucinLayer, ++listItNumInfinLayer, ++listItNumReminLayer, ++k){
				totalNumberSinLayer[j][k] += *listItNumSucinLayer;
				varianceNumberSinLayer[j][k] += (int)pow((double)*listItNumSucinLayer,2);
				totalNumberIinLayer[j][k] += *listItNumInfinLayer;
				varianceNumberIinLayer[j][k] += (int)pow((double)*listItNumInfinLayer,2);
				totalNumberRinLayer[j][k] += *listItNumReminLayer;
				varianceNumberRinLayer[j][k] += (int)pow((double)*listItNumReminLayer,2);
			}
		}
	}

	ofstream myfile;
	myfile.open ("Sc2-" + filename + ".txt");
//	myfile << "S\n" ;
	for (int i = 0 ; i < totalNumberS.size() ; i++)
		myfile << totalNumberS[i]/numberOfRealizations << "\t" ;
	myfile << "\n";
//	myfile << "I\n" ;
	for (int i = 0 ; i < totalNumberI.size() ; i++)
			myfile << totalNumberI[i]/numberOfRealizations << "\t" ;
	myfile << "\n";
//	myfile << "R\n" ;
	for (int i = 0 ; i < totalNumberR.size() ; i++)
			myfile << totalNumberR[i]/numberOfRealizations << "\t" ;
	myfile << "\n";
	myfile << "variance\n";
//	myfile << "S\n" ;
	for (int i = 0 ; i < totalNumberS.size() ; i++)
		myfile << sqrt(double(varianceNumberS[i]-pow((double)totalNumberS[i],2)/numberOfRealizations)/numberOfRealizations) << "\t" ;
	myfile << "\n";
//	myfile << "I\n" ;
	for (int i = 0 ; i < totalNumberI.size() ; i++)
		myfile << sqrt(double(varianceNumberI[i]-pow((double)totalNumberI[i],2)/numberOfRealizations)/numberOfRealizations) << "\t" ;
	myfile << "\n";
//	myfile << "R\n" ;
	for (int i = 0 ; i < totalNumberR.size() ; i++)
		myfile << sqrt(double(varianceNumberR[i]-pow((double)totalNumberR[i],2)/numberOfRealizations)/numberOfRealizations) << "\t" ;
	myfile << "\n";
////		myfile << "Time\n" ;
//	for (int i = 0 ; i < totalNumberS.size() ; i++)
//				myfile << i << "\t" ;
//	myfile << "\n" ;

	for (int i = 0 ; i < totalNumberSinLayer.size() ; i++){
		myfile << "layer " << i << "\n" ;
		for (int j = 0 ; j < totalNumberSinLayer[i].size() ; j++)
			myfile << totalNumberSinLayer[i][j]/numberOfRealizations << "\t" ;
		myfile << "\n";
		for (int j = 0 ; j < totalNumberIinLayer[i].size() ; j++)
			myfile << totalNumberIinLayer[i][j]/numberOfRealizations << "\t" ;
		myfile << "\n";
		for (int j = 0 ; j < totalNumberRinLayer[i].size() ; j++)
			myfile << totalNumberRinLayer[i][j]/numberOfRealizations << "\t" ;
		myfile << "\n";
		myfile << "variance\n";
		for (int j = 0 ; j < totalNumberSinLayer[i].size() ; j++)
			myfile << sqrt(double(varianceNumberSinLayer[i][j]-pow((double)totalNumberSinLayer[i][j],2)/numberOfRealizations)/numberOfRealizations) << "\t" ;
		myfile << "\n";
		for (int j = 0 ; j < totalNumberIinLayer[i].size() ; j++)
			myfile << sqrt(double(varianceNumberIinLayer[i][j]-pow((double)totalNumberIinLayer[i][j],2)/numberOfRealizations)/numberOfRealizations) << "\t" ;
		myfile << "\n";
		for (int j = 0 ; j < totalNumberRinLayer[i].size() ; j++)
			myfile << sqrt(double(varianceNumberRinLayer[i][j]-pow((double)totalNumberRinLayer[i][j],2)/numberOfRealizations)/numberOfRealizations) << "\t" ;
		myfile << "\n";
	}

	myfile.close();
}

void SIRScenarios::thirdScenario(){
}

void SIRScenarios::fourthScenario(){
	//Network Generation
	const double WEEK = 0.1;
	const double STRONG = 1.5;
	const int numLayers = 2;
	int nums[numLayers] = {10000,10000};
	double avgs[numLayers] = {1.5,6};
	double interLayerInfo[1][3];
			interLayerInfo[0][0] = 0;
			interLayerInfo[0][1] = 1;

	//SIR Model Run
	int infectionTimeSpan = 5;//Recovery rate
	for (int counter = 0 ; counter < 2 ; counter++){
		if(counter == 0){
			interLayerInfo[0][2] = WEEK;
		}
		if(counter == 1){
			interLayerInfo[0][2] = STRONG;
		}
		MultipleNetwork mnet = erErModel::generate_ERER_MultipleNet_AvgDegreeBased(numLayers,nums,avgs,interLayerInfo,1);
		print(mnet);
		vector<int> GCCSizeInLayer(mnet.getNumNetworks());
		for (int layerNumber = 0 ; layerNumber < mnet.getNumNetworks() ; layerNumber++){
			//Find the largest connected component size in each layer
			GCCSizeInLayer[layerNumber] = mnet.getNetwork(layerNumber).getGCCSize();
		}
		for(double intralayerDiffRate = 0.01 ; intralayerDiffRate <= 1.00 ; intralayerDiffRate += 0.05){
			for(double interlayerDiffRate = 0.01 ; interlayerDiffRate <= 1.00 ; interlayerDiffRate += 0.05){
				cout << "intra: " << intralayerDiffRate << ", inter: " << interlayerDiffRate << endl;
				SIRModel sirModel = SIRModel(mnet, intralayerDiffRate, interlayerDiffRate, infectionTimeSpan);
				//Find average of infected nodes in each layer
				if(counter == 0)
					SIRScenarios::fourthScenarioRun(sirModel, "week", GCCSizeInLayer);
				if(counter == 1)
					SIRScenarios::fourthScenarioRun(sirModel, "strong", GCCSizeInLayer);
			}
			ofstream myfile;
			string filename = "";
			if(counter == 0){
				filename = "week";
			}
			if(counter == 1){
				filename = "strong";
			}
			myfile.open ("Sc4-"+filename+"AllS.txt",ios::app);
			myfile << "\n";
			myfile.close();
			myfile.open ("Sc4-"+filename+"AllI.txt",ios::app);
			myfile << "\n";
			myfile.close();
			myfile.open ("Sc4-"+filename+"AllR.txt",ios::app);
			myfile << "\n";
			myfile.close();
			for (int j = 0 ; j <  mnet.getNumNetworks() ; j++){
				myfile.open ("Sc4-"+filename+ std::to_string(j) +"S.txt",ios::app);
				myfile << "\n" ;
				myfile.close();
				myfile.open ("Sc4-"+filename+ std::to_string(j) +"I.txt",ios::app);
				myfile << "\n" ;
				myfile.close();
				myfile.open ("Sc4-"+filename+ std::to_string(j) +"R.txt",ios::app);
				myfile << "\n" ;
				myfile.close();
			}
		}
	}
}

void SIRScenarios::fourthScenarioRun(SIRModel sirModel, string filename, vector<int> GCCSizeInLayer){
	double avgNumberS = 0;
	vector<double> avgNumberSinLayer = vector<double>(sirModel.getNetwork().getNumNetworks(),0);
	double avgNumberI = 0;
	vector<double> avgNumberIinLayer = vector<double>(sirModel.getNetwork().getNumNetworks(),0);
	double avgNumberR = 0;
	vector<double> avgNumberRinLayer = vector<double>(sirModel.getNetwork().getNumNetworks(),0);
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
		}
	}
	avgNumberS /= numberOfRealizations;
	avgNumberI /= numberOfRealizations;
	avgNumberR /= numberOfRealizations;
	for (int j = 0 ; j <  avgNumberSinLayer.size() ; j++){
		avgNumberSinLayer[j] /= numberOfRealizations;
		avgNumberIinLayer[j] /= numberOfRealizations;
		avgNumberRinLayer[j] /= numberOfRealizations;
	}
	ofstream myfile;
	myfile.open ("Sc4-"+filename+"AllS.txt",ios::app);
	myfile << avgNumberS << "\t" ;
	myfile.close();
	myfile.open ("Sc4-"+filename+"AllI.txt",ios::app);
	myfile << avgNumberI << "\t" ;
	myfile.close();
	myfile.open ("Sc4-"+filename+"AllR.txt",ios::app);
	myfile << avgNumberR << "\t" ;
	myfile.close();
	for (int j = 0 ; j <  avgNumberSinLayer.size() ; j++){
		myfile.open ("Sc4-"+filename+ std::to_string(j) +"S.txt",ios::app);
		myfile << avgNumberSinLayer[j]/(double)GCCSizeInLayer[j] << "\t" ;
		myfile.close();
		myfile.open ("Sc4-"+filename+ std::to_string(j) +"I.txt",ios::app);
		myfile << avgNumberIinLayer[j]/(double)GCCSizeInLayer[j] << "\t" ;
		myfile.close();
		myfile.open ("Sc4-"+filename+ std::to_string(j) +"R.txt",ios::app);
		myfile << avgNumberRinLayer[j]/(double)GCCSizeInLayer[j] << "\t" ;
		myfile.close();
	}
}
