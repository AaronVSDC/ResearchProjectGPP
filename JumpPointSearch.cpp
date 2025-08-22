#include "JumpPointSearch.h"
#include "Game.h"
#include <cmath>

JumpPointSearch::JumpPointSearch(const std::vector<CellType>& grid, int cols, int rows)
    : mGrid(grid), mCols(cols), mRows(rows)
{
}

void JumpPointSearch::SetStart(POINT start)
{
    mStart = start;
}

void JumpPointSearch::SetGoal(POINT goal)
{
    mGoal = goal;
}

void JumpPointSearch::Reset()
{
    mPath.clear();
    mFinished = false;
    for (auto& pair : mNodes)
        delete pair.second;
    mNodes.clear();
    mClosed.clear();
    while (!mOpen.empty()) mOpen.pop();
}

std::vector<POINT> JumpPointSearch::Solve()
{
    while (!Step())
    {
    }
    return mPath;
}

bool JumpPointSearch::Step()
{
    if (mFinished) return true;

    if (mOpen.empty())
    {
        Node* startNode = GetNode(mStart.x, mStart.y);
        startNode->g = 0.0;
        startNode->h = Heuristic(mStart.x, mStart.y);
        mOpen.push(startNode);
    }

    if (mOpen.empty())
    {
        mFinished = true;
        return true; // no path
    }

    Node* current = mOpen.top();
    mOpen.pop();
    int currentIndex = Index(current->x, current->y);
    if (mClosed.count(currentIndex))
    {
        return false;
    }
    mClosed.insert(currentIndex);

    if (current->x == mGoal.x && current->y == mGoal.y)
    {
        mFinished = true;
        // reconstruct path
        mPath.clear();
        Node* n = current;
        while (n)
        {
            mPath.push_back(POINT{ n->x, n->y });
            n = n->parent;
        }
        std::reverse(mPath.begin(), mPath.end());
        return true;
    }

    auto neighbors = GetNeighbors(current);
    for (auto& dir : neighbors)
    {
        int nx = dir.first;
        int ny = dir.second;
        int dx = nx - current->x;
        int dy = ny - current->y;
        Node* jumpNode = Jump(nx, ny, dx, dy);
        if (!jumpNode) continue;
        int jIndex = Index(jumpNode->x, jumpNode->y);
        if (mClosed.count(jIndex)) continue;

        double newG = current->g + std::hypot(jumpNode->x - current->x, jumpNode->y - current->y);
        if (jumpNode->parent == nullptr || newG < jumpNode->g)
        {
            jumpNode->g = newG;
            jumpNode->h = Heuristic(jumpNode->x, jumpNode->y);
            jumpNode->parent = current;
            mOpen.push(jumpNode);
        }
    }

    return false;
}

JumpPointSearch::Node* JumpPointSearch::GetNode(int x, int y)
{
    int index = Index(x, y);
    auto it = mNodes.find(index);
    if (it != mNodes.end()) return it->second;
    Node* n = new Node{};
    n->x = x;
    n->y = y;
    mNodes[index] = n;
    return n;
}

bool JumpPointSearch::IsWalkable(int x, int y) const
{
    if (x < 0 || y < 0 || x >= mCols || y >= mRows) return false;
    CellType cell = mGrid[Index(x, y)];
    return cell != CellType::Obstacle;
}

double JumpPointSearch::Heuristic(int x, int y) const
{
    return std::abs(x - mGoal.x) + std::abs(y - mGoal.y);
}

JumpPointSearch::Node* JumpPointSearch::Jump(int x, int y, int dx, int dy)
{
    if (!IsWalkable(x, y)) return nullptr;
    if (x == mGoal.x && y == mGoal.y) return GetNode(x, y);

    // forced neighbor detection for 4-directional movement
    if (dx != 0)
    {
        if ((IsWalkable(x, y - 1) && !IsWalkable(x - dx, y - 1)) ||
            (IsWalkable(x, y + 1) && !IsWalkable(x - dx, y + 1)))
        {
            return GetNode(x, y);
        }
    }
    else if (dy != 0)
    {
        if ((IsWalkable(x - 1, y) && !IsWalkable(x - 1, y - dy)) ||
            (IsWalkable(x + 1, y) && !IsWalkable(x + 1, y - dy)))
        {
            return GetNode(x, y);
        }
    }

    if (dx != 0 && dy != 0)
    {
        // not used (4-directional)
    }

    return Jump(x + dx, y + dy, dx, dy);
}

std::vector<std::pair<int, int>> JumpPointSearch::GetNeighbors(Node* node)
{
    std::vector<std::pair<int, int>> neighbors;

    if (node->parent)
    {
        int dx = (node->x - node->parent->x); if (dx != 0) dx /= std::abs(dx);
        int dy = (node->y - node->parent->y); if (dy != 0) dy /= std::abs(dy);

        if (dx != 0)
        {
            if (IsWalkable(node->x + dx, node->y))
                neighbors.emplace_back(node->x + dx, node->y);
            if (!IsWalkable(node->x, node->y + 1) && IsWalkable(node->x + dx, node->y + 1))
                neighbors.emplace_back(node->x + dx, node->y + 1);
            if (!IsWalkable(node->x, node->y - 1) && IsWalkable(node->x + dx, node->y - 1))
                neighbors.emplace_back(node->x + dx, node->y - 1);
        }
        else if (dy != 0)
        {
            if (IsWalkable(node->x, node->y + dy))
                neighbors.emplace_back(node->x, node->y + dy);
            if (!IsWalkable(node->x + 1, node->y) && IsWalkable(node->x + 1, node->y + dy))
                neighbors.emplace_back(node->x + 1, node->y + dy);
            if (!IsWalkable(node->x - 1, node->y) && IsWalkable(node->x - 1, node->y + dy))
                neighbors.emplace_back(node->x - 1, node->y + dy);
        }
    }
    else
    {
        // return all neighbors
        const int dirs[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
        for (auto& d : dirs)
        {
            int nx = node->x + d[0];
            int ny = node->y + d[1];
            if (IsWalkable(nx, ny))
                neighbors.emplace_back(nx, ny);
        }
    }

    return neighbors;
}

