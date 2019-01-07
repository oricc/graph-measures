/*
 * GPUMotifCalculator.cpp
 *
 *  Created on: Dec 2, 2018
 *      Author: ori
 */

#include "../includes/GPUMotifCalculator.h"
#include "../includes/MotifVariationConstants.h"

void GPUMotifCalculator::init() {
	CacheGraph inverse(true);
	mGraph->InverseGraph(inverse);
	mGraph->CureateUndirectedGraph(inverse, fullGraph);

	//std::cout << "Load variations" << std::endl;
	this->LoadMotifVariations(level, directed);
	//std::cout << "All motifs" << std::endl;
	this->SetAllMotifs();
	//std::cout << "Sorted Nodes" << std::endl;
	this->SetSortedNodes();
	//std::cout << "Removal Index" << std::endl;
	this->SetRemovalIndex();
	//std::cout << "Feature counters" << std::endl;
	this->InitFeatureCounters();

	this->CopyAllToDevice();
	//std::cout << "Done" << std::endl;
	//std::cout << this->removalIndex->size() << std::endl;
}

GPUMotifCalculator::GPUMotifCalculator(int level, bool directed) :
		directed(directed), nodeVariations(NULL), allMotifs(NULL), removalIndex(
		NULL), sortedNodesByDegree(NULL), fullGraph(false),numOfMotifs(0),deviceFeatures(NULL) {
	//check level
	if (level != 3 && level != 4)
		throw invalid_argument("Level must be 3 or 4");
	this->level = level;
	this->features = new std::vector<vector<unsigned int> *>;
}

void GPUMotifCalculator::InitFeatureCounters() {
	for (int node = 0; node < mGraph->GetNumberOfNodes(); node++) {
		vector<unsigned int> *motifCounter = new vector<unsigned int>;
		std::set<unsigned int> s(this->allMotifs->begin(),
				this->allMotifs->end());
		for (auto motif : s)
			if (motif != -1)
				//				(*motifCounter)[motif] = 0;
				motifCounter->push_back(0);

		features->push_back(motifCounter);
	}
	delete this->allMotifs;
}

void GPUMotifCalculator::LoadMotifVariations(int level, bool directed) {

	//
	//	string suffix;
	//	if (directed)
	//		suffix = "_directed_cpp.txt";
	//	else
	//		suffix = "_undirected_cpp.txt";
	//
	//	string fileName = MOTIF_VARIATIONS_PATH + "/" + std::to_string(level)
	//			+ suffix;
	//	std::ifstream infile(fileName);
	const char* motifVariations[4] = { undirected3, directed3, undirected4,
			directed4 };
	const int numOfMotifs[4] = { 8, 64, 64, 4096 };

	int variationIndex = 2 * (level - 3) + (directed ? 1 : 0);
	this->numOfMotifs = numOfMotifs[variationIndex];
	this->nodeVariations = new std::vector<unsigned int>(numOfMotifs);
	std::istringstream f(motifVariations[variationIndex]);
	std::string line;
	std::string a, b;
	while (getline(f, line)) {
		int x, y;
		int n = line.find(" ");
		a = line.substr(0, n);
		b = line.substr(n);
		try {
			x = stoi(a);
			y = stoi(b);
		} catch (exception &e) {
			y = -1;
		}
		//		cout << line << endl;
		//		cout << x << ":" << y << endl;

		(*nodeVariations)[x] = y;
	}
}

void GPUMotifCalculator::SetAllMotifs() {
	this->allMotifs = new std::vector<int>();

	for (const auto &x : *(this->nodeVariations))
		this->allMotifs->push_back(x);
}

void GPUMotifCalculator::SetSortedNodes() {
	this->sortedNodesByDegree = mGraph->SortedNodesByDegree();
}
/**
 * We iterate over the list of sorted nodes.
 * If node p is in index i in the list, it means that it will be removed after the i-th iteration,
 * or conversely that it is considered "not in the graph" from iteration i+1 onwards.
 * This means that any node with a removal index of j is not considered from iteration j+1.
 * (The check is (removalIndex[node] > currentIteration).
 */
