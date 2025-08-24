#include "ThetaStar.h"
#include <algorithm>
#include <cmath>

#undef min
#undef max

void ThetaStar::Start()
{
    FindStartNode();
    FindDestinationNode();
    Initialize();
}

void ThetaStar::Tick()
{
    Step();
}

void ThetaStar::Reset()
{
    m_OpenList.clear();
    m_ClosedList.clear();
    m_CurrentNode = nullptr;
    m_StartNode = nullptr;
    m_DestinationNode = nullptr;
    m_BacktrackNode = nullptr;
    m_IsBacktracking = false;
}

void ThetaStar::Initialize()
{
    m_StartNode->gScore = 0;
    m_StartNode->parent = m_StartNode; 
    m_StartNode->hScore = CostEuclid(m_StartNode, m_DestinationNode);
    m_StartNode->fScore = m_StartNode->hScore;

}

void ThetaStar::Step()
{
    if (!m_CurrentNode)
        return;

    if (m_IsBacktracking)
    {
        BacktrackStep();
        return;
    }

    if (m_CurrentNode == m_DestinationNode)
    {
        m_IsBacktracking = true;
        m_BacktrackNode = m_CurrentNode; 
        m_LastWaypoint = m_DestinationNode; 
        return;
    }

    EvaluateNeighbors();
    SelectLowestCostNode();
}

void ThetaStar::BacktrackStep()
{
    if (!m_BacktrackNode) return;

    if (m_BacktrackNode == m_StartNode)
    {
        // optionally mark start as a waypoint, or not
        m_IsBacktracking = false;
        m_CurrentNode = nullptr;
        return;
    }

    Node* n = m_BacktrackNode;
    Node* p = n->parent;
    if (!p)
    {
        m_IsBacktracking = false;
        m_CurrentNode = nullptr;
        return;
    }

    // If the parent can't see the last chosen waypoint,
    // 'n' is the next waypoint to keep.
    if (!HasLineOfSight(p, m_LastWaypoint))
    {
        if (n != m_StartNode && n != m_DestinationNode)
            n->nodeType = NodeType::Path; // mark the waypoint node

        m_LastWaypoint = n;
    }

    // move up the chain
    m_BacktrackNode = p;
}

void ThetaStar::FindStartNode()
{
    auto& nodes = m_Grid.GetNodes();
    for (size_t row = 0; row < nodes.size(); ++row)
    {
        for (size_t column = 0; column < nodes[row].size(); ++column)
        {
            if (nodes[row][column]->nodeType == NodeType::Start)
            {
                m_StartNode = nodes[row][column];
                m_CurrentNode = m_StartNode;
                m_StartNode->open = true;
                m_OpenList.push_back(m_StartNode);
            }
        }
    }
}

void ThetaStar::FindDestinationNode()
{
    auto& nodes = m_Grid.GetNodes();
    for (size_t row = 0; row < nodes.size(); ++row)
    {
        for (size_t column = 0; column < nodes[row].size(); ++column)
        {
            if (nodes[row][column]->nodeType == NodeType::Destination)
            {
                m_DestinationNode = nodes[row][column];
            }
        }
    }
}
int ThetaStar::CostEuclid(const Node* a, const Node* b) const
{
    const int dx = a->column - b->column;
    const int dy = a->row - b->row;
    return static_cast<int>(std::lround(10.0 * std::sqrt(static_cast<double>(dx * dx + dy * dy)))); 
}
bool ThetaStar::IsNodePassable(int row, int col) const
{
    if (row < 0 || col < 0 ||
        row >= m_Grid.GetAmountOfGridRows() ||
        col >= m_Grid.GetAmountOfGridCols()) return false;

    const auto& nodes = m_Grid.GetNodes();
    return nodes[row][col]->nodeType != NodeType::Obstacle;
}

