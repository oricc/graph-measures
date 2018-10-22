#pragma once

#include <vector>
#include "GraphSnapshot.h"

using namespace std;

template<typename T> class FeatureCalculator
{
public:
	FeatureCalculator();
	virtual T CalculateCpu() = 0;
	virtual ~FeatureCalculator();
	GraphSnapshot* getGraph();
	void setGraph(GraphSnapshot* graph);
private:
	void init();

protected:
	virtual bool checkGPUEnabled() = 0;
	GraphSnapshot* mGraph;

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
void FeatureCalculator<T>::setGraph(GraphSnapshot* graph)
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
GraphSnapshot* FeatureCalculator<T>::getGraph()
{
	return this->mGraph;
}


