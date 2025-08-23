#pragma once
#include <vector>

#include "AbstractGame.h"

enum class NodeType
{
	Empty,
	Start,
	Destination,
	Obstacle,
	Path
};

struct Node
{
	int row = 0;
	int column = 0;
	int gScore = std::numeric_limits<int>::infinity();
	int hScore = 0;
	int fScore = std::numeric_limits<int>::infinity();

	Node* parent; 
	bool open = false;
	bool closed = false;
	NodeType nodeType = NodeType::Empty; 

};
class Grid final
{
public:
	void Start(); 

	void Paint() const;
	void MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam); 
	void Reset();

	std::vector<std::vector<Node*>>& GetNodes() { return m_Nodes;  }
	int GetAmountOfGridCols() const { return GRID_COLS;  }
	int GetAmountOfGridRows() const { return GRID_ROWS;  }

private:

	std::vector<std::vector<Node*>> m_Nodes;


	const int GRID_COLS{ 40 };
	const int GRID_ROWS{ 26 };
	const int CELL_SIZE{ 32 };

	int LEFT_OFFSET{}; 
	int TOP_OFFSET{};

	bool m_IsMousePressedLeft = false; 
	bool m_IsMousePressedRight = false;

	Node m_StartNode;
	Node m_DestinationNode;

	void PaintGrid() const ;
	void DecideStartAndDestination(); 
	bool PointToGrid(int x, int y, int& ourRow, int& outCol) const; 

};

