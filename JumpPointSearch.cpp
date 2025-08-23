#include "JumpPointSearch.h"
#include <algorithm>
#include <cmath>
#undef min
#undef max 
void JumpPointSearch::Start()
{
    FindStartNode();
    FindDestinationNode();
    Initialize();
}

void JumpPointSearch::Tick()
{
    Step();
}

void JumpPointSearch::Reset()
{
    m_OpenList.clear();
    m_ClosedList.clear();
    m_CurrentNode = nullptr;
    m_StartNode = nullptr;
    m_DestinationNode = nullptr;
    m_BacktrackNode = nullptr;
    m_IsBacktracking = false;
}

void JumpPointSearch::Initialize()
{
    if (!m_StartNode || !m_DestinationNode)
        return;

    m_StartNode->gScore = 0;

    const int costStraight = 10;
    const int costDiagonal = 14;

    auto Heuristic = [&](Node* a, Node* b)
        {
            int differenceColumn = std::abs(a->column - b->column);
            int differenceRow = std::abs(a->row - b->row);
            int minimumDifference = std::min(differenceColumn, differenceRow);
            int maximumDifference = std::max(differenceColumn, differenceRow);
            return costDiagonal * minimumDifference + costStraight * (maximumDifference - minimumDifference);
        };

    m_StartNode->hScore = Heuristic(m_StartNode, m_DestinationNode);
    m_StartNode->fScore = m_StartNode->hScore;
    m_CurrentNode = m_StartNode;
}

void JumpPointSearch::Step()
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

    IdentifySuccessors();
    SelectLowestCostNode();
}

