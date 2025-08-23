#pragma once
#include <list>
#include "Grid.h"

class AStar final
{
public:

	AStar(Grid& grid) 
		:m_Grid{ grid } {}

	void Start();
	void Tick(); 

private:

	void Step();
	void FindStartNode();
	void FindDestinationNode(); 
	void EvaluateNeighbors();
	void SelectLowestCostNode();  
	void CalculateNodeCost(Node* from, Node* node);
	Grid& m_Grid; 
	std::list<Node*> m_OpenList;
	std::list<Node*> m_ClosedList;

	Node* m_CurrentNode = nullptr;
	Node* m_StartNode = nullptr;
	Node* m_DestinationNode = nullptr;

};

