/*
 * MotifCalculator.h
 *
 *  Created on: Dec 2, 2018
 *      Author: ori
 */

#ifndef FEATURES_GPUMOTIFCALCULATOR_H_
#define FEATURES_GPUMOTIFCALCULATOR_H_

#include "FeatureCalculator.h"
#include "MotifUtils.h"
#include <stdlib.h>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <set>
#include <iostream>
#include <stdio.h>




/**
 * The motif calc returns a list for each node counting the motifs in it.
 */
class GPUMotifCalculator: public FeatureCalculator<vector<vector<unsigned int>*>*> {
public:
	GPUMotifCalculator(int level,bool directed);
	virtual vector<vector<unsigned int>*>* Calculate();
	virtual ~GPUMotifCalculator();

	__global__ friend void Motif3Kernel(GPUMotifCalculator* calc);
	__global__ friend void Motif4Kernel(GPUMotifCalculator* calc);

private:
	__device__ void Motif3Subtree(unsigned int node);
	__device__ void Motif4Subtree(unsigned int node);

	void InitFeatureCounters();
	void LoadMotifVariations(int level,bool directed);
	void SetAllMotifs();
	void SetSortedNodes();
	void SetRemovalIndex();
	void CopyAllToDevice(); // Copy all the relevent data to the device.
	virtual void init();
	__device__ static void GroupUpdater(unsigned int group[], int size);
	__device__ static int GetGroupNumber(unsigned int group[], int size);
	__device__ static bool AreNeighbors(unsigned int p, unsigned int q);

    CacheGraph fullGraph;

	//Either 3 or 4
	unsigned int level;
	//The CacheGraph is always directed, so we need to specify the motif variation
	bool directed;
	
	// HOST VARIABLES

	//map the group num to the iso motif
	std::vector<unsigned int>* nodeVariations;

	//list of base motifs
	std::vector<int>* allMotifs;
	//the index in which we remove the node from the graph. Basically, from this index on the node doesen't exist.
	std::vector<unsigned int>* removalIndex;
	//the nodes, sorted in descending order by the degree.
	std::vector<unsigned int>* sortedNodesByDegree;

	//the results, node -> {motif-> motif_count}
	vector<vector<unsigned int>*>* features;

	unsigned int numOfMotifs;
	unsigned int numOfNodes;
	unsigned int numOfEdges;


	// DEVICE VARIABLES

	unsigned int* devicePointerMotifVariations;
	unsigned int* devicePointerRemovalIndex;
	unsigned int* devicePointerSortedNodesByDegree;



	// For the original graph
	int64* deviceOriginalGraphOffsets;
	unsigned int* deviceOriginalGraphNeighbors;

	// For the full graph
	int64* deviceFullGraphOffsets;
	unsigned int* deviceFullGraphNeighbors;

	// Feature array
	unsigned int* deviceFeatures;
};




#endif /* FEATURES_MOTIFCALCULATOR_H_ */
