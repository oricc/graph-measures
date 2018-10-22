#pragma once
#include "NodeFeatureCalculator.h"
#include <vector>
#include <tuple>
#include "GraphSnapshot.h"

using namespace std;
class BfsMomentsCalculator :
	public NodeFeatureCalculator<vector<tuple<float,float>>>
{
public:
	BfsMomentsCalculator();
	virtual vector<tuple<float,float>> CalculateCpu();

	virtual ~BfsMomentsCalculator();

protected:
	virtual inline bool checkGPUEnabled() {
		return false;
	}
};

