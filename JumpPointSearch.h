#pragma once
#include <list>
#include <vector>
#include "Grid.h"

class JumpPointSearch final
{
public: 
    JumpPointSearch(Grid& grid)
        :m_Grid{ grid } {}

    void Start();
    void Tick();
    void Reset();

private:
    void Initialize();
    void Step();
    void BacktrackStep();
    void FindStartNode();
    void FindDestinationNode();
    void IdentifySuccessors();
    std::vector<std::pair<int, int>> FindNeighbors(Node* node);
    Node* Jump(int row, int column, int deltaRow, int deltaColumn);
    void CalculateNodeCost(Node* fromNode, Node* toNode);
    void SelectLowestCostNode();

    Grid& m_Grid;
    std::list<Node*> m_OpenList;
    std::list<Node*> m_ClosedList;

    Node* m_CurrentNode = nullptr;
    Node* m_StartNode = nullptr;
    Node* m_DestinationNode = nullptr;
    Node* m_BacktrackNode = nullptr;
    bool m_IsBacktracking = false;
};
