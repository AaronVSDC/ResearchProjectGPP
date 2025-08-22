
//-----------------------------------------------------------------
// Main Game File
// C++ Source - Game.cpp - version v8_01
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Game.h"
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <algorithm>

//-----------------------------------------------------------------
// Game Member Functions																				
//-----------------------------------------------------------------

Game::Game()
{
    // nothing to create
}

Game::~Game()
{
    // nothing to destroy
}

void Game::Initialize()
{
    // Code that needs to execute (once) at the start of the game, before the game window is created

    AbstractGame::Initialize();
    GAME_ENGINE->SetTitle(_T("ResearchProjectGPP"));

    GAME_ENGINE->SetWidth(1024);
    GAME_ENGINE->SetHeight(768);
    GAME_ENGINE->SetFrameRate(50);

}

void Game::Start()
{
    mGrid.assign(GRID_COLS * GRID_ROWS, CellType::Empty);

    int baseX = GRID_COLS * CELL_SIZE + 20;
    int y = 20;

    mBtnStart.SetBounds(baseX, y, baseX + 100, y + 30);
    mBtnStart.SetText(L"Start");
    mBtnStart.AddActionListener(this);
    mBtnStart.Show(); 
    y += 40;

    mBtnDest.SetBounds(baseX, y, baseX + 100, y + 30);
    mBtnDest.SetText(L"Destination");
    mBtnDest.AddActionListener(this);
    mBtnDest.Show();
    y += 40;

    mBtnObstacle.SetBounds(baseX, y, baseX + 100, y + 30);
    mBtnObstacle.SetText(L"Obstacle");
    mBtnObstacle.AddActionListener(this);
    mBtnObstacle.Show();
    y += 40;

    mBtnEmpty.SetBounds(baseX, y, baseX + 100, y + 30);
    mBtnEmpty.SetText(L"Empty");
    mBtnEmpty.AddActionListener(this);
    mBtnEmpty.Show();
    y += 40;

    mBtnSolve.SetBounds(baseX, y, baseX + 100, y + 30);
    mBtnSolve.SetText(L"Solve");
    mBtnSolve.AddActionListener(this);
    mBtnSolve.Show();
    y += 40;

    mBtnStep.SetBounds(baseX, y, baseX + 100, y + 30);
    mBtnStep.SetText(L"Step");
    mBtnStep.AddActionListener(this);
    mBtnStep.Show();
}

void Game::End()
{
    // Insert code that needs to execute when the game ends
}

void Game::Paint(RECT rect) const
{
    GAME_ENGINE->FillWindowRect(RGB(0, 0, 0));
    for (int row = 0; row < GRID_ROWS; ++row)
    {
        for (int col = 0; col < GRID_COLS; ++col)
        {
            int left = col * CELL_SIZE;
            int top = row * CELL_SIZE;
            int right = left + CELL_SIZE;
            int bottom = top + CELL_SIZE;

            switch (mGrid[row * GRID_COLS + col])
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
                GAME_ENGINE->SetColor(RGB(0, 0, 0));
                GAME_ENGINE->FillRect(left, top, right, bottom);
                break;
            default:
                break;
            }

            GAME_ENGINE->SetColor(RGB(200, 200, 200));
            GAME_ENGINE->DrawRect(left, top, right, bottom);
        }
    }

    for (size_t i = 0; i < mPathStep && i < mPath.size(); ++i)
    {
        POINT p = mPath[i];
        int left = p.x * CELL_SIZE;
        int top = p.y * CELL_SIZE;
        int right = left + CELL_SIZE;
        int bottom = top + CELL_SIZE;

        GAME_ENGINE->SetColor(RGB(0, 0, 255));
        GAME_ENGINE->FillRect(left, top, right, bottom);
        GAME_ENGINE->SetColor(RGB(200, 200, 200));
        GAME_ENGINE->DrawRect(left, top, right, bottom);
    }

    if (mStartCell.x != -1)
    {
        int left = mStartCell.x * CELL_SIZE;
        int top = mStartCell.y * CELL_SIZE;
        int right = left + CELL_SIZE;
        int bottom = top + CELL_SIZE;
        GAME_ENGINE->SetColor(RGB(0, 255, 0));
        GAME_ENGINE->FillRect(left, top, right, bottom);
        GAME_ENGINE->SetColor(RGB(200, 200, 200));
        GAME_ENGINE->DrawRect(left, top, right, bottom);
    }
    if (mDestCell.x != -1)
    {
        int left = mDestCell.x * CELL_SIZE;
        int top = mDestCell.y * CELL_SIZE;
        int right = left + CELL_SIZE;
        int bottom = top + CELL_SIZE;
        GAME_ENGINE->SetColor(RGB(255, 0, 0));
        GAME_ENGINE->FillRect(left, top, right, bottom);
        GAME_ENGINE->SetColor(RGB(200, 200, 200));
        GAME_ENGINE->DrawRect(left, top, right, bottom);
    }

    GAME_ENGINE->DrawString(m_BrushType, GAME_ENGINE->GetWidth() / 2, GAME_ENGINE->GetHeight() - 20);

}