void GPUMotifCalculator::SetRemovalIndex() {
	this->removalIndex = new std::vector<unsigned int>();
	for (int i = 0; i < mGraph->GetNumberOfNodes(); i++) {
		removalIndex->push_back(0);
	}
	for (unsigned int index = 0; index < mGraph->GetNumberOfNodes(); index++) {
		auto node = sortedNodesByDegree->at(index);
		removalIndex->at(node) = index;
	}
}

void GPUMotifCalculator::CopyAllToDevice() {
	//TODO: fill!
	// Motif Variations
	this->deviceMotifVariations = *this->nodeVariations;
	// Removal index
	this->deviceRemovalIndex = *this->removalIndex;

	// Feature matrix
	unsigned int size = this->mGraph->GetNumberOfNodes()
			* this->nodeVariations->size() * sizeof(unsigned int);
	cudaMallocManged(this->deviceFeatures, size);

	// Original graph
	cudaMallocManged(&deviceOriginalGraphOffsets,
			(this->mGraph->GetNumberOfNodes() + 1) * sizeof(int64));
	cudaMallocManged(&deviceOriginalGraphNeighbors,
			(this->mGraph->GetNumberOfEdges()) * sizeof(unsigned int));
	std::memcpy(deviceOriginalGraphOffsets, this->mGraph->GetOffsetList(),
			(this->mGraph->GetNumberOfNodes() + 1) * sizeof(int64));
	std::memcpy(deviceOriginalGraphNeighbors, this->mGraph->GetNeighborList(),
			(this->mGraph->GetNumberOfEdges()) * sizeof(unsigned int));

	// Full graph
	cudaMallocManged(&deviceFullGraphOffsets,
			(this->fullGraph.GetNumberOfNodes() + 1) * sizeof(int64));
	cudaMallocManged(&deviceFullGraphNeighbors,
			(this->fullGraph.GetNumberOfEdges()) * sizeof(unsigned int));
	std::memcpy(deviceOriginalGraphOffsets, this->fullGraph.GetOffsetList(),
			(this->fullGraph.GetNumberOfNodes() + 1) * sizeof(int64));
	std::memcpy(deviceOriginalGraphNeighbors, this->fullGraph.GetNeighborList(),
			(this->fullGraph.GetNumberOfEdges()) * sizeof(unsigned int));

}

vector<vector<unsigned int> *> *GPUMotifCalculator::Calculate() {

	int blockSize = 256;
	int numBlocks = (this->mGraph->GetNumberOfNodes() + blockSize - 1)
			/ blockSize;

	if (this->level == 3) {
		//std::cout << "Start 3" << std::endl;

		// for (auto node : *(this->sortedNodesByDegree)) {
		// 	//std::cout << node << std::endl;
		// 	Motif3Subtree(node);
		// }
	Motif3Kernel<<<numBlocks, blockSize>>>(this);
} else {
	//std::cout << "Start 4" << std::endl;

	// for (auto node : *(this->sortedNodesByDegree))
	// 	Motif4Subtree(node);
	Motif4Kernel<<<numBlocks, blockSize>>>(this);
}
	//std::cout << "Done All" << std::endl;

//TODO: convert the device features to the vector format
for (int node = 0; node < this->mGraph->GetNumberOfNodes(); node++) {
vector<unsigned int>* current = new vector<unsigned int>();
for (int motif = 0; motif < this->numOfMotifs; motif++) {
	current->push_back(this->deviceFeatures[motif + this->numOfMotifs * node]);
}
this->features->push_back(current);
}

return this->features;
}

// Kernel and friend functions
void Motif3Kernel(GPUMotifCalculator *calc) {
int index = blockIdx.x * blockDim.x + threadIdx.x;
int stride = blockDim.x * gridDim.x;
auto n = calc->mGraph->GetNumberOfNodes();
for (int i = index; i < n; i += stride)
calc->Motif3Subtree(calc->sortedNodesByDegree->at(i));
}

void Motif4Kernel(GPUMotifCalculator *calc) {
int index = blockIdx.x * blockDim.x + threadIdx.x;
int stride = blockDim.x * gridDim.x;
auto n = calc->mGraph->GetNumberOfNodes();
for (int i = index; i < n; i += stride)
calc->Motif4Subtree(calc->sortedNodesByDegree->at(i));
}

