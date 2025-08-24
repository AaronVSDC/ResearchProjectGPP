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

        const int x1 = a->column * cellSize + cellSize / 2 + leftOffset;
        const int y1 = a->row * cellSize + cellSize / 2 + topOffset;
        const int x2 = b->column * cellSize + cellSize / 2 + leftOffset;
        const int y2 = b->row * cellSize + cellSize / 2 + topOffset;

        GAME_ENGINE->DrawLine(x1, y1, x2, y2, 5);
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
    m_StartNode->hScore = CostEuclidian(m_StartNode, m_DestinationNode);
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
        m_IsBacktracking = false;
        m_CurrentNode = nullptr;
        return;
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
int ThetaStar::CostEuclidian(const Node* a, const Node* b) const
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
    double x0 = a->column + 0.5;
    double y0 = a->row + 0.5;
    double x1 = b->column + 0.5;
    double y1 = b->row + 0.5;

    int x = a->column;
    int y = a->row;

    const int tx = b->column;
    const int ty = b->row;

    const double dx = x1 - x0;
    const double dy = y1 - y0;

    const int stepX = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
    const int stepY = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;

    const double tDeltaX = (stepX != 0) ? 1.0 / std::abs(dx) : std::numeric_limits<double>::infinity();
    const double tDeltaY = (stepY != 0) ? 1.0 / std::abs(dy) : std::numeric_limits<double>::infinity();

    const double nextV = (stepX > 0) ? (std::floor(x0) + 1.0) : (std::ceil(x0) - 1.0);
    const double nextH = (stepY > 0) ? (std::floor(y0) + 1.0) : (std::ceil(y0) - 1.0);

    double tMaxX = (stepX != 0) ? std::abs(nextV - x0) * tDeltaX : std::numeric_limits<double>::infinity();
    double tMaxY = (stepY != 0) ? std::abs(nextH - y0) * tDeltaY : std::numeric_limits<double>::infinity();

    if (!IsNodePassable(y, x) || !IsNodePassable(ty, tx))
        return false;

    while (x != tx || y != ty)
    {
        if (tMaxX < tMaxY) 
        {
            x += stepX;
            tMaxX += tDeltaX;
            if (!IsNodePassable(y, x)) return false;
        }
        else if (tMaxY < tMaxX) {
            y += stepY;
            tMaxY += tDeltaY;
            if (!IsNodePassable(y, x)) return false;
        }
        else {
            const int xn = x + stepX;
            const int yn = y + stepY;

            if (!IsNodePassable(y, xn) and !IsNodePassable(yn, x))
                return false;

            x = xn; y = yn;
            tMaxX += tDeltaX;
            tMaxY += tDeltaY;

            if (!IsNodePassable(y, x)) return false;
        }
    }
    return true;
}

void ThetaStar::EvaluateNeighbors()
{
    auto& nodes = m_Grid.GetNodes();

    auto inBounds = [&](int row, int column) { return row >= 0 and row < m_Grid.GetAmountOfGridRows() and column >= 0 and column < m_Grid.GetAmountOfGridCols(); }; 
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

void ThetaStar::CalculateNodeCost(Node* currentNode, Node* expandedNode)
{
    const int gViaCurrentNode = currentNode->gScore + CostEuclidian(currentNode, expandedNode);

    int bestG = gViaCurrentNode;
    Node* bestParent = currentNode;

    if (HasLineOfSight(currentNode->parent, expandedNode))
    {
        const int gViaParent = currentNode->parent->gScore + CostEuclidian(currentNode->parent, expandedNode);
        if (gViaParent < bestG)
        {
            bestG = gViaParent;  
            bestParent = currentNode->parent;
        }
    }
     
    if (!expandedNode->open or bestG < expandedNode->gScore)
    {
        expandedNode->parent = bestParent; 
        expandedNode->gScore = bestG; 
        expandedNode->hScore = CostEuclidian(expandedNode, m_DestinationNode);
        expandedNode->fScore = expandedNode->gScore + expandedNode->hScore;
    }

}
std::vector<Node*> ThetaStar::ReconstructPathChain()
{
    std::vector<Node*> chain;
    while (m_DestinationNode) 
    {
        chain.push_back(m_DestinationNode);
        if (m_DestinationNode == m_StartNode) break; 
        m_DestinationNode = m_DestinationNode->parent; 
    }

    if (chain.empty() or chain.back() != m_StartNode)
        return {};          

    std::ranges::reverse(chain.begin(), chain.end());

    for (Node* node : chain)
    { 
        if (node != m_StartNode && node != m_DestinationNode)
        {
            node->nodeType = NodeType::Path;
        }
    }
    return chain;
}

std::vector<Node*> ThetaStar::ExtractWaypoints(const std::vector<Node*>& chain) const
{
    std::vector<Node*> waypoint;
    if (chain.empty()) return waypoint;

    Node* last = chain.front(); 
    waypoint.push_back(last); 

    Node* previous = last;
    for (size_t i = 1; i < chain.size(); ++i)
    {
        Node* current = chain[i];
        if (!HasLineOfSight(last, current)) 
        { 
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


