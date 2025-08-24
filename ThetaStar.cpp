#include "ThetaStar.h"
#include <algorithm>
#include <cmath>

#include "GameEngine.h"

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
void ThetaStar::Paint() const
{

    const int cellSize = m_Grid.GetCellSize();
    const int leftOffset = m_Grid.GetLeftOffset();
    const int topOffset = m_Grid.GetTopOffset();

    GAME_ENGINE->SetColor(RGB(255, 255, 214));

    for (size_t i = 1; i < m_Waypoints.size(); ++i)
    {
        const Node* a = m_Waypoints[i - 1];
        const Node* b = m_Waypoints[i];

        int x1 = a->column * cellSize + cellSize / 2 + leftOffset;
        int y1 = a->row * cellSize + cellSize / 2 + topOffset;
        int x2 = b->column * cellSize + cellSize / 2 + leftOffset;
        int y2 = b->row * cellSize + cellSize / 2 + topOffset;

        GAME_ENGINE->DrawLine(x1, y1, x2, y2);
    }
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
    m_Waypoints.clear();

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
        FinalizePathVisualization(); 
        m_IsBacktracking = false;
        m_CurrentNode = nullptr;
        return;
    }

    if (!m_BacktrackNode->parent)
    {
        FinalizePathVisualization();

        m_IsBacktracking = false;
        m_CurrentNode = nullptr;
        return;
    }

    if (!HasLineOfSight(m_BacktrackNode->parent, m_LastWaypoint))
    {
        if (m_BacktrackNode != m_StartNode and m_BacktrackNode != m_DestinationNode)
            m_BacktrackNode->nodeType = NodeType::Path;

        m_LastWaypoint = m_BacktrackNode;
    }

    m_BacktrackNode = m_BacktrackNode->parent;
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

bool ThetaStar::HasLineOfSight(const Node* a, const Node* b) const
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
std::vector<Node*> ThetaStar::ReconstructPathChain() const
{
    std::vector<Node*> chain;
    const Node* n = m_DestinationNode;
    if (!n) return chain;
    while (n) {
        chain.push_back(const_cast<Node*>(n));
        if (n == m_StartNode) break;
        n = n->parent;
    }
    //std::reverse(chain.begin(), chain.end());
    //if (chain.empty() || chain.front() != m_StartNode || chain.back() != m_DestinationNode)
    //    chain.clear();
    return chain;
}

std::vector<Node*> ThetaStar::ExtractWaypoints(const std::vector<Node*>& chain) const
{
    std::vector<Node*> waypoint;
    if (chain.empty()) return waypoint;

    Node* last = chain.front();    // start
    waypoint.push_back(last); 

    Node* previous = last;
    for (size_t i = 1; i < chain.size(); ++i)
    {
        Node* current = chain[i];
        if (!HasLineOfSight(last, current)) 
        { 
            // 'prev' is the farthest still visible from 'last'
            if (previous != waypoint.back()) waypoint.push_back(previous);
            last = previous;
        }
        previous = current;
    }
    if (waypoint.back() != chain.back()) waypoint.push_back(chain.back()); 
    return waypoint;
}

void ThetaStar::FinalizePathVisualization()
{
    auto chain = ReconstructPathChain();
    auto waypoint = ExtractWaypoints(chain);
    m_Waypoints = std::move(waypoint);  

}


