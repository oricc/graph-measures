#pragma once
#include <vector>

class MathUtils
{
public:
	static float calculateStd(const std::vector<float>& data);
	static float calculateMean(const std::vector<float>& data);
	static float calculateWeightedAverage(const std::vector<float>& data, const std::vector<float>& weights);

};

