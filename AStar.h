#pragma once
#include <list>
#include "Grid.h"

class AStar final
{
public:

    AStar(Grid& grid)
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
    void EvaluateNeighbors();
    void SelectLowestCostNode();
    int CostOctile(Node* a, Node* b);
    void CalculateNodeCost(Node* currentNode, Node* neighborNode);

    const int m_CostStraight = 10;
    const int m_CostDiagonal = 14;
    Grid& m_Grid;
    std::list<Node*> m_OpenList;
    std::list<Node*> m_ClosedList;

    Node* m_CurrentNode = nullptr;
    Node* m_StartNode = nullptr;
    Node* m_DestinationNode = nullptr;
    Node* m_BacktrackNode = nullptr;
    bool m_IsBacktracking = false;

};