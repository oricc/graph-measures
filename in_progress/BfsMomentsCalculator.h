#ifndef BFS_MOMENTS_CALCULATOR_H_
#define BFS_MOMENTS_CALCULATOR_H_

#include "FeatureCalculator.h"
#include <vector>
#include <tuple>
#include "CacheGraph.h"

using namespace std;
class BfsMomentsCalculator :
	public FeatureCalculator<vector<tuple<float,float>>>
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

#endif
