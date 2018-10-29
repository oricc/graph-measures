#pragma once

#include <vector>
#include "stdafx.h"
#include "CacheGraph.h"

using namespace std;

template<typename T> class FeatureCalculator
{
public:
	FeatureCalculator();
	virtual T Calculate() = 0;
	virtual ~FeatureCalculator();
	const CacheGraph* getGraph();
	void setGraph(const CacheGraph* graph);
private:
	void init();

protected:
	virtual bool checkGPUEnabled() = 0;
	const CacheGraph* mGraph;

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
void FeatureCalculator<T>::setGraph(const CacheGraph* graph)
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
const CacheGraph* FeatureCalculator<T>::getGraph()
{
	return this->mGraph;
}


