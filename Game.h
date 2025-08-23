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
#include "Grid.h"

//-----------------------------------------------------------------
// Game Class
//-----------------------------------------------------------------
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
    Game(const Game& other) = delete;
    Game(Game&& other) noexcept = delete;
    Game& operator=(const Game& other) = delete;
    Game& operator=(Game&& other) noexcept = delete;

    //---------------------------
    // General Member Functions
    //---------------------------
    void Initialize() override;
    void Start() override;
    void End() override;
    void Paint(RECT rect) const override;
    void Tick() override;
    void MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam) override;
    void MouseWheelAction(int x, int y, int distance, WPARAM wParam) override;
    void MouseMove(int x, int y, WPARAM wParam) override;
    void CheckKeyboard() override;
    void KeyPressed(TCHAR key) override;

    void CallAction(Caller* callerPtr) override;

private:
    // -------------------------
    // Datamembers
    // ------------------------- 
    Grid m_Grid; 



    // Buttons for brush selection and solving
    std::unique_ptr<Button> mBtnStart;
    std::unique_ptr<Button> mBtnDest;
    std::unique_ptr<Button> mBtnObstacle;
    std::unique_ptr<Button> mBtnEmpty; 
    std::unique_ptr<Button> mBtnSolve;
    std::unique_ptr<Button> mBtnStep;
};