void JumpPointSearch::BacktrackStep()
{
    if (!m_BacktrackNode)
        return;

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

void JumpPointSearch::FindStartNode()
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

void JumpPointSearch::FindDestinationNode()
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

void JumpPointSearch::IdentifySuccessors()
{
    if (!m_CurrentNode)
        return;

    std::vector<std::pair<int, int>> neighborDirections = FindNeighbors(m_CurrentNode);

    for (const std::pair<int, int>& direction : neighborDirections)
    {
        int deltaRow = direction.first;
        int deltaColumn = direction.second;

        Node* jumpNode = Jump(m_CurrentNode->row + deltaRow, m_CurrentNode->column + deltaColumn, deltaRow, deltaColumn);

        if (jumpNode != nullptr && !jumpNode->closed)
        {
            CalculateNodeCost(m_CurrentNode, jumpNode);

            if (!jumpNode->open)
            {
                jumpNode->open = true;
                m_OpenList.push_back(jumpNode);
            }
        }
    }

    m_OpenList.remove(m_CurrentNode);
    m_CurrentNode->closed = true;
    m_CurrentNode->open = false;
    m_ClosedList.push_back(m_CurrentNode);
}

std::vector<std::pair<int, int>> JumpPointSearch::FindNeighbors(Node* node)
{
    std::vector<std::pair<int, int>> neighborOffsets;

    auto& nodes = m_Grid.GetNodes();
    const int totalRows = m_Grid.GetAmountOfGridRows();
    const int totalColumns = m_Grid.GetAmountOfGridCols();

    auto InBounds = [&](int row, int column)
        {
            return row >= 0 && row < totalRows && column >= 0 && column < totalColumns;
        };

    auto IsPassable = [&](int row, int column)
        {
            if (!InBounds(row, column))
                return false;
            return nodes[row][column]->nodeType != NodeType::Obstacle;
        };

    Node* parentNode = node->parent;

    if (parentNode == nullptr)
    {
        for (int rowOffset = -1; rowOffset <= 1; ++rowOffset)
        {
            for (int columnOffset = -1; columnOffset <= 1; ++columnOffset)
            {
                if (rowOffset == 0 && columnOffset == 0)
                    continue;
                int newRow = node->row + rowOffset;
                int newColumn = node->column + columnOffset;
                if (IsPassable(newRow, newColumn))
                    neighborOffsets.emplace_back(rowOffset, columnOffset);
            }
        }
        return neighborOffsets;
    }

    int deltaRow = node->row - parentNode->row;
    int deltaColumn = node->column - parentNode->column;

    if (deltaRow != 0)
        deltaRow /= std::abs(deltaRow);
    if (deltaColumn != 0)
        deltaColumn /= std::abs(deltaColumn);

    if (deltaRow != 0 && deltaColumn != 0)
    {
        if (IsPassable(node->row, node->column + deltaColumn))
            neighborOffsets.emplace_back(0, deltaColumn);
        if (IsPassable(node->row + deltaRow, node->column))
            neighborOffsets.emplace_back(deltaRow, 0);
        if (IsPassable(node->row + deltaRow, node->column) && IsPassable(node->row, node->column + deltaColumn))
            neighborOffsets.emplace_back(deltaRow, deltaColumn);

        if (!IsPassable(node->row - deltaRow, node->column) && IsPassable(node->row - deltaRow, node->column + deltaColumn))
            neighborOffsets.emplace_back(-deltaRow, deltaColumn);
        if (!IsPassable(node->row, node->column - deltaColumn) && IsPassable(node->row + deltaRow, node->column - deltaColumn))
            neighborOffsets.emplace_back(deltaRow, -deltaColumn);
    }
    else if (deltaRow == 0)
    {
        if (IsPassable(node->row, node->column + deltaColumn))
            neighborOffsets.emplace_back(0, deltaColumn);

        if (!IsPassable(node->row + 1, node->column) && IsPassable(node->row + 1, node->column + deltaColumn))
            neighborOffsets.emplace_back(1, deltaColumn);
        if (!IsPassable(node->row - 1, node->column) && IsPassable(node->row - 1, node->column + deltaColumn))
            neighborOffsets.emplace_back(-1, deltaColumn);
    }
    else if (deltaColumn == 0)
    {
        if (IsPassable(node->row + deltaRow, node->column))
            neighborOffsets.emplace_back(deltaRow, 0);

        if (!IsPassable(node->row, node->column + 1) && IsPassable(node->row + deltaRow, node->column + 1))
            neighborOffsets.emplace_back(deltaRow, 1);
        if (!IsPassable(node->row, node->column - 1) && IsPassable(node->row + deltaRow, node->column - 1))
            neighborOffsets.emplace_back(deltaRow, -1);
    }

    return neighborOffsets;
}

Node* JumpPointSearch::Jump(int row, int column, int deltaRow, int deltaColumn)
{
    auto& nodes = m_Grid.GetNodes();
    const int totalRows = m_Grid.GetAmountOfGridRows();
    const int totalColumns = m_Grid.GetAmountOfGridCols();

    auto InBounds = [&](int r, int c)
        {
            return r >= 0 && r < totalRows && c >= 0 && c < totalColumns;
        };

    auto IsPassable = [&](int r, int c)
        {
            if (!InBounds(r, c))
                return false;
            return nodes[r][c]->nodeType != NodeType::Obstacle;
        };

    if (!InBounds(row, column) || !IsPassable(row, column))
        return nullptr;

    Node* node = nodes[row][column];

    if (node == m_DestinationNode)
        return node;

    if (deltaRow != 0 && deltaColumn != 0)
    {
        if ((!IsPassable(row - deltaRow, column) && IsPassable(row - deltaRow, column + deltaColumn)) ||
            (!IsPassable(row, column - deltaColumn) && IsPassable(row + deltaRow, column - deltaColumn)))
            return node;

        if (Jump(row + deltaRow, column, deltaRow, 0) != nullptr ||
            Jump(row, column + deltaColumn, 0, deltaColumn) != nullptr)
            return node;
    }
    else if (deltaRow != 0)
    {
        if ((!IsPassable(row, column + 1) && IsPassable(row + deltaRow, column + 1)) ||
            (!IsPassable(row, column - 1) && IsPassable(row + deltaRow, column - 1)))
            return node;
    }
    else if (deltaColumn != 0)
    {
        if ((!IsPassable(row + 1, column) && IsPassable(row + 1, column + deltaColumn)) ||
            (!IsPassable(row - 1, column) && IsPassable(row - 1, column + deltaColumn)))
            return node;
    }

    return Jump(row + deltaRow, column + deltaColumn, deltaRow, deltaColumn);
}

void JumpPointSearch::CalculateNodeCost(Node* fromNode, Node* toNode)
{
    const int costStraight = 10;
    const int costDiagonal = 14;

    auto Heuristic = [&](Node* a, Node* b)
        {
            int differenceColumn = std::abs(a->column - b->column);
            int differenceRow = std::abs(a->row - b->row);
            int minimumDifference = std::min(differenceColumn, differenceRow);
            int maximumDifference = std::max(differenceColumn, differenceRow);
            return costDiagonal * minimumDifference + costStraight * (maximumDifference - minimumDifference);
        };

    bool diagonalMovement = (fromNode->row != toNode->row) && (fromNode->column != toNode->column);
    int stepCost = diagonalMovement ? costDiagonal : costStraight;

    int baseCost = fromNode ? fromNode->gScore : 0;
    int tentativeGScore = (baseCost >= (INT_MAX / 8)) ? baseCost : baseCost + stepCost;

    if (!toNode->open || tentativeGScore < toNode->gScore)
    {
        toNode->parent = fromNode;
        toNode->gScore = tentativeGScore;
        toNode->hScore = Heuristic(toNode, m_DestinationNode);
        toNode->fScore = toNode->gScore + toNode->hScore;
    }
}

void JumpPointSearch::SelectLowestCostNode()
{
    if (m_OpenList.empty())
    {
        m_CurrentNode = nullptr;
        return;
    }

    auto iterator = std::min_element(
        m_OpenList.begin(), m_OpenList.end(),
        [](const Node* left, const Node* right)
        {
            if (left->fScore != right->fScore) return left->fScore < right->fScore;
            if (left->hScore != right->hScore) return left->hScore < right->hScore;
            return left->gScore > right->gScore;
        });

    m_CurrentNode = (iterator != m_OpenList.end()) ? *iterator : nullptr;
}