void Game::Tick()
{
}

void Game::MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam)
{
    if (isLeft && isDown)
    {
        int col = x / CELL_SIZE;
        int row = y / CELL_SIZE;
        if (col >= 0 && col < GRID_COLS && row >= 0 && row < GRID_ROWS)
        {
            int index = row * GRID_COLS + col;
            switch (mCurrentBrush)
            {
            case CellType::Start:
                if (mStartCell.x != -1)
                    mGrid[mStartCell.y * GRID_COLS + mStartCell.x] = CellType::Empty;
                mStartCell = { col, row };
                mGrid[index] = CellType::Start;
                break;
            case CellType::Destination:
				if (mDestCell.x != -1)
					mGrid[mDestCell.y * GRID_COLS + mDestCell.x] = CellType::Empty;
				mDestCell = { col, row };
				mGrid[index] = CellType::Destination;
				break;
			default:
				mGrid[index] = mCurrentBrush;
				break;
			}
		}
	}
}

void Game::MouseWheelAction(int x, int y, int distance, WPARAM wParam)
{	
	// Insert code for a mouse wheel action
}

void Game::MouseMove(int x, int y, WPARAM wParam)
{	
	// Insert code that needs to execute when the mouse pointer moves across the game window

    /* Example:
    if ( x > 261 && x < 261 + 117 ) // check if mouse position is within x coordinates of choice
    {
        if ( y > 182 && y < 182 + 33 ) // check if mouse position also is within y coordinates of choice
        {
            GAME_ENGINE->MessageBox("Mouse move.");
        }
    }
    */
}

void Game::CheckKeyboard()
{
    // Here you can check if a key is pressed down
    // Is executed once per frame 

    /* Example:
    if (GAME_ENGINE->IsKeyDown(_T('K'))) xIcon -= xSpeed;
    if (GAME_ENGINE->IsKeyDown(_T('L'))) yIcon += xSpeed;
    if (GAME_ENGINE->IsKeyDown(_T('M'))) xIcon += xSpeed;
    if (GAME_ENGINE->IsKeyDown(_T('O'))) yIcon -= ySpeed;
    */
}

void Game::KeyPressed(TCHAR key)
{
    // brush selection handled by UI buttons
}

void Game::CallAction(Caller* callerPtr)
{
    if (callerPtr == &mBtnStart)
    {
        mCurrentBrush = CellType::Start;
        m_BrushType = L"BrushType: Start";
    }
    else if (callerPtr == &mBtnDest)
    {
        mCurrentBrush = CellType::Destination;
        m_BrushType = L"BrushType: Destination";
    }
    else if (callerPtr == &mBtnObstacle)
    {
        mCurrentBrush = CellType::Obstacle;
        m_BrushType = L"BrushType: Obstacle";
    }
    else if (callerPtr == &mBtnEmpty)
    {
        mCurrentBrush = CellType::Empty;
        m_BrushType = L"BrushType: Empty";
    }
    else if (callerPtr == &mBtnSolve)
    {
        SolvePath();
    }
    else if (callerPtr == &mBtnStep)
    {
        StepPath();
    }
}

bool Game::IsWalkable(int x, int y) const
{
    return x >= 0 && x < GRID_COLS && y >= 0 && y < GRID_ROWS &&
        mGrid[y * GRID_COLS + x] != CellType::Obstacle;
}

float Game::Heuristic(int x1, int y1, int x2, int y2) const
{
    return static_cast<float>(std::abs(x1 - x2) + std::abs(y1 - y2));
}

POINT Game::Jump(int x, int y, int dx, int dy) const
{
    int nx = x + dx;
    int ny = y + dy;
    if (!IsWalkable(nx, ny)) return { -1, -1 };
    if (nx == mDestCell.x && ny == mDestCell.y) return { nx, ny };

    if (dx != 0 && dy != 0)
    {
        if ((IsWalkable(nx - dx, ny + dy) && !IsWalkable(nx - dx, ny)) ||
            (IsWalkable(nx + dx, ny - dy) && !IsWalkable(nx, ny - dy)))
            return { nx, ny };
    }
    else
    {
        if (dx != 0)
        {
            if ((IsWalkable(nx + dx, ny + 1) && !IsWalkable(nx, ny + 1)) ||
                (IsWalkable(nx + dx, ny - 1) && !IsWalkable(nx, ny - 1)))
                return { nx, ny };
        }
        else
        {
            if ((IsWalkable(nx + 1, ny + dy) && !IsWalkable(nx + 1, ny)) ||
                (IsWalkable(nx - 1, ny + dy) && !IsWalkable(nx - 1, ny)))
                return { nx, ny };
        }
    }

    if (dx != 0 && dy != 0)
    {
        POINT jx = Jump(nx, ny, dx, 0);
        POINT jy = Jump(nx, ny, 0, dy);
        if (jx.x != -1 || jy.x != -1) return { nx, ny };
    }

    return Jump(nx, ny, dx, dy);
}

