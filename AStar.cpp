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

    m_StartNode->hScore = CostOctile(m_StartNode, m_DestinationNode);
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

void AStar::SelectLowestCostNode()
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
int AStar::CostOctile(Node* a, Node* b)
{
        const int deltaX = std::abs(a->column - b->column);
        const int deltaY = std::abs(a->row - b->row);
        const int min = std::min(deltaX, deltaY); 
        const int max = std::max(deltaX, deltaY);
        return m_CostDiagonal * min + m_CostStraight * (max - min);
};
void AStar::CalculateNodeCost(Node* currentNode, Node* neighborNode)
{
	const bool diagonal = currentNode->row != neighborNode->row and currentNode->column != neighborNode->column;
	const int stepCost = diagonal ? m_CostDiagonal : m_CostStraight;

    const int baseG = currentNode->gScore;
    const int tentativeG = baseG >= INT_MAX / 8 ? baseG : baseG + stepCost;

    if (!neighborNode->open or tentativeG < neighborNode->gScore)
    {
        neighborNode->parent = currentNode; 
        neighborNode->gScore = tentativeG;
        neighborNode->hScore = CostOctile(neighborNode, m_DestinationNode);
        neighborNode->fScore = neighborNode->gScore + neighborNode->hScore; 
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