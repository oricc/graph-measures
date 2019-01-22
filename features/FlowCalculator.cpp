/*
 * FlowCalculator.cpp
 *
 *  Created on: Jan 22, 2019
 *      Author: ori
 */

#include "../includes/FlowCalculator.h"

FlowCalculator::FlowCalculator(double threshold) :
		numOfNodes(0), threshold(threshold) {

}
void FlowCalculator::init() {
	numOfNodes = mGraph->GetNumberOfNodes();

	mGraph->InverseGraph(inverse);
	mGraph->CureateUndirectedGraph(inverse, undirectedGraph);

	directed_dists.reserve(numOfNodes);

	undirected_dists.reserve(numOfNodes);

	b_u.reserve(numOfNodes);
	features.reserve(numOfNodes);
}

void FlowCalculator::CalcDists() {
	for (unsigned int node = 0; node < numOfNodes; node++) {
		auto directedBfsDist = DistanceUtils::BfsSingleSourceShortestPath(
				mGraph, node);
		directed_dists.push_back(
				new std::vector<unsigned int>(directedBfsDist.begin(),
						directedBfsDist.end()));

		auto undirectedBfsDist = DistanceUtils::BfsSingleSourceShortestPath(
				&undirectedGraph, node);
		undirected_dists.push_back(
				new std::vector<unsigned int>(undirectedBfsDist.begin(),
						undirectedBfsDist.end()));

	}
}

void FlowCalculator::CountReachables() {

	for (unsigned int node = 0; node < numOfNodes; node++) {
		b_u[node] = 0;
		for (unsigned int dest = 0; dest < numOfNodes; dest++) {
			if (undirected_dists[node]->at(dest) > 0)
				b_u[node]++;
		}
	}

}

std::vector<double> FlowCalculator::Calculate() {
	CalcDists();
	CountReachables();

	double max_b_u = (double) (*std::max_element(b_u.begin(), b_u.end()));

	for (unsigned int node = 0; node < numOfNodes; node++) {

		// Check threshold
		if ((b_u[node] / max_b_u) <= threshold) {
			features[node] = 0;
			continue;
		}
		auto udists = undirected_dists[node];
		auto dists = directed_dists[node];

		double sum=0;

		for (unsigned int n = 0; n < numOfNodes; n++){
			if (dists->at(n) == 0) {
				continue;
			}
			sum += (double)udists->at(n) / dists->at(n);
			features[node] = sum / (double) b_u[node];
		}

	}
	return features;

}


FlowCalculator::~FlowCalculator() {
	for (auto p : directed_dists)
		delete p;

	for (auto p : undirected_dists)
		delete p;
}