bool ThetaStar::HasLineOfSight(const Node* a, const Node* b)
{
    //using the integer bresenham with corner checks algorithm for light of sight checks (from what ive checked a very common algorithm to do this)
    //note: this rejects lines that would squeeze between two obstacle nodes at a corner
    int x0 = a->column, y0 = a->row;
    const int x1 = b->column, y1 = b->row;

    int deltaX = std::abs(x1 - x0);
    int deltaY = std::abs(y1 - y0);
    int stepX = (x1 < x0) ? -1 : 1;
    int stepY = (y1 < y0) ? -1 : 1;

    int f = 0;

    if (deltaX >= deltaY)
    {
        while (x0 != x1)
        {
            f += deltaY;
            if (f >= deltaX)
            {
                if (!IsNodePassable(y0 + ((stepY - 1) / 2), x0 + ((stepX - 1) / 2)))
                    return false;
                y0 += stepY;
                f -= deltaX;
            }
            if (f != 0 && !IsNodePassable(y0 + ((stepY - 1) / 2), x0 + ((stepX - 1) / 2)))
                return false;

            if (deltaY == 0 &&
                (!IsNodePassable(y0, x0 + ((stepX - 1) / 2)) ||
                    !IsNodePassable(y0 - 1, x0 + ((stepX - 1) / 2))))
                return false;

            x0 += stepX;
            if (!IsNodePassable(y0, x0)) return false;
        }
    }
    else
    {
        while (y0 != y1)
        {
            f += deltaX;
            if (f >= deltaY)
            {
                if (!IsNodePassable(y0 + ((stepY - 1) / 2), x0 + ((stepX - 1) / 2)))
                    return false;
                x0 += stepX;
                f -= deltaY;
            }
            if (f != 0 && !IsNodePassable(y0 + ((stepY - 1) / 2), x0 + ((stepX - 1) / 2)))
                return false;

            if (deltaX == 0 &&
                (!IsNodePassable(y0 + ((stepY - 1) / 2), x0) ||
                    !IsNodePassable(y0 + ((stepY - 1) / 2), x0 - 1)))
                return false;

            y0 += stepY;
            if (!IsNodePassable(y0, x0)) return false;
        }
    }
    return true;
}

void ThetaStar::EvaluateNeighbors()
{
    auto& nodes = m_Grid.GetNodes();
    const int rows = m_Grid.GetAmountOfGridRows();
    const int cols = m_Grid.GetAmountOfGridCols();

    auto inBounds = [&](int row, int column) { return row >= 0 && row < rows && column >= 0 && column < cols; };
    auto isPassable = [&](int row, int column) { if (!inBounds(row, column)) return false; return nodes[row][column]->nodeType != NodeType::Obstacle; };

    auto tryNeighbor = [&](int row, int column, bool diagonal)
        {
            if (!inBounds(row, column)) return;

            if (diagonal)
            {
                if (!(isPassable(m_CurrentNode->row, column) or isPassable(row, m_CurrentNode->column)))
                    return;
            }

            Node* neighbor = nodes[row][column]; 
            if (!isPassable(row, column))
                return;
            if (neighbor->closed) return;

            CalculateNodeCost(m_CurrentNode, neighbor);

            if (!neighbor->open)
            {
                neighbor->open = true;
                m_OpenList.push_back(neighbor);
            }
        };

    tryNeighbor(m_CurrentNode->row - 1, m_CurrentNode->column, false); // up
    tryNeighbor(m_CurrentNode->row + 1, m_CurrentNode->column, false); // down
    tryNeighbor(m_CurrentNode->row, m_CurrentNode->column + 1, false); // right
    tryNeighbor(m_CurrentNode->row, m_CurrentNode->column - 1, false); // left
    tryNeighbor(m_CurrentNode->row - 1, m_CurrentNode->column - 1, true); // upLeft
    tryNeighbor(m_CurrentNode->row - 1, m_CurrentNode->column + 1, true); // upRight
    tryNeighbor(m_CurrentNode->row + 1, m_CurrentNode->column + 1, true); // downRight
    tryNeighbor(m_CurrentNode->row + 1, m_CurrentNode->column - 1, true); // downLeft

    m_OpenList.remove(m_CurrentNode);
    m_CurrentNode->closed = true;
    m_CurrentNode->open = false;
    m_ClosedList.push_back(m_CurrentNode); 
}

void ThetaStar::SelectLowestCostNode()
{
    if (m_OpenList.empty()) { m_CurrentNode = nullptr; return; }

    auto it = std::ranges::min_element(
        m_OpenList.begin(), m_OpenList.end(), 
        [](const Node* a, const Node* b)
        {
            if (a->fScore != b->fScore) return a->fScore < b->fScore;
            if (a->hScore != b->hScore) return a->hScore < b->hScore;
            return a->gScore > b->gScore;
        });

    m_CurrentNode = (it != m_OpenList.end()) ? *it : nullptr;
}

void ThetaStar::CalculateNodeCost(Node* currentNode, Node* neighborNode)
{
    const int gViaCurrentNode = currentNode->gScore + CostEuclid(currentNode, neighborNode);

    int bestG = gViaCurrentNode;
    Node* bestParent = currentNode;

    if (currentNode->parent and currentNode->parent != currentNode and HasLineOfSight(currentNode->parent, neighborNode))
    {
        const int gViaParent = currentNode->parent->gScore + CostEuclid(currentNode->parent, neighborNode);
        if (gViaParent < bestG)
        {
            bestG = gViaParent;  
            bestParent = currentNode->parent;
        }
    }

    if (!neighborNode->open || bestG < neighborNode->gScore)
    {
        neighborNode->parent = bestParent;
        neighborNode->gScore = bestG; 
        neighborNode->hScore = CostEuclid(neighborNode, m_DestinationNode);
        neighborNode->fScore = neighborNode->gScore + neighborNode->hScore;
    }

}


