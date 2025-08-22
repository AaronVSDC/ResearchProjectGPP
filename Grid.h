#pragma once
#include "AbstractGame.h"

class Grid
{
public:
	void InitializeGrid(); 

	void Tick();
	void Paint();
	void KeyPressed(TCHAR key); 

private: 


	void PaintGrid();

};

