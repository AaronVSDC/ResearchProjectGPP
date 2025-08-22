//-----------------------------------------------------------------
// Main Game File
// C++ Source - Game.cpp - version v8_01
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Game.h"
#include "JumpPointSearch.h"

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

        // create buttons for brush selection and actions
        mBtnStart = new Button(L"Start");
        mBtnStart->SetBounds(650, 50, 730, 80);
        mBtnStart->AddActionListener(this);
        mBtnStart->Show();

        mBtnDest = new Button(L"Destination");
        mBtnDest->SetBounds(650, 90, 730, 120);
        mBtnDest->AddActionListener(this);
        mBtnDest->Show();

        mBtnObstacle = new Button(L"Obstacle");
        mBtnObstacle->SetBounds(650, 130, 730, 160);
        mBtnObstacle->AddActionListener(this);
        mBtnObstacle->Show();

        mBtnEmpty = new Button(L"Empty");
        mBtnEmpty->SetBounds(650, 170, 730, 200);
        mBtnEmpty->AddActionListener(this);
        mBtnEmpty->Show();

        mBtnSolve = new Button(L"Solve");
        mBtnSolve->SetBounds(650, 210, 730, 240);
        mBtnSolve->AddActionListener(this);
        mBtnSolve->Show();

        mBtnStep = new Button(L"Step");
        mBtnStep->SetBounds(650, 250, 730, 280);
        mBtnStep->AddActionListener(this);
        mBtnStep->Show();
}

void Game::End()
{
        delete mBtnStart;
        delete mBtnDest;
        delete mBtnObstacle;
        delete mBtnEmpty;
        delete mBtnSolve;
        delete mBtnStep;
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
                        case CellType::Path:
                                GAME_ENGINE->SetColor(RGB(0, 0, 255));
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
        // Brush selection handled through buttons
}

void Game::CallAction(Caller* callerPtr)
{
        if (callerPtr == mBtnStart)
        {
                mCurrentBrush = CellType::Start;
                m_BrushType = L"BrushType: Start";
        }
        else if (callerPtr == mBtnDest)
        {
                mCurrentBrush = CellType::Destination;
                m_BrushType = L"BrushType: Destination";
        }
        else if (callerPtr == mBtnObstacle)
        {
                mCurrentBrush = CellType::Obstacle;
                m_BrushType = L"BrushType: Obstacle";
        }
        else if (callerPtr == mBtnEmpty)
        {
                mCurrentBrush = CellType::Empty;
                m_BrushType = L"BrushType: Empty";
        }
        else if (callerPtr == mBtnSolve)
        {
                ClearPath();
                if (mStartCell.x != -1 && mDestCell.x != -1)
                {
                        mJps = std::make_unique<JumpPointSearch>(mGrid, GRID_COLS, GRID_ROWS);
                        mJps->SetStart(mStartCell);
                        mJps->SetGoal(mDestCell);
                        auto path = mJps->Solve();
                        for (const auto& p : path)
                        {
                                int idx = p.y * GRID_COLS + p.x;
                                if (mGrid[idx] == CellType::Empty)
                                        mGrid[idx] = CellType::Path;
                        }
                        mJps.reset();
                }
        }
        else if (callerPtr == mBtnStep)
        {
                if (!mJps)
                {
                        ClearPath();
                        if (mStartCell.x == -1 || mDestCell.x == -1) return;
                        mJps = std::make_unique<JumpPointSearch>(mGrid, GRID_COLS, GRID_ROWS);
                        mJps->SetStart(mStartCell);
                        mJps->SetGoal(mDestCell);
                }

                if (mJps && mJps->Step())
                {
                        auto path = mJps->GetPath();
                        for (const auto& p : path)
                        {
                                int idx = p.y * GRID_COLS + p.x;
                                if (mGrid[idx] == CellType::Empty)
                                        mGrid[idx] = CellType::Path;
                        }
                        mJps.reset();
                }
        }
}

void Game::ClearPath()
{
        for (auto& cell : mGrid)
        {
                if (cell == CellType::Path)
                        cell = CellType::Empty;
        }
}