void GPUMotifCalculator::Motif3Subtree(unsigned int root) {
	// Instead of yield call GroupUpdater function
	// Don't forget to check each time that the nodes are in the graph (check removal index).
int idx_root = this->deviceRemovalIndex[root];// root_idx is also our current iteration -
thrust::device_vector<bool> visited_vertices(this->mGraph->GetNumberOfNodes()); // every node_idx smaller than root_idx is already handled
visited_vertices[root] = true;

const unsigned int *neighbors = this->deviceFullGraphNeighbors; // all neighbors - ancestors and descendants
const int64 *offsets = this->deviceFullGraphOffsets;

 // TODO problem with dual edges
 //std::cout << "Mark" << std::endl;
for (int64 i = offsets[root]; i < offsets[root + 1]; i++) // loop first neighbors
if (this->deviceRemovalIndex[neighbors[i]] > idx_root) // n1 not handled yet
	visited_vertices[neighbors[i]] = true;
 //std::cout << "Mark" << std::endl;
for (int64 n1_idx = offsets[root]; n1_idx < offsets[root + 1]; n1_idx++) { // loop first neighbors
unsigned int n1 = neighbors[n1_idx];
if (this->deviceRemovalIndex[n1] <= idx_root) // n1 already handled
	continue;
for (int64 n2_idx = offsets[n1]; n2_idx < offsets[n1 + 1]; n2_idx++) { // loop second neighbors
	unsigned int n2 = neighbors[n2_idx];
	if (this->deviceRemovalIndex[n2] <= idx_root) // n2 already handled
		continue;
	if (visited_vertices[n2]) {					// check if n2 was visited &&
		if (n1 < n2)		// n2 is after n1 (stops counting the motif twice)
			this->GroupUpdater( { root, n1, n2 }, 3); // update motif counter [r,n1,n2]
	} else {
		visited_vertices[n2] = true;

		this->GroupUpdater( { root, n1, n2 }, 3); // update motif counter [r,n1,n2]
	}										   // end ELSE
}											   // end LOOP_SECOND_NEIGHBORS

} // end LOOP_FIRST_NEIGHBORS
  //std::cout << "Mark" << std::endl;
  // vector<vector<unsigned int> *> *n1_comb = neighbors_combinations(neighbors,
  // 	offsets[root], offsets[root + 1]);
for (int64 i = offsets[root]; i < offsets[root + 1]; i++) {
for (int64 j = i + 1; j < offsets[root + 1]; j++) {
	unsigned int n1 = neighbors[i];
	unsigned int n2 = neighbors[j];
	//std::cout << "\t" << n1 << "," << n2 << std::endl;
	if (this->deviceRemovalIndex[n1] <= idx_root
			|| this->deviceRemovalIndex[n2] <= idx_root) // motif already handled
		continue;
	//std::cout << "Mark1" << std::endl;
	//std::cout << (visited_vertices[n1] < visited_vertices[n2]) << std::endl;
	//std::cout << mGraph->areNeighbors(n1, n2) << std::endl;
	//std::cout << mGraph->areNeighbors(n2, n1) << std::endl;
	if ((n1 < n2)
			&& !(this->AreNeighbors(n1, n2) || this->AreNeighbors(n2, n1))) // check n1, n2 not neighbors
		//std::cout << "Mark2" << std::endl;
		this->GroupUpdater( { root, n1, n2 }, 3); // update motif counter [r,n1,n2]
}
} // end loop COMBINATIONS_NEIGHBORS_N1
}

