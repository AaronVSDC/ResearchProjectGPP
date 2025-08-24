#include "Grid.h"
#include "GameEngine.h"

void Grid::Start()
{
	LEFT_OFFSET = GAME_ENGINE->GetWidth() / 2 - GRID_COLS * CELL_SIZE / 2;
	TOP_OFFSET = GAME_ENGINE->GetHeight() / 2 - GRID_ROWS * CELL_SIZE / 2;

	m_Nodes.assign(GRID_ROWS, std::vector<Node*>(GRID_COLS, nullptr));
	for (int row = 0; row < GRID_ROWS; ++row)
	{
		for (int col = 0; col < GRID_COLS; ++col)
		{
			m_Nodes[row][col] = new Node{};
			m_Nodes[row][col]->row = row;
			m_Nodes[row][col]->column = col;
		}
	}
	DecideStartAndDestination();
	FillRandomWalls();

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
			if (m_Nodes[row][collumn]->nodeType != NodeType::Start and m_Nodes[row][collumn]->nodeType != NodeType::Destination)
				m_Nodes[row][collumn]->nodeType = NodeType::Obstacle;
		}
	}
	if (!isLeft and isDown)
	{
		int row{};
		int collumn{};
		if (PointToGrid(x, y, row, collumn))
		{
			if (m_Nodes[row][collumn]->nodeType == NodeType::Obstacle)
			{
				m_Nodes[row][collumn]->nodeType = NodeType::Empty; 
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


			if (m_Nodes[row][collumn]->open and m_Nodes[row][collumn]->nodeType != NodeType::Destination)
			{
				GAME_ENGINE->SetColor(RGB(0, 200, 200));
				GAME_ENGINE->FillRect(left, top, right, bottom);
			}
			if (m_Nodes[row][collumn]->closed and m_Nodes[row][collumn]->nodeType != NodeType::Destination)
			{
				GAME_ENGINE->SetColor(RGB(200, 200, 0));
				GAME_ENGINE->FillRect(left, top, right, bottom);
			}

			switch (m_Nodes[row][collumn]->nodeType)
			{
			case NodeType::Start:
				GAME_ENGINE->SetColor(RGB(0, 255, 0));
				GAME_ENGINE->FillRect(left, top, right, bottom);
				break;
			case NodeType::Destination:
				GAME_ENGINE->SetColor(RGB(255, 0, 0));
				GAME_ENGINE->FillRect(left, top, right, bottom);
				break;
			case NodeType::Obstacle:
				GAME_ENGINE->SetColor(RGB(240, 234, 214));
				GAME_ENGINE->FillRect(left, top, right, bottom);
				break;
			case NodeType::Path:
				GAME_ENGINE->SetColor(RGB(0, 0, 214));
				GAME_ENGINE->FillRect(left, top, right, bottom);
				break;
			default:
				break;
			}




			GAME_ENGINE->SetColor(RGB(23, 23, 23)); 
			GAME_ENGINE->DrawRect(left, top, right, bottom);

			if (m_Nodes[row][collumn]->fScore != INT_MAX)
			{
				GAME_ENGINE->SetColor(RGB(255, 255, 255));
				GAME_ENGINE->DrawString(to_tstring(m_Nodes[row][collumn]->fScore), left + 4, top + 4);
			}

		}
	}
}

void Grid::FillRandomWalls()
{
	if (m_Nodes.empty() || m_Nodes[0].empty()) return;

	const int rows = static_cast<int>(m_Nodes.size());
	const int cols = static_cast<int>(m_Nodes[0].size());

	const int centerX = cols / 2;  
	const int thickness = 1;            
	const int margin = max(1, rows / 10); 

	for (int dx = 0; dx < thickness; ++dx)
	{
		int x = std::clamp(centerX + dx - thickness / 2, 0, cols - 1);
		for (int y = margin; y < rows - margin; ++y)
		{
			auto* node = m_Nodes[y][x];
			if (node && node->nodeType == NodeType::Empty)
				node->nodeType = NodeType::Obstacle;
		}
	}
}
void Grid::DecideStartAndDestination()
{
	std::size_t rowStart = GRID_ROWS / 2 - 1;
	std::size_t collumnStart = 4;

	m_Nodes[rowStart][collumnStart]->nodeType = NodeType::Start; 

	std::size_t rowDestination = GRID_ROWS / 2 - 1;
	std::size_t collumnDestination = GRID_COLS - 5;

	m_Nodes[rowDestination][collumnDestination]->nodeType = NodeType::Destination; 
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

void Grid::Reset()
{
	for (auto& row : m_Nodes)
	{
		for (auto& node : row)
		{
			node->gScore = INT_MAX;
			node->hScore = 0;
			node->fScore = INT_MAX; 
			node->parent = nullptr;
			node->open = false;
			node->closed = false;
			node->nodeType = NodeType::Empty;
		}
	}
	DecideStartAndDestination();
}