#pragma once
#include "GraphSnapshot.h"
#include <vector>


class DistanceUtils
{
public:
	static std::vector<int> BfsSingleSourceShortestPath(const GraphSnapshot * g, int src);
	static std::vector<float> DijkstraSingleSourceShortestPath(const GraphSnapshot * g, int src);
};

