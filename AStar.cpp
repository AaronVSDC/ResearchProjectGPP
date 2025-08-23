#include "AStar.h"

void AStar::Start()
{
    FindStartNode();

}

void AStar::Tick()
{
	auto& grid = m_Grid.GetNodes();




}

void AStar::Step()
{




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
                m_CurrentNode = nodes[row][column]; //push startNode in the openList
                m_CurrentNode->open = true; 
                m_OpenList.push_back(m_CurrentNode);
            }
        }
    }
}

void AStar::EvaluateNeighbors()
{


}
