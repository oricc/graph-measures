#pragma once
#include "stdafx.h"
#include "FeatureCalculator.h"

/*
	Calculate the average clustering coefficient of a network.
	We assume the network to be UNDIRECTED
*/
class ClusteringCoefficient :
	public FeatureCalculator<float>
{
public:
	ClusteringCoefficient();
	virtual float Calculate();
	
	virtual bool checkGPUEnabled(){
		return false;
	};
	virtual ~ClusteringCoefficient();
private:
	float LocalClusteringCoefficient(unsigned int i, unsigned int begin_offset, unsigned int end_offset, const unsigned int * neighborList);
};

