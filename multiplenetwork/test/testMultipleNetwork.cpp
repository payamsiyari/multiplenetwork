/*
 * testMultipleNetwork.cpp
 *
 * Author: Payam Siyari <payamsiyari@gmail.com>
 * Version: 0.0.1
 */

#import "test.h"
#import "datastructures.h"
#import "exceptions.h"
#import "utils.h"

void testMultipleNetwork() {
	log("**************************************************************");
	log("TESTING MultipleNetwork");
	log("REQUIRES Network class and MultilayerNetwork class having been tested");

	log("Creating an empty multiple network...",false);
	MultipleNetwork mnet;
	log("done!");

	log("Creating and adding three undirected networks and adding them to the multiple network...",false);
	// Network 1
	Network net1(false,false,false);
	vertex_id n1v0 = net1.addVertex();
	vertex_id n1v1 = net1.addVertex();
	vertex_id n1v2 = net1.addVertex();
	// Network 2
	Network net2(false,false,false);
	vertex_id n2v0 = net2.addVertex();
	vertex_id n2v1 = net2.addVertex();
	vertex_id n2v2 = net2.addVertex();
	// Network 3
	Network net3(false,false,false);
	vertex_id n3v0 = net3.addVertex();
	vertex_id n3v1 = net3.addVertex();
	vertex_id n3v2 = net3.addVertex();

	network_id n1 = mnet.addNetwork(net1);
	network_id n2 = mnet.addNetwork(net2);
	network_id n3 = mnet.addNetwork(net3);
	log("done!");

	log("Adding six intralayer edges...",false);
	mnet.getNetwork(n1).addEdge(n1v0,n1v1);
	mnet.getNetwork(n1).addEdge(n1v0,n1v2);
	mnet.getNetwork(n2).addEdge(n2v1,n2v0);
	mnet.getNetwork(n2).addEdge(n2v1,n2v2);
	mnet.getNetwork(n3).addEdge(n3v2,n3v0);
	mnet.getNetwork(n3).addEdge(n3v2,n3v1);
	log("done!");

	log("Trying to add an interlayer edge between two vertexes from the same network (should fail)...",false);
	try {
		mnet.newInterlayerConnection(global_vertex_id(n1v0,n1),global_vertex_id(n1v1,n1));
	}
	catch (OperationNotSupportedException& ex) {
		log("[FAIL] done!");
	}
	log("Adding a sample (correct) interlayer edge...",false);
	mnet.newInterlayerConnection(global_vertex_id(n1v0,n1),global_vertex_id(n2v0,n2));
	log("Trying to add an existing interlayer edge (should fail)...",false);
	try {
		mnet.newInterlayerConnection(global_vertex_id(n1v0,n1),global_vertex_id(n1v1,n1));
	}
	catch (OperationNotSupportedException& ex) {
		log("[FAIL] done!");
	}

	log("Adding five interlayer edges (actually, 4 + the sample just created)...",false);
	mnet.newInterlayerConnection(global_vertex_id(n2v0,n2),global_vertex_id(n3v0,n3));
	mnet.newInterlayerConnection(global_vertex_id(n1v1,n1),global_vertex_id(n3v2,n3));
	mnet.newInterlayerConnection(global_vertex_id(n1v1,n1),global_vertex_id(n2v2,n2));
	mnet.newInterlayerConnection(global_vertex_id(n2v1,n2),global_vertex_id(n3v2,n3));
	if (mnet.getNumInterlayerConnections()!=5) throw FailedUnitTestException("Wrong number of interlayer edges");
	if (mnet.getNumEdges()!=11) throw FailedUnitTestException("Wrong number of edges");
	log("done!");


	log("TEST SUCCESSFULLY COMPLETED (MultipleNetwork class)");

	log("Printing final multiple network information:");
	print(mnet);

}


