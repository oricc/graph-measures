#ifndef DISTANCE_UTILS_H_
#define DISTANCE_UTILS_H_
#include "CacheGraph.h"
#include "fiboqueue.h"
#include <list>
#include <vector>
#include <limits.h>

class DistanceUtils
{
public:
	static std::vector<int> BfsSingleSourceShortestPath(const CacheGraph * g, int src);
	static std::vector<float> DijkstraSingleSourceShortestPath(const CacheGraph * g, int src);
};

#endif
