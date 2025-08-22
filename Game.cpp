//-----------------------------------------------------------------
// Main Game File
// C++ Source - Game.cpp - version v8_01
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Game.h"

//-----------------------------------------------------------------
// Game Member Functions																				
//-----------------------------------------------------------------

Game::Game() 																	
{
	// nothing to create
}

Game::~Game()																						
{
	// nothing to destroy
}

void Game::Initialize()			
{
	// Code that needs to execute (once) at the start of the game, before the game window is created

	AbstractGame::Initialize();
	GAME_ENGINE->SetTitle(_T("ResearchProjectGPP"));	
	
	GAME_ENGINE->SetWidth(1024);
	GAME_ENGINE->SetHeight(768);
    GAME_ENGINE->SetFrameRate(50);

	GAME_ENGINE->SetKeyList(_T("SDOE"));

}

void Game::Start()
{
	mGrid.assign(GRID_COLS * GRID_ROWS, CellType::Empty); 
}

void Game::End()
{
	// Insert code that needs to execute when the game ends
}

void Game::Paint(RECT rect) const
{
	GAME_ENGINE->FillWindowRect(RGB(255, 255, 255)); 
	for (int row = 0; row < GRID_ROWS; ++row)
	{
		for (int col = 0; col < GRID_COLS; ++col)
		{
			int left = col * CELL_SIZE;
			int top = row * CELL_SIZE;
			int right = left + CELL_SIZE;
			int bottom = top + CELL_SIZE;
			 
			switch (mGrid[row * GRID_COLS + col])
			{
			case CellType::Start:
				GAME_ENGINE->SetColor(RGB(0, 255, 0));
				GAME_ENGINE->FillRect(left, top, right, bottom);
				break;
			case CellType::Destination:
				GAME_ENGINE->SetColor(RGB(255, 0, 0));
				GAME_ENGINE->FillRect(left, top, right, bottom);
				break;
			case CellType::Obstacle:
				GAME_ENGINE->SetColor(RGB(0, 0, 0));
				GAME_ENGINE->FillRect(left, top, right, bottom);
				break;
			default:
				break;
			}

			GAME_ENGINE->SetColor(RGB(200, 200, 200));
			GAME_ENGINE->DrawRect(left, top, right, bottom);
		}
	}

	GAME_ENGINE->DrawString(m_BrushType, GAME_ENGINE->GetWidth() / 2, GAME_ENGINE->GetHeight() - 20); 

}

void Game::Tick()
{
}

void Game::MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam)
{	
	if (isLeft && isDown)
	{
		int col = x / CELL_SIZE;
		int row = y / CELL_SIZE;
		if (col >= 0 && col < GRID_COLS && row >= 0 && row < GRID_ROWS)
		{ 
			int index = row * GRID_COLS + col;
			switch (mCurrentBrush)
			{
			case CellType::Start:
				if (mStartCell.x != -1)
					mGrid[mStartCell.y * GRID_COLS + mStartCell.x] = CellType::Empty;
				mStartCell = { col, row };
				mGrid[index] = CellType::Start;
				break;
			case CellType::Destination:
				if (mDestCell.x != -1)
					mGrid[mDestCell.y * GRID_COLS + mDestCell.x] = CellType::Empty;
				mDestCell = { col, row };
				mGrid[index] = CellType::Destination;
				break;
			default:
				mGrid[index] = mCurrentBrush;
				break;
			}
		}
	}
}

void Game::MouseWheelAction(int x, int y, int distance, WPARAM wParam)
{	
	// Insert code for a mouse wheel action
}

void Game::MouseMove(int x, int y, WPARAM wParam)
{	
	// Insert code that needs to execute when the mouse pointer moves across the game window

	/* Example:
	if ( x > 261 && x < 261 + 117 ) // check if mouse position is within x coordinates of choice
	{
		if ( y > 182 && y < 182 + 33 ) // check if mouse position also is within y coordinates of choice
		{
			GAME_ENGINE->MessageBox("Mouse move.");
		}
	}
	*/
}

void Game::CheckKeyboard()
{	
	// Here you can check if a key is pressed down
	// Is executed once per frame 

	/* Example:
	if (GAME_ENGINE->IsKeyDown(_T('K'))) xIcon -= xSpeed;
	if (GAME_ENGINE->IsKeyDown(_T('L'))) yIcon += xSpeed;
	if (GAME_ENGINE->IsKeyDown(_T('M'))) xIcon += xSpeed;
	if (GAME_ENGINE->IsKeyDown(_T('O'))) yIcon -= ySpeed;
	*/
}

void Game::KeyPressed(TCHAR key)
{	
	switch (key)
	{
	case _T('S'):
		mCurrentBrush = CellType::Start;
		m_BrushType = L"BrushType: Start"; 
		break;
	case _T('D'):
		mCurrentBrush = CellType::Destination;
		m_BrushType = L"BrushType: Destination";

		break;
	case _T('O'):
		mCurrentBrush = CellType::Obstacle;
		m_BrushType = L"BrushType: Obstacle";

		break;
	case _T('E'):
		mCurrentBrush = CellType::Empty;
		m_BrushType = L"BrushType: Empty";
		break;
	default:
		break;
	}
}

void Game::CallAction(Caller* callerPtr)
{
	// Insert the code that needs to execute when a Caller (= Button, TextBox, Timer, Audio) executes an action
}




