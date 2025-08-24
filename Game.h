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
#include "AStar.h"
#include "ThetaStar.h"


class JumpPointSearch;
class Grid;

class Game : public AbstractGame, public Callable
{
public:

    Game();

    virtual ~Game() override;

    Game(const Game& other) = delete;
    Game(Game&& other) noexcept = delete;
    Game& operator=(const Game& other) = delete;
    Game& operator=(Game&& other) noexcept = delete;

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
    std::unique_ptr<Grid> m_pGrid;
    std::unique_ptr<AStar> m_pAStar;
    std::unique_ptr<ThetaStar> m_pThetaStar;

    bool m_StartAStar = false;
    bool m_StartThetaStar = false;

    std::unique_ptr<Button> m_pBtnSolveAStar;
    std::unique_ptr<Button> m_pBtnSolveThetaStar;
    std::unique_ptr<Button> m_pBtnReset;

};