#include "Grid.h"
#include "GameEngine.h"

void Grid::Start()
{
	LEFT_OFFSET = GAME_ENGINE->GetWidth() / 2 - GRID_COLS * CELL_SIZE / 2;
	TOP_OFFSET = GAME_ENGINE->GetHeight() / 2 - GRID_ROWS * CELL_SIZE / 2;

	m_Grid.assign(GRID_ROWS, std::vector<CellType>(GRID_COLS, CellType::Empty));
	DecideStartAndDestination();
}

void Grid::Paint() const
{
	PaintGrid();
}

void Grid::MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam)
{
	if (isLeft and isDown)
	{
		int row{};
		int collumn{}; 
		if (PointToGrid(x, y, row, collumn))
		{
			if (m_Grid[row][collumn] != CellType::Start and m_Grid[row][collumn] != CellType::Destination)
				m_Grid[row][collumn] = CellType::Obstacle;
		}
	}
	if (!isLeft and isDown)
	{
		int row{};
		int collumn{};
		if (PointToGrid(x, y, row, collumn))
		{
			if (m_Grid[row][collumn] == CellType::Obstacle)
			{
				m_Grid[row][collumn] = CellType::Empty; 
			}
		}
	}

}

void Grid::PaintGrid() const
{
	for (int row = 0; row < GRID_ROWS; ++row)
	{
		for (int collumn = 0; collumn < GRID_COLS; ++collumn)
		{
			int left = collumn * CELL_SIZE + LEFT_OFFSET;
			int top = row * CELL_SIZE + TOP_OFFSET;
			int right = left + CELL_SIZE;
			int bottom = top + CELL_SIZE;

			switch (m_Grid[row][collumn])
			{
			case CellType::Start:
				GAME_ENGINE->SetColor(RGB(0, 255, 0));
				GAME_ENGINE->FillRect(left, top, right, bottom);
				break;
			case CellType::Destination:
				GAME_ENGINE->SetColor(RGB(255, 0, 0));
				GAME_ENGINE->FillRect(left, top, right, bottom);
				break;
			case CellType::Obstacle:
				GAME_ENGINE->SetColor(RGB(240,234,214));
				GAME_ENGINE->FillRect(left, top, right, bottom);
				break;
			default:
				break;
			}

			GAME_ENGINE->SetColor(RGB(200, 200, 200));
			GAME_ENGINE->DrawRect(left, top, right, bottom);
		}
	}
}

void Grid::DecideStartAndDestination()
{
	//keep in mind indices start from 0
	std::size_t rowStart = GRID_ROWS / 2 - 1;
	std::size_t collumnStart = 4;

	m_Grid[rowStart][collumnStart] = CellType::Start; 

	std::size_t rowDestination = GRID_ROWS / 2 - 1;
	std::size_t collumnDestination = GRID_COLS - 5;

	m_Grid[rowDestination][collumnDestination] = CellType::Destination; 
}

bool Grid::PointToGrid(int x, int y, int& outRow, int& outCol) const
{
	const int localX = x - LEFT_OFFSET;
	const int localY = y - TOP_OFFSET;

	if (localX < 0 || localY < 0) return false;
	if (localX >= GRID_COLS * CELL_SIZE || localY >= GRID_ROWS * CELL_SIZE) return false;

	outCol = localX / CELL_SIZE;  
	outRow = localY / CELL_SIZE; 
	return true;
}