void GPUMotifCalculator::Motif4Subtree(unsigned int root) {
	int idx_root = this->deviceRemovalIndex[root]; // root_idx is also our current iteration -
	thrust::device_vector<short> visited_vertices(this->mGraph->GetNumberOfNodes()); // every node_idx smaller than root_idx is already handled
	thrust::fill(thrust::device, visited_vertices.begin(), visited_vertices.end(), -1);
	visited_vertices[root] = 0;

	const unsigned int *neighbors = this->deviceFullGraphNeighbors; // all neighbors - ancestors and descendants
	const int64 *offsets = this->deviceFullGraphOffsets;

	 // TODO problem with dual edges
	for (int64 i = offsets[root]; i < offsets[root + 1]; i++) // loop first neighbors
	if (this->deviceRemovalIndex[neighbors[i]] > idx_root) // n1 not handled yet
		visited_vertices[neighbors[i]] = 1;

	/*
	 *    for n1, n2, n3 in combinations(neighbors_first_deg, 3):
	 yield [root, n1, n2, n3]
	 */
	 // vector<vector<unsigned int> *> *n1_3_comb = neighbors_combinations(
	 // 	neighbors, offsets[root], offsets[root + 1], 3);
	 // for (auto it = n1_3_comb->begin(); it != n1_3_comb->end(); ++it)
	 // {
	int64 end = offsets[root + 1];
	for (int64 i = offsets[root]; i < end; i++) {
	for (int64 j = i + 1; j < end; j++) {
		if (j == end - 1) //if j is the last element, we can't add an element and therefore it's not a 3-combination
			continue;
		for (int64 k = j + 1; k < end; k++) {
			unsigned int n11 = neighbors[i];
			unsigned int n12 = neighbors[j];
			unsigned int n13 = neighbors[k];
			if (this->deviceRemovalIndex[n11] <= idx_root
					|| this->deviceRemovalIndex[n12] <= idx_root
					|| this->deviceRemovalIndex[n13] <= idx_root) // motif already handled
				continue;
			this->GroupUpdater( { root, n11, n12, n13 }, 4); // update motif counter [r,n11,n12,n13]
		}
	}
	}

	 // All other cases
	for (int64 n1_idx = offsets[root]; n1_idx < offsets[root + 1]; n1_idx++) { // loop first neighbors
	unsigned int n1 = neighbors[n1_idx];
	if (this->deviceRemovalIndex[n1] <= idx_root) // n1 already handled
		continue;
	//Mark second neighbors
	for (int64 n2_idx = offsets[n1]; n2_idx < offsets[n1 + 1]; n2_idx++) { // loop second neighbors
		unsigned int n2 = neighbors[n2_idx];
		if (this->deviceRemovalIndex[n2] <= idx_root) // n2 already handled
			continue;
		if (visited_vertices[n2] == -1) { // check if n2 was *not* visited
			visited_vertices[n2] = 2;

		} //end if
	}	 //end loop SECOND NEIGHBORS

	// The case of root-n1-n2-n11
	for (int64 n2_idx = offsets[n1]; n2_idx < offsets[n1 + 1]; n2_idx++) { // loop second neighbors (again)
		unsigned int n2 = neighbors[n2_idx];
		if (this->deviceRemovalIndex[n2] <= idx_root) // n2 already handled
			continue;
		for (int64 n11_idx = offsets[root]; n11_idx < offsets[root + 1];
				n11_idx++) { // loop first neighbors
			unsigned int n11 = neighbors[n11_idx];
			if (this->deviceRemovalIndex[n11] <= idx_root) // n2 already handled
				continue;
			if (visited_vertices[n2] == 2 && n11 != n1) {
				this->GroupUpdater( { root, n1, n11, n2 }, 4); // update motif counter [r,n1,n11,n2]
			}												// end if
		}										// end loop INNER FIRST NEIGHBORS
	}											// end loop SECOND NEIGHBORS AGAIN

	// The case of root-n1-n21-n22
	//2-combinations on second neighbors
	// vector<vector<unsigned int> *> *n2_comb = neighbors_combinations(
	// 	neighbors, offsets[n1], offsets[n1 + 1]);
	// for (auto it = n2_comb->begin(); it != n2_comb->end(); ++it)
	// {
	end = offsets[n1 + 1];
	for (int64 i = offsets[n1]; i < end; i++) {
		for (int64 j = i + 1; j < end; j++) {

			unsigned int n21 = neighbors[i];
			unsigned int n22 = neighbors[j];
			if (this->deviceRemovalIndex[n21] <= idx_root
					|| this->deviceRemovalIndex[n22] <= idx_root) // motif already handled
				continue;
			if (2 == visited_vertices[n21] && visited_vertices[n22] == 2) {
				this->GroupUpdater( { root, n1, n21, n22 }, 4); // update motif counter [r,n1,n21,n22]
			}
		}
	} // end loop SECOND NEIGHBOR COMBINATIONS

	//The case of n1-n2-n3
	for (int64 n2_idx = offsets[n1]; n2_idx < offsets[n1 + 1]; n2_idx++) { // loop second neighbors (third time's the charm)
		unsigned int n2 = neighbors[n2_idx];
		if (this->deviceRemovalIndex[n2] <= idx_root) // n2 already handled
			continue;
		for (int64 n3_idx = offsets[n2]; n3_idx < offsets[n2 + 1]; n3_idx++) { // loop third neighbors
			unsigned int n3 = neighbors[n3_idx];
			if (this->deviceRemovalIndex[n3] <= idx_root) // n2 already handled
				continue;
			if (visited_vertices[n3] == -1) { // check if n3 was *not* visited
				visited_vertices[n3] = 3;
				if (visited_vertices[n2] == 2) { // check if n2 is a visited second neighbor
					this->GroupUpdater( { root, n1, n2, n3 }, 4); // update motif counter [r,n1,n2,n3]
				}					// end check if n2 is a visited second neighbor
			}									// end check if n3 was not visited

			else if (visited_vertices[n3] == 3 && visited_vertices[n2] == 2) {
				this->GroupUpdater( { root, n1, n2, n3 }, 4); // update motif counter [r,n1,n2,n3]
			}											   // end if
		}												 // end loop THIRD NEIGHBORS
	}							// end loop SECOND NEIGHBORS THIRD TIME'S THE CHARM

	} // end loop FIRST NEIGHBORS
}

