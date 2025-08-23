#pragma once
#include "Grid.h"

class JumpPointSearch final
{
public:
	JumpPointSearch(Grid& grid)
	:m_Grid{grid}
	{}

	void Tick(); 

private: 

	Grid& m_Grid; 

};

