#pragma once
#include <vector>

#include "AbstractGame.h"

enum class CellType { Empty, Start, Destination, Obstacle };

class Grid final
{
public:
	void Start(); 

	void Paint() const;
	void MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam); 

	std::vector<std::vector<CellType>>& GetGrid() { return m_Grid;  };
private:

	std::vector<std::vector<CellType>> m_Grid;


	const int GRID_COLS{ 40 };
	const int GRID_ROWS{ 26 };
	const int CELL_SIZE{ 32 };

	int LEFT_OFFSET{}; 
	int TOP_OFFSET{};

	bool m_IsMousePressedLeft = false; 
	bool m_IsMousePressedRight = false;

	void PaintGrid() const ;
	void DecideStartAndDestination(); 
	bool PointToGrid(int x, int y, int& ourRow, int& outCol) const; 

};

