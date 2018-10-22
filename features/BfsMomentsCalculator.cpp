#include "stdafx.h"
#include "BfsMomentsCalculator.h"
#include "DistanceUtils.h"
#include "MathUtils.h"
#include <unordered_map>


BfsMomentsCalculator::BfsMomentsCalculator()
{
}

vector<tuple<float, float>> BfsMomentsCalculator::CalculateCpu()
{

	const int numOfNodes = mGraph->GetNumberOfNodes();
	vector<tuple<float, float>> features(numOfNodes);
	
	for (int i = 0; i < numOfNodes; i++) {

		//calculate BFS distances
		std::vector<int> distances = DistanceUtils::BfsSingleSourceShortestPath(mGraph, i);

		//count the number of times each distance exists
		std::unordered_map<int, int> distCounter;

		for (int j = 0; j < distances.size(); i++) {
			if (distCounter.find(distances[j]) == distCounter.end())
				//distance[j] hasn't been counted before
				distCounter[distances[j]] = 0;
			distCounter[distances[j]] += 1;
		}

		std::vector<float> dists(distCounter.size()), weights(distCounter.size());
		
		for (const auto& n : distCounter) {
			dists.push_back((float) n.first + 1); // the key is the distance, which needs adjustment
			weights.push_back((float)n.second); //the value is the number of times it has been counted
		}
		
		features[i] = std::make_tuple(MathUtils::calculateWeightedAverage(dists, weights),MathUtils::calculateStd(weights));
	}

	return features;
}


BfsMomentsCalculator::~BfsMomentsCalculator()
{
}
