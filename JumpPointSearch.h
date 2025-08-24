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
    void Paint() const;

private:
    void Initialize();
    void Step();
    void BuildFinalPath(); 
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

    struct Arrow
    {
        Node* from{};
        Node* to{};
    };
    std::vector<Arrow> m_JumpArrows;
};