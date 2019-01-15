/*
 * AttractionBasinCalculator.cpp
 *
 *  Created on: Jan 14, 2019
 *      Author: ori
 */

#include "../includes/AttractionBasinCalculator.h"

AttractionBasinCalculator::AttractionBasinCalculator(int alpha) :
		alpha(alpha), ab_in_dist(NULL), ab_out_dist(NULL), average_out_per_dist(
		NULL), average_in_per_dist(NULL), features(NULL) {
}
AttractionBasinCalculator::AttractionBasinCalculator():AttractionBasinCalculator(2){}

std::vector<double>* AttractionBasinCalculator::Calculate() {
	this->calc_attraction_basin_dists();
	this->calc_average_per_dist();
	unsigned int numOfNodes = mGraph->GetNumberOfNodes();

	for (unsigned int node = 0; node < numOfNodes; node++) {
		auto out_dist = ab_out_dist->at(node);
		auto in_dist = ab_in_dist->at(node);

		(*features)[node] = -1;

		double numerator = 0, denominator = 0;
		for (auto& x : *out_dist) {
			auto dist = x.first;
			auto occurances = x.second;

			denominator += (occurances/average_out_per_dist->at(dist)) * pow(alpha,(-dist));
		} //end summing loop

		if(denominator != 0){
			for (auto& x : *in_dist) {
				auto dist = x.first;
				auto occurances = x.second;

				denominator += (occurances/average_in_per_dist->at(dist)) * pow(alpha,(-dist));
			} //end summing loop
			(*features)[node] = numerator/denominator;
		} //end if
	}

	return this->features;
}

void AttractionBasinCalculator::calc_attraction_basin_dists() {

	unsigned int numOfNodes = mGraph->GetNumberOfNodes();
	this->ab_in_dist = new std::vector<std::map<unsigned int, unsigned int>*>(
			numOfNodes);
	this->ab_out_dist = new std::vector<std::map<unsigned int, unsigned int>*>(
			numOfNodes);

	// Build a distance matrix
	std::vector<std::vector<unsigned int>*> dists;
	dists.reserve(numOfNodes);
	for (unsigned int node = 0; node < numOfNodes; node++) {
		dists[node] = new std::vector<unsigned int>();
		dists[node] = DistanceUtils::BfsSingleSourceShortestPath(mGraph, node);
		(*ab_out_dist)[node] = new std::map<unsigned int, unsigned int>();
		(*ab_in_dist)[node] = new std::map<unsigned int, unsigned int>();
	}

	for (unsigned int src = 0; src < numOfNodes; src++) {
		for (unsigned int dest = 0; dest < numOfNodes; dest++) {
			unsigned int d = dists.at(src)->at(dest);
			if (d > 0) {
				(*(*ab_out_dist)[src])[d] =
						(ab_out_dist->at(src)->find(d)
								!= ab_out_dist->at(src)->end()) ?
								ab_out_dist->at(src)->at(d) + 1 : 1;
				(*(*ab_in_dist)[dest])[d] =
						(ab_in_dist->at(dest)->find(d)
								!= ab_in_dist->at(dest)->end()) ?
								ab_in_dist[dest][d] + 1 : 1;

			} // end if
		}  // end dest loop
	} //end src loop

	for(auto& p:dists)
		delete p;
}

void AttractionBasinCalculator::calc_average_per_dist() {
	average_in_per_dist = new std::map<unsigned int, double>();
	average_out_per_dist = new std::map<unsigned int, double>();

	unsigned int numOfNodes = mGraph->GetNumberOfNodes();
	for (unsigned int src = 0; src < numOfNodes; src++) {

		// Unify the in distance counters
		auto counter = ab_in_dist->at(src);
		for (auto& x : *counter) {
			auto dist = x.first;
			auto occurances = x.second;
			(*average_in_per_dist)[dist] =
					(average_in_per_dist->find(dist)
							!= average_in_per_dist->end()) ?
							average_in_per_dist->at(dist) + 1 : 1;
		}

		// Unify the out distances
		counter = ab_out_dist->at(src);
		for (auto& x : *counter) {
			auto dist = x.first;
			auto occurances = x.second;
			(*average_out_per_dist)[dist] =
					(average_out_per_dist->find(dist)
							!= average_out_per_dist->end()) ?
							average_out_per_dist->at(dist) + 1 : 1;
		}

	} // End src loop

	for (auto& d : *average_out_per_dist) {
		(*average_out_per_dist)[d.first] = d.second / (double) numOfNodes;
	}
	for (auto& d : *average_in_per_dist) {
		(*average_in_per_dist[d.first]) = d.second / (double) numOfNodes;
	}

}

AttractionBasinCalculator::~AttractionBasinCalculator() {
	// TODO Auto-generated destructor stub

	for (int i = 0; i < this->ab_in_dist->size(); i++)
		delete ab_in_dist->at(i);

	delete ab_in_dist;

	for (int i = 0; i < this->ab_out_dist->size(); i++)
		delete ab_out_dist->at(i);
	delete ab_out_dist;

	delete average_out_per_dist;
	delete average_in_per_dist;

}
