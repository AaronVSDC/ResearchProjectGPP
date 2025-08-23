#pragma once
#include <vector>

#include "AbstractGame.h"

class Grid
{
public:
	void Start(); 

	void Paint() const;
	void MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam); 

private: 

	enum class CellType { Empty, Start, Destination, Obstacle };
	std::vector<std::vector<CellType>> m_Grid;


	const int GRID_COLS{ 40 };
	const int GRID_ROWS{ 26 };
	const int CELL_SIZE{ 32 };

	int LEFT_OFFSET{}; 
	int TOP_OFFSET{}; 

	void PaintGrid() const ;
	void DecideStartAndDestination();
	bool PointToGrid(int x, int y, int& ourRow, int& outColl) const; 

};

