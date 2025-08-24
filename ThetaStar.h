#pragma once
#include <list>
#include "Grid.h"

class ThetaStar final
{
public:
    explicit ThetaStar(Grid& grid)
        : m_Grid{ grid } {}

    void Start();
    void Tick();
    void Reset();

private:
    void Initialize();
    void Step();
    void BacktrackStep();
    void FindStartNode();
    void FindDestinationNode();
    int CostEuclid(const Node* a, const Node* b) const;
    bool IsNodePassable(int row, int col) const;
    bool HasLineOfSight(const Node* a, const Node* b);
    void EvaluateNeighbors();
    void SelectLowestCostNode();
    void CalculateNodeCost(Node* currentNode, Node* neighborNode);

    Grid& m_Grid;
    std::list<Node*> m_OpenList;
    std::list<Node*> m_ClosedList;

    Node* m_CurrentNode = nullptr;
    Node* m_StartNode = nullptr;
    Node* m_DestinationNode = nullptr;
    Node* m_BacktrackNode = nullptr;
    Node* m_LastWaypoint = nullptr;
    bool m_IsBacktracking = false;
};