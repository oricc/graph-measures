#pragma once
#include "stdafx.h"
//#include "Utilities.h"
#include <string>
#include <vector>
//#include "IncludeBoostLogger.h"
#define _CRT_SECURE_NO_WARNINGS
/*
	This class is based on the code from Lev's lab.
	The class encalpsulates a graph that is saved in memory with two lists:
	1) An ordered adjacency list of all nodes
	2) A list of offsets that saves the index of the beginning of every node's section 
		in the adjacency list.

	Access to the graph is done by first checking the offset list to find the position of a
	node, and then iterating over all the adjacent nodes.

*/
class GraphSnapshot
{
public: 
	GraphSnapshot(): m_NumberOfNodes(0), m_Graph(NULL), m_Offsets(NULL){}
	GraphSnapshot(const GraphSnapshot&)=delete;
	~GraphSnapshot() { Clear();  }
	const GraphSnapshot& operator=(const GraphSnapshot&) = delete; 

	void Clear(); 
	void Assign(const std::vector<unsigned __int64>& NodeOffsets, const std::vector<unsigned int>& Neighbours);
	bool SaveToFile(const std::string& FileName) const; 
	bool LoadFromFile(const std::string& FileName);
	bool LoadFromFile(const std::string& DirectroyName, const std::string& BaseFileName);

	static std::string GetFileNameFromFolder(const std::string& DirectroyName, const std::string& BaseFileName);

	const unsigned int GetNumberOfNodes() const { return m_NumberOfNodes; }
	const unsigned int* GetNeighborList() const { return m_Graph; }
	const unsigned __int64* GetOffsetList() const { return m_Offsets; }

	void InverseGraph(GraphSnapshot& InvertedGraph) const;
	void CureateUndirectedGraph(const GraphSnapshot& InvertedGraph, GraphSnapshot& UndirectedGraph) const;

	//void ToBoostGraph() const;
	std::vector<unsigned int> ComputeNodeDegrees() const; 
	std::vector<float> ComputeNodePageRank(float dumping, unsigned int NumberOfIterations) const;
	std::vector<unsigned short> ComputeKCore() const;

	template <typename  value_type> 
	static bool SaveValueToFile(const std::vector<value_type>& values, const std::string& FileName, bool IsNewFile);

	bool areNeighbors(const unsigned int p, const unsigned int q);

private:
	unsigned int m_NumberOfNodes;
	unsigned int *m_Graph; 
	unsigned __int64* m_Offsets; 
};