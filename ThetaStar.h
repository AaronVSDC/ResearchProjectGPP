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
    void Paint() const;
    void Reset();

private:
    void Initialize();
    void Step();
    void BacktrackStep();
    void FindStartNode();
    void FindDestinationNode();
    int CostEuclid(const Node* a, const Node* b) const;
    bool IsNodePassable(int row, int col) const;
    bool HasLineOfSight(const Node* a, const Node* b) const;
    void EvaluateNeighbors();
    void SelectLowestCostNode();
    void CalculateNodeCost(Node* currentNode, Node* neighborNode);

    std::vector<Node*> ReconstructPathChain() const;
    std::vector<Node*> ExtractWaypoints(const std::vector<Node*>& chain) const;
    void FinalizePathVisualization();

    Grid& m_Grid;
    std::list<Node*> m_OpenList;
    std::list<Node*> m_ClosedList;

    Node* m_CurrentNode = nullptr;
    Node* m_StartNode = nullptr;
    Node* m_DestinationNode = nullptr;
    Node* m_BacktrackNode = nullptr;
    Node* m_LastWaypoint = nullptr; 
    bool m_IsBacktracking = false;
    std::vector<Node*> m_Waypoints;

};