bool GPUMotifCalculator::AreNeighbors(unsigned int p, unsigned int q) {
 // int64* deviceOriginalGraphOffsets;
 // unsigned int* deviceOriginalGraphNeighbors;
thrust::device_ptr<unsigned int> neighbor_ptr( // @suppress("Type cannot be resolved") // @suppress("Symbol is not resolved")
	this->deviceOriginalGraphNeighbors);
int64 begin_offset = deviceOriginalGraphOffsets[p];
int64 end_offset = deviceOriginalGraphOffsets[p + 1];

thrust::device_vector<unsigned int> values(1); // @suppress("Type cannot be resolved") // @suppress("Symbol is not resolved")
values[0] = q;
thrust::device_vector<bool> output(1); // @suppress("Type cannot be resolved") // @suppress("Symbol is not resolved")
thrust::binary_search(neighbor_ptr + begin_offset, neighbor_ptr + end_offset, // @suppress("Function cannot be resolved")
values.begin(), values.end(), output.begin(), thrust::less<unsigned int>()); // @suppress("Function cannot be resolved")

return output[0];
}

void GPUMotifCalculator::GroupUpdater(unsigned int group[], int size) {
 // TODO: count overall number of motifs in graph (maybe different class)?
int groupNumber = GetGroupNumber(group, size);
int motifNumber = (this->deviceMotifVariations)[groupNumber];
if (motifNumber != -1)
for (int i = 0; i < size; i++)
	atomicAdd(deviceFeatures + (motifNumber + this->numOfMotifs * group[i]), 1); //atomic add + access as 1D array : features[motif + M*node] // @suppress("Function cannot be resolved")
// where M is the number of motifs
}

int GPUMotifCalculator::GetGroupNumber(unsigned int group[], int size) {
int sum = 0;
int power = 1;
bool hasEdge;
if (directed) {
// Use permutations
for (int i = 0; i < size; i++) {
	for (int j = 0; j < size; j++) {
		if (i != j) {
			hasEdge = this->AreNeighbors(group[i], group[j]);
			if (hasEdge)
				sum += power;
			power *= 2;
		}
	}
}
} else {
// Use combinations
for (int i = 0; i < size; i++) {
	for (int j = i + 1; j < size; j++) {

		hasEdge = this->AreNeighbors(group[i], group[j]);
		if (hasEdge)
			sum += power;
		power *= 2;
	}
}

}
return sum;
}

GPUMotifCalculator::~GPUMotifCalculator() {
//map the group num to the iso motif
delete nodeVariations;
//the index in which we remove the node from the graph. Basically, from this index on the node doesen't exist.
delete removalIndex;
//the nodes, sorted in descending order by the degree.
delete sortedNodesByDegree;

 // For the original graph
cudaFree(deviceOriginalGraphOffsets); // @suppress("Function cannot be resolved")
cudaFree(deviceOriginalGraphNeighbors); // @suppress("Function cannot be resolved")

 // For the full graph
cudaFree(deviceFullGraphOffsets); // @suppress("Function cannot be resolved")
cudaFree(deviceFullGraphNeighbors); // @suppress("Function cannot be resolved")

 // Feature array
cudaFree(deviceFeatures); // @suppress("Function cannot be resolved")
}
