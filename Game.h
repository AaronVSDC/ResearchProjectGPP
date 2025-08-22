//-----------------------------------------------------------------
// Main Game  File
// C++ Header - Game.h - version v8_01			
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------

#include "Resource.h"	
#include "GameEngine.h"
#include "AbstractGame.h"
#include <memory>

//-----------------------------------------------------------------
// Game Class																
//-----------------------------------------------------------------
class JumpPointSearch; // forward declaration

class Game : public AbstractGame, public Callable
{
public:				
	//---------------------------
	// Constructor(s) and Destructor
	//---------------------------
	Game();

	virtual ~Game() override;

	//---------------------------
	// Disabling copy/move constructors and assignment operators   
	//---------------------------
	Game(const Game& other)					= delete;
	Game(Game&& other) noexcept				= delete;
	Game& operator=(const Game& other)		= delete;
	Game& operator=(Game&& other) noexcept	= delete;

	//---------------------------
	// General Member Functions
	//---------------------------
	void Initialize			()															override;
	void Start				()															override;
	void End				()															override;
	void Paint				(RECT rect)	const											override;
	void Tick				()															override;
	void MouseButtonAction	(bool isLeft, bool isDown, int x, int y, WPARAM wParam)		override;
	void MouseWheelAction	(int x, int y, int distance, WPARAM wParam)					override;
	void MouseMove			(int x, int y, WPARAM wParam)								override;
	void CheckKeyboard		()															override;
	void KeyPressed			(TCHAR key)													override;
	
	void CallAction			(Caller* callerPtr)											override;

private:
	// -------------------------
	// Datamembers
	// -------------------------


        enum class CellType { Empty, Start, Destination, Obstacle, Path };

	static const int GRID_COLS{ 20 };
	static const int GRID_ROWS{ 15 };
	static const int CELL_SIZE{ 32 };

        std::vector<CellType> mGrid;
        CellType mCurrentBrush{ CellType::Obstacle };
        POINT mStartCell{ -1, -1 };
        POINT mDestCell{ -1, -1 };

        std::wstring m_BrushType = L"undefined";

        // UI Buttons
        Button* mBtnStart{ nullptr };
        Button* mBtnDest{ nullptr };
        Button* mBtnObstacle{ nullptr };
        Button* mBtnEmpty{ nullptr };
        Button* mBtnSolve{ nullptr };
        Button* mBtnStep{ nullptr };

        std::unique_ptr<JumpPointSearch> mJps;
        bool mStepping{ false };

        void ClearPath();
};
