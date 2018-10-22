#include "stdafx.h"
#include "MathUtils.h"
#include <stdexcept>



float MathUtils::calculateStd(const std::vector<float>& data)
{
	float standartDeviation = 0.0f;
	int len = data.size();
	float mean = calculateMean(data);
	for (int i = 0; i < len; i++) 
		standartDeviation += pow(data[i] - mean, 2);
	
	standartDeviation = sqrt(standartDeviation / len);

	return standartDeviation;
}

float MathUtils::calculateMean(const std::vector<float>& data)
{
	int len = data.size();
	float sum = 0.0f;
	for (int i = 0; i < len; i++) {
		sum += data[i];
	}
	return sum/len;
}

float MathUtils::calculateWeightedAverage(const std::vector<float>& data, const std::vector<float>& weights)
{
	int lenData = data.size();
	int lenWeights = weights.size();

	if (lenData != lenWeights)
		throw std::length_error("Data and weights must have the same size");

	float sum = 0.0f;
	for (int i = 0; i < lenData; i++)
		sum += data[i] * weights[i];
	
	return sum;
}
