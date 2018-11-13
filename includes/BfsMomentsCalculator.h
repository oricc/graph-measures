#pragma once
#include "NodeFeatureCalculator.h"
#include <vector>
#include <tuple>
#include "CacheGraph.h"

using namespace std;
class BfsMomentsCalculator :
	public NodeFeatureCalculator<vector<tuple<float,float>>>
{
public:
	BfsMomentsCalculator();
	virtual vector<tuple<float,float>> Calculate();

	virtual ~BfsMomentsCalculator();

protected:
	virtual inline bool checkGPUEnabled() {
		return false;
	}
};

