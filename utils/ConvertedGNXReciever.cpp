/*
 * ConvertedGNXReciever.cpp
 *
 *  Created on: Oct 28, 2018
 *      Author: ori
 */

#include "../includes/ConvertedGNXReciever.h"


ConvertedGNXReciever::ConvertedGNXReciever(dict converted_graph) {


	list offsetList = extract<list>(converted_graph['indices']);
	list neighborList = extract<list>(converted_graph['neighbors']);

	this -> offsets = new std::vector<int64>(len(offsetList));
	this -> neighbors = new std::vector<unsigned int>(len(neighborList));

	for(int i=0;i<len(offsetList);++i){
		int64 currentOffset = static_cast<int64>(extract<unsigned int>(offsetList[i])*sizeof(unsigned int));
		this->offsets->push_back(currentOffset);
	}

	for(int i=0;i<len(neighborList);++i){
		unsigned int currentNeighbor = extract<unsigned int>(neighborList[i]);
		this->neighbors->push_back(currentNeighbor);
	}

	this->mGraph = new CacheGraph();
	mGraph->Assign(*offsets,*neighbors);

}

ConvertedGNXReciever::~ConvertedGNXReciever() {
	// TODO Auto-generated destructor stub
	delete offsets;
	delete neighbors;
	delete mGraph;
}


