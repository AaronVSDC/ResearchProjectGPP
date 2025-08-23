#include "AStar.h"
#include <numbers>
#include <algorithm>
#undef min
#undef max 
void AStar::Start()
{
    FindStartNode();
    FindDestinationNode();
    Initialize(); 
}

void AStar::Tick()
{
    Step(); 
}

void AStar::Initialize()
{
    m_StartNode->gScore = 0;

    auto octile = [&](Node* a, Node* b)
        {
            constexpr int costStraight = 10;
            constexpr int costDiagonal = 14;
            const int dx = std::abs(a->column - b->column);
            const int dy = std::abs(a->row - b->row);
            const int mn = std::min(dx, dy);
            const int mx = std::max(dx, dy);
            return costDiagonal * mn + costStraight * (mx - mn);
        };
    m_StartNode->hScore = octile(m_StartNode, m_DestinationNode);
    m_StartNode->fScore = m_StartNode->hScore; 
}

void AStar::Step()
{
    if (!m_CurrentNode) return;

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

void AStar::FindStartNode()
{
    auto& nodes = m_Grid.GetNodes();
    for (size_t row = 0; row < nodes.size(); ++row)
    {
        for (size_t column = 0; column < nodes[row].size(); ++column)
        {
            if (nodes[row][column]->nodeType == NodeType::Start)
            {
                m_StartNode = nodes[row][column]; //push startNode in the openList
                m_CurrentNode = m_StartNode;
                m_CurrentNode->open = true;
                m_OpenList.push_back(m_CurrentNode);
            }
        }
    }
}

void AStar::FindDestinationNode()
{
    auto& nodes = m_Grid.GetNodes();
    for (size_t row = 0; row < nodes.size(); ++row)
    {
        for (size_t column = 0; column < nodes[row].size(); ++column)
        {
            if (nodes[row][column]->nodeType == NodeType::Destination)
            {
                m_DestinationNode = nodes[row][column]; //push startNode in the openList
            }
        }
    }
}
void AStar::EvaluateNeighbors()
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
                int cr = m_CurrentNode->row;
                int cc = m_CurrentNode->column;
                if (!(isPassable(cr, column) or isPassable(row, cc))) return;
            }

            Node* neighbor = nodes[row][column];
            if (neighbor->nodeType == NodeType::Obstacle) return;
            if (neighbor->closed) return;

            CalculateNodeCost(m_CurrentNode, neighbor);

            if (!neighbor->open)
            {
                neighbor->open = true;
                m_OpenList.push_back(neighbor);
            }
        };

    const int r = m_CurrentNode->row;
    const int c = m_CurrentNode->column;

    tryNeighbor(r - 1, c, false); // up
    tryNeighbor(r + 1, c, false); // down
    tryNeighbor(r, c + 1, false); // right
    tryNeighbor(r, c - 1, false); // left
	tryNeighbor(r - 1, c - 1, true); // upLeft
    tryNeighbor(r - 1, c + 1, true); // upRight
    tryNeighbor(r + 1, c + 1, true); // downRight
    tryNeighbor(r + 1, c - 1, true); // downLeft

    m_OpenList.remove(m_CurrentNode);
    m_CurrentNode->closed = true;
    m_CurrentNode->open = false;
    m_ClosedList.push_back(m_CurrentNode);
}

void AStar::SelectLowestCostNode()
{
    if (m_OpenList.empty()) { m_CurrentNode = nullptr; return; }

    auto it = std::min_element(
        m_OpenList.begin(), m_OpenList.end(),
        [](const Node* a, const Node* b) 
        {
            if (a->fScore != b->fScore) return a->fScore < b->fScore;
            if (a->hScore != b->hScore) return a->hScore < b->hScore;
            return a->gScore > b->gScore; 
        });

    m_CurrentNode = (it != m_OpenList.end()) ? *it : nullptr;
}

void AStar::CalculateNodeCost(Node* from, Node* node)
{
    constexpr int costStraight = 10, costDiagonal = 14;

    auto heuristicOctile = [&](Node* a, Node* b)
        {
            const int dx = std::abs(a->column - b->column);
            const int dy = std::abs(a->row - b->row);
            const int mn = std::min(dx, dy);
            const int mx = std::max(dx, dy); 
            return costDiagonal * mn + costStraight * (mx - mn);
        };

    int stepCost = costStraight;
    if (from)
    {
        const bool diagonal = (from->row != node->row) && (from->column != node->column);
        stepCost = diagonal ? costDiagonal : costStraight;
    }

    const int baseG = from ? from->gScore : 0;
    const int tentativeG = (baseG >= (INT_MAX / 8)) ? baseG : baseG + stepCost;

    if (!node->open || tentativeG < node->gScore)
    {
        node->parent = from;
        node->gScore = tentativeG;
        node->hScore = heuristicOctile(node, m_DestinationNode);
        node->fScore = node->gScore + node->hScore;
    }
}

void AStar::BacktrackStep()
{
    if (!m_BacktrackNode) return;

    if (m_BacktrackNode->parent && m_BacktrackNode->parent != m_StartNode)
    {
        m_BacktrackNode->parent->nodeType = NodeType::Path;
    }

    m_BacktrackNode = m_BacktrackNode->parent;

    if (!m_BacktrackNode || m_BacktrackNode == m_StartNode)
    {
        m_IsBacktracking = false;
        m_CurrentNode = nullptr;
    }
}

void AStar::Reset()
{
    m_OpenList.clear();
    m_ClosedList.clear();
    m_CurrentNode = nullptr;
    m_StartNode = nullptr;
    m_DestinationNode = nullptr;
    m_BacktrackNode = nullptr;
    m_IsBacktracking = false;
}