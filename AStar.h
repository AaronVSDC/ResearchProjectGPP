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
	void EvaluateNeighbors(); 
	Grid& m_Grid; 
	std::list<Node*> m_OpenList;
	std::list<Node*> m_ClosedList;

	Node* m_CurrentNode = nullptr; 
};