std::vector<std::pair<int, int>> Game::GetNeighbors(const Node* node) const
{
    std::vector<std::pair<int, int>> neighbors;
    if (node->parent == nullptr)
    {
        const int dirs[8][2] = { {1,0},{0,1},{-1,0},{0,-1},{1,1},{-1,1},{-1,-1},{1,-1} };
        for (auto& d : dirs)
        {
            if (IsWalkable(node->x + d[0], node->y + d[1]))
                neighbors.emplace_back(d[0], d[1]);
        }
    }
    else
    {
        int dx = (node->x - node->parent->x);
        dx = (dx > 0) ? 1 : (dx < 0 ? -1 : 0);
        int dy = (node->y - node->parent->y);
        dy = (dy > 0) ? 1 : (dy < 0 ? -1 : 0);

        if (dx != 0 && dy != 0)
        {
            if (IsWalkable(node->x, node->y + dy))
                neighbors.emplace_back(0, dy);
            if (IsWalkable(node->x + dx, node->y))
                neighbors.emplace_back(dx, 0);
            if (IsWalkable(node->x + dx, node->y + dy))
                neighbors.emplace_back(dx, dy);
            if (!IsWalkable(node->x - dx, node->y) && IsWalkable(node->x - dx, node->y + dy))
                neighbors.emplace_back(-dx, dy);
            if (!IsWalkable(node->x, node->y - dy) && IsWalkable(node->x + dx, node->y - dy))
                neighbors.emplace_back(dx, -dy);
        }
        else if (dx == 0)
        {
            if (IsWalkable(node->x, node->y + dy))
            {
                neighbors.emplace_back(0, dy);
                if (!IsWalkable(node->x + 1, node->y) && IsWalkable(node->x + 1, node->y + dy))
                    neighbors.emplace_back(1, dy);
                if (!IsWalkable(node->x - 1, node->y) && IsWalkable(node->x - 1, node->y + dy))
                    neighbors.emplace_back(-1, dy);
            }
        }
        else
        {
            if (IsWalkable(node->x + dx, node->y))
            {
                neighbors.emplace_back(dx, 0);
                if (!IsWalkable(node->x, node->y + 1) && IsWalkable(node->x + dx, node->y + 1))
                    neighbors.emplace_back(dx, 1);
                if (!IsWalkable(node->x, node->y - 1) && IsWalkable(node->x + dx, node->y - 1))
                    neighbors.emplace_back(dx, -1);
            }
        }
    }
    return neighbors;
}

std::vector<POINT> Game::JumpPointSearch()
{
    std::vector<POINT> path;
    if (mStartCell.x == -1 || mDestCell.x == -1) return path;

    auto cmp = [](const Node* a, const Node* b) { return a->f > b->f; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> open(cmp);
    std::unordered_map<int, Node*> allNodes;
    std::unordered_set<int> closed;

    auto index = [](int x, int y) { return y * GRID_COLS + x; };

    Node* start = new Node{ mStartCell.x, mStartCell.y, 0.0f, Heuristic(mStartCell.x, mStartCell.y, mDestCell.x, mDestCell.y), nullptr };
    open.push(start);
    allNodes[index(start->x, start->y)] = start;

    while (!open.empty())
    {
        Node* current = open.top();
        open.pop();

        int currentIdx = index(current->x, current->y);
        if (closed.count(currentIdx)) continue;
        closed.insert(currentIdx);

        if (current->x == mDestCell.x && current->y == mDestCell.y)
        {
            Node* n = current;
            while (n)
            {
                path.push_back({ n->x, n->y });
                n = n->parent;
            }
            std::reverse(path.begin(), path.end());
            break;
        }

        auto neighbors = GetNeighbors(current);
        for (auto& dir : neighbors)
        {
            POINT jp = Jump(current->x, current->y, dir.first, dir.second);
            if (jp.x == -1) continue;
            int id = index(jp.x, jp.y);
            if (closed.count(id)) continue;

            float g = current->g + std::hypot(jp.x - current->x, jp.y - current->y);
            Node* successor;
            auto it = allNodes.find(id);
            if (it == allNodes.end())
            {
                successor = new Node{ jp.x, jp.y, g, g + Heuristic(jp.x, jp.y, mDestCell.x, mDestCell.y), current };
                allNodes[id] = successor;
            }
            else
            {
                successor = it->second;
                if (g < successor->g)
                {
                    successor->g = g;
                    successor->f = g + Heuristic(jp.x, jp.y, mDestCell.x, mDestCell.y);
                    successor->parent = current;
                }
                else
                {
                    continue;
                }
            }
            open.push(successor);
        }
    }

    for (auto& p : allNodes) delete p.second;
    return path;
}

void Game::SolvePath()
{
    mPath = JumpPointSearch();
    mPathStep = mPath.empty() ? 0 : 1;
}

void Game::StepPath()
{
    if (mPathStep < mPath.size())
        ++mPathStep;
}