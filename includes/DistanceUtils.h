#pragma once
#include "../includes/CacheGraph.h"
#include <vector>

#define INT_MAX 99999999999999999999999999999999

class DistanceUtils
{
public:
	static std::vector<int> BfsSingleSourceShortestPath(const CacheGraph * g, int src);
	static std::vector<float> DijkstraSingleSourceShortestPath(const CacheGraph * g, int src);
};

