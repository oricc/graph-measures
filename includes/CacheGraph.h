/*
 * CacheGraph.h
 *
 *  Created on: Oct 28, 2018
 *      Author: ori
 */

#ifndef CACHEGRAPH_H_
#define CACHEGRAPH_H_

#include <string>
#include <vector>
#include <iomanip>
#include <exception>
//#include <boost/graph/adjacency_list.hpp>
#pragma warning(push)
#pragma warning(disable: 4996) //4996 for _CRT_SECURE_NO_WARNINGS equivalent
#include <numeric>
#include <random>
#pragma warning(pop)
#include "stdafx.h"


/*
	This class is based on the code from Lev's lab.
	The class encapsulates a graph that is saved in memory with two lists:
	1) An ordered adjacency list of all nodes
	2) A list of offsets that saves the index of the beginning of every node's section
		in the adjacency list.

	Access to the graph is done by first checking the offset list to find the position of a
	node, and then iterating over all the adjacent nodes.

*/
class CacheGraph
{
public:
	CacheGraph(): m_NumberOfNodes(0), m_Graph(NULL), m_Offsets(NULL){}
	CacheGraph(const CacheGraph&)=delete;
	~CacheGraph() { Clear();  }
	const CacheGraph& operator=(const CacheGraph&) = delete;

	void Clear();
	void Assign(const std::vector<int64>& NodeOffsets, const std::vector<unsigned int>& Neighbours);
	bool SaveToFile(const std::string& FileName) const;
	bool LoadFromFile(const std::string& FileName);
	bool LoadFromFile(const std::string& DirectroyName, const std::string& BaseFileName);

	static std::string GetFileNameFromFolder(const std::string& DirectroyName, const std::string& BaseFileName);

	const unsigned int GetNumberOfNodes() const { return m_NumberOfNodes; }
	const unsigned int* GetNeighborList() const { return m_Graph; }
	const int64* GetOffsetList() const { return m_Offsets; }

	void InverseGraph(CacheGraph& InvertedGraph) const;
	void CureateUndirectedGraph(const CacheGraph& InvertedGraph, CacheGraph& UndirectedGraph) const;

	//void ToBoostGraph() const;
	std::vector<unsigned int> ComputeNodeDegrees() const;
	std::vector<float> ComputeNodePageRank(float dumping, unsigned int NumberOfIterations) const;
	std::vector<unsigned short> ComputeKCore() const;

	template <typename  value_type>
	static bool SaveValueToFile(const std::vector<value_type>& values, const std::string& FileName, bool IsNewFile);

	bool areNeighbors(const unsigned int p, const unsigned int q) const;

private:
	unsigned int m_NumberOfNodes;
	unsigned int *m_Graph;
	int64* m_Offsets;
};

template <typename  value_type> bool CacheGraph::SaveValueToFile(const std::vector<value_type>& values, const std::string& FileName, bool IsNewFile)
{
	FILE* hFile = std::fopen(FileName.c_str(), (IsNewFile) ? "w+b" : "ab");
	std::fwrite(&values[0], sizeof(value_type), values.size(), hFile);
	std::fclose(hFile); hFile = NULL;
	return true;
}
template bool CacheGraph::SaveValueToFile<unsigned int>(const std::vector<unsigned int>& values, const std::string& FileName, bool IsNewFile);
template bool CacheGraph::SaveValueToFile<float>(const std::vector<float>& values, const std::string& FileName, bool IsNewFile);
template bool CacheGraph::SaveValueToFile<int64>(const std::vector<int64>& values, const std::string& FileName, bool IsNewFile);
template bool CacheGraph::SaveValueToFile<unsigned short>(const std::vector<unsigned short>& values, const std::string& FileName, bool IsNewFile);

#endif /* CACHEGRAPH_H_ */
