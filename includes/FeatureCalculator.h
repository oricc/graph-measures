#pragma once

#include <vector>
#include "stdafx.h"
#include "CacheGraph.h"

using namespace std;

template<typename T> class FeatureCalculator
{
public:
	FeatureCalculator();
	virtual T CalculateCpu() = 0;
	virtual ~FeatureCalculator();
	CacheGraph* getGraph();
	void setGraph(CacheGraph* graph);
private:
	void init();

protected:
	virtual bool checkGPUEnabled() = 0;
	CacheGraph* mGraph;

private:
	bool isGPUEnabled;
};

template<class T>
FeatureCalculator<T>::FeatureCalculator():
	mGraph(NULL)
{
	init();
}

template<typename T>
void FeatureCalculator<T>::setGraph(CacheGraph* graph)
{
	this->mGraph = graph;
};

template<typename T>
void FeatureCalculator<T>::init()
{
	isGPUEnabled = checkGPUEnabled();
}

template<class T>
FeatureCalculator<T>::~FeatureCalculator()
{
}

template<typename T>
CacheGraph* FeatureCalculator<T>::getGraph()
{
	return this->mGraph;
}


