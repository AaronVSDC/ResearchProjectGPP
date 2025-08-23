#include "AStar.h"
#include <numbers>
void AStar::Start()
{
    FindStartNode();
    FindDestinationNode(); 
}

void AStar::Tick()
{
	auto& grid = m_Grid.GetNodes();


    Step(); 

}

void AStar::Step()
{

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

    //GET NEIGHBORS
	auto* topNeighbor = nodes[m_CurrentNode->row - 1][m_CurrentNode->column];
    auto* bottomNeighbor = nodes[m_CurrentNode->row + 1][m_CurrentNode->column];
    auto* rightNeighbor = nodes[m_CurrentNode->row][m_CurrentNode->column + 1];
    auto* leftNeighbor = nodes[m_CurrentNode->row][m_CurrentNode->column - 1]; 

    auto tryNeighbor = [&](int row, int column)
    {

    	if (row < 0 or row >= m_Grid.GetAmountOfGridRows() or column < 0 or column >= m_Grid.GetAmountOfGridCols()) return;

    	Node* neighbor = nodes[row][column];

    	if (neighbor->nodeType == NodeType::Obstacle) return;
    	if (neighbor->closed) return; 

    	CalculateNodeCost(m_CurrentNode, neighbor);

        neighbor->open = true;
        m_OpenList.push_back(neighbor);
    };
     
    tryNeighbor(m_CurrentNode->row - 1, m_CurrentNode->column); // up
    tryNeighbor(m_CurrentNode->row + 1, m_CurrentNode->column); // down
    tryNeighbor(m_CurrentNode->row, m_CurrentNode->column + 1); // right
    tryNeighbor(m_CurrentNode->row, m_CurrentNode->column - 1); // left

    m_CurrentNode->closed = true;
    m_CurrentNode->open = false; 
    m_ClosedList.push_back(m_CurrentNode);



}

void AStar::SelectLowestCostNode()
{
    if (m_OpenList.empty())
    {
        m_CurrentNode = nullptr;
        return;
    }

    auto it = std::ranges::min_element(
        m_OpenList.begin(), m_OpenList.end(),
        [](const Node* a, const Node* b) 
        {
            if (a->fScore == b->fScore)      
                return a->gScore < b->gScore; //prefer lower gScore with tiebreaker
            return a->fScore < b->fScore;
        });

    m_CurrentNode = (it != m_OpenList.end()) ? *it : nullptr;
}

void AStar::CalculateNodeCost(Node* from, Node* node)
{
    auto heuristicManhatten = [](Node* a, Node* b)
    {
    	const int dx = std::abs(a->column - b->column);
    	const int dy = std::abs(a->row - b->row);
    	return (dx + dy) * 10;

    }; 
    constexpr int kStraight = 10;     

    const float tentativeG = (from ? from->gScore : 0.0f) + kStraight;
	if (!node->open or tentativeG < node->gScore)
    {
        node->parent = from;
        node->gScore = tentativeG;
        node->hScore = heuristicManhatten(node, m_DestinationNode); 
        node->fScore = node->gScore + node->hScore;
    }
}
