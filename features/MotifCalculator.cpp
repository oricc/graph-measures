/*
 * MotifCalculator.cpp
 *
 *  Created on: Dec 2, 2018
 *      Author: ori
 */

#include "MotifCalculator.h"

MotifCalculator::MotifCalculator(int level, bool directed):
directed(directed),nodeVariations(NULL),allMotifs(NULL),removalIndex(NULL),sortedNodesByDegree(NULL)
{
	//check level
	if (level != 3 && level != 4)
		throw invalid_argument("Level must be 3 or 4");
	this->level = level;

	this->features = new std::vector<std::map<unsigned int, unsigned int>*>;
	this->LoadMotifVariations(level, directed);
	this->SetAllMotifs();
	this->SetSortedNodes();
	this->SetRemovalIndex();
	this->InitFeatureCounters();

}

void MotifCalculator::InitFeatureCounters() {
	for (int node = 0; node < mGraph->GetNumberOfNodes(); node++) {
		std::map<unsigned int, unsigned int>* motifCounter = new std::map<
				unsigned int, unsigned int>;
		for (auto motif : this->allMotifs)
			if (motif != -1)
				(*motifCounter)[motif] = 0;

		features->push_back(motifCounter);
	}
}

void MotifCalculator::LoadMotifVariations(int level, bool directed) {
	this->nodeVariations = new std::map<unsigned int,int>();

	string suffix;
	if (directed)
		suffix = "_directed_cpp.txt";
	else
		suffix = "_undirected_cpp.txt";

	string fileName = "motif_variations/" + std::to_string(level) + suffix;
	std::ifstream infile(fileName);
	string a, b;
	while (infile >> a >> b) {
		int x, y;
		try {
			x = stoi(a);
			y = stoi(b);
		} catch (exception &e) {
			y = -1;

		}
		cout << x << ":" << y << endl;

		(*nodeVariations)[x] = y;
	}

}

void MotifCalculator::SetAllMotifs() {
	this->allMotifs = new std::vector<int>();

	for (const auto& x : this->nodeVariations)
		this->allMotifs->push_back(x.second);
}

void MotifCalculator::SetSortedNodes() {
	this->sortedNodesByDegree = new std::vector<unsigned int>();
	this->sortedNodesByDegree = mGraph->SortedNodesByDegree();
}
/**
 * We iterate over the list of sorted nodes.
 * If node p is in index i in the list, it means that it will be removed after the i-th iteration,
 * or conversely that it is considered "not in the graph" from iteration i+1 onwards.
 * This means that any node with a removal index of j is not considered from iteration j+1.
 * (The check is (removalIndex[node] > currentIteration).
 */
void MotifCalculator::SetRemovalIndex() {
	this->removalIndex = new std::vector<unsigned int>();

	for (unsigned int index = 0; index < mGraph->GetNumberOfNodes(); index++) {
		auto node = (*sortedNodesByDegree)[index];
		(*removalIndex)[node] = index;
	}
}

vector<std::map<unsigned int, unsigned int>*>* MotifCalculator::Calculate() {
	for (auto node : this->sortedNodesByDegree) {
		if (this->level == 3)
			Motif3Subtree(node);
		else
			Motif4Subtree(node);
	}

	return this->features;
}

void MotifCalculator::Motif3Subtree(unsigned int node) {
	// Instead of yield call GroupUpdater function
	// Don't forget to check each time that the nodes are in the graph (check removal index).
}

void MotifCalculator::Motif4Subtree(unsigned int node) {
}

void MotifCalculator::GroupUpdater(std::vector<unsigned int> group) {

	unsigned int groupNumber = GetGroupNumber(group);
	int motifNumber = (*(this->nodeVariations))[groupNumber];
	if (motifNumber != -1)
		for (auto node : group)
			(*(*features)[node])[motifNumber]++;

}
int MotifCalculator::GetGroupNumber(std::vector<unsigned int> group) {
	vector<vector<unsigned int> *> * options;
	unsigned int n1, n2;

	vector<bool> edges;

	if (directed) {
		options = permutations(group);

	} else {
		options = combinations(group);
	}

	for (int i = 0; i < options->size(); i++) {
		n1 = options->at(i)->at(0);
		n2 = options->at(i)->at(1);
		edges.push_back(this->mGraph->areNeighbors(n1, n2));
	}
	delete options;
	return bool_vector_to_int(edges);

}

MotifCalculator::~MotifCalculator() {
// TODO Auto-generated destructor stub
}

