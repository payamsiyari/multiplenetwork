/**
 * @class MultipleNetwork
 *
 * @author Matteo Magnani <matteo.magnani@it.uu.se>
 * @author Payam Siyari <payamsiyari@gmail.com>
 * @date May 2014
 * @version 0.1.0
 *
 * Data structure containing several interdependent networks. In this version the
 * only kind of interdependency is node correspondence: every vertex in one of the networks
 * has a global identifier, and different nodes may share the same global identifier.
 *
 */

#include "datastructures.h"
#include "exceptions.h"
#include "utils.h"
#include <iostream>
#include <sstream>

MultipleNetwork::MultipleNetwork() {
	MultipleNetwork(false);
}

MultipleNetwork::MultipleNetwork(bool directed) {
	is_directed = directed;
}

MultipleNetwork::~MultipleNetwork() {
	// TODO
}

interlayer_edge_id MultipleNetwork::newInterlayerConnection(global_vertex_id v1, global_vertex_id v2){
//	if (!containsVertex(v1)) throw ElementNotFoundException("Vertex " + std::to_string(v1.vid));
//	if (!containsVertex(v2)) throw ElementNotFoundException("Vertex " + std::to_string(v2.vid));
	if(v1.network == v2.network) throw OperationNotSupportedException("Vertexes lie on the same network");
	if (out_interlayer_edges[v1].count(v2)>0) throw DuplicateElementException("Interlayer edge (" + std::to_string(v1.vid) + "," + std::to_string(v2.vid) +  ") already exists");
	out_interlayer_edges[v1].insert(v2);
	in_interlayer_edges[v2].insert(v1);
	if (!isDirected() /*&& v1!=v2*/) {
		out_interlayer_edges[v2].insert(v1);
		in_interlayer_edges[v1].insert(v2);
	}
	num_interlayer_connections++;
	return interlayer_edge_id(v1,v2,isDirected());
}

void MultipleNetwork::getInterlayerConnections(std::set<interlayer_edge_id>& interlayer_connections) const{
	std::map<global_vertex_id,std::set<global_vertex_id> >::const_iterator from_edge_iterator;
	std::set<global_vertex_id>::iterator to_edge_iterator;
	for (from_edge_iterator=out_interlayer_edges.begin(); from_edge_iterator!=out_interlayer_edges.end(); ++from_edge_iterator) {
		global_vertex_id from = from_edge_iterator->first;
		for (to_edge_iterator=from_edge_iterator->second.begin(); to_edge_iterator!=from_edge_iterator->second.end(); ++to_edge_iterator) {
			global_vertex_id to = (*to_edge_iterator);
			interlayer_connections.insert(interlayer_edge_id(from,to,isDirected()));
		}
	}
}

long MultipleNetwork::getNumEdges() const{
	return num_interlayer_connections + MultilayerNetwork::getNumEdges();
}

long MultipleNetwork::getNumInterlayerConnections() const{
	return num_interlayer_connections;
}

bool MultipleNetwork::isDirected() const  {
	return is_directed;
}

void print(MultipleNetwork& mnet) {
	std::cout << "*MULTIPLE NETWORK*\n";
	std::cout << (mnet.isDirected()?"DIRECTED":"UNDIRECTED") << "\n";
	std::cout << "Number of vertexes: " << mnet.getNumVertexes() << "\n";
	std::cout << "Number of edges (intralayer + inerlayer edges): " << mnet.getNumEdges() << "\n";
	std::cout << "Number of interlayer edges: " << mnet.getNumInterlayerConnections() << "\n";
}

