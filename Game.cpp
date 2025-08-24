#include "Game.h"
#include "Grid.h"
#include "JumpPointSearch.h"
Game::Game()
{
}

Game::~Game()
{
}

void Game::Initialize()
{
    // Code that needs to execute (once) at the start of the game, before the game window is created

    AbstractGame::Initialize();
    GAME_ENGINE->SetTitle(_T("ResearchProjectGPP"));

    GAME_ENGINE->SetWidth(1880);
    GAME_ENGINE->SetHeight(1000);
    GAME_ENGINE->SetFrameRate(165);

    m_pGrid = std::make_unique<Grid>();
    m_pJumpPointSearch = std::make_unique<JumpPointSearch>(*m_pGrid);
    m_pAStar = std::make_unique<AStar>(*m_pGrid);
    m_pThetaStar = std::make_unique<ThetaStar>(*m_pGrid);
    // Set the keys that the game needs to listen to
    //tstringstream buffer;
    //buffer << _T("KLMO");
    //buffer << (char) VK_LEFT;
    //buffer << (char) VK_RIGHT;
    GAME_ENGINE->SetKeyList(L" ");

}

void Game::Start()
{
    m_pGrid->Start();
    m_pAStar->Start();
    m_pJumpPointSearch->Start();
    m_pThetaStar->Start();

    m_pBtnSolveAStar = std::make_unique<Button>(_T("SolveAStar"));
    m_pBtnSolveAStar->SetBounds(10, 10, 110, 40);
    m_pBtnSolveAStar->AddActionListener(this);

    m_pBtnSolveJumpPointSearch = std::make_unique<Button>(_T("SolveJumpPointSearch"));
    m_pBtnSolveJumpPointSearch->SetBounds(10, 50, 110, 80);
    m_pBtnSolveJumpPointSearch->AddActionListener(this);

    m_pBtnSolveThetaStar = std::make_unique<Button>(_T("SolveThetaStar"));
    m_pBtnSolveThetaStar->SetBounds(10, 90, 110, 120);
    m_pBtnSolveThetaStar->AddActionListener(this);

    m_pBtnReset = std::make_unique<Button>(_T("Reset"));
    m_pBtnReset->SetBounds(10, 130, 110, 160);
    m_pBtnReset->AddActionListener(this);

}

void Game::End()
{
    // Insert code that needs to execute when the game ends
}

void Game::Paint(RECT rect) const
{
    GAME_ENGINE->FillWindowRect(RGB(18, 18, 18));
    m_pGrid->Paint();
    m_pThetaStar->Paint(); 
}

void Game::Tick()
{
    //m_pJumpPointSearch->Tick();
    if (m_StartAStar)
        m_pAStar->Tick();
    else if (m_StartJumpPointSearch)
        m_pJumpPointSearch->Tick();
    else if (m_StartThetaStar)
        m_pThetaStar->Tick();


}

void Game::MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam)
{
    m_pGrid->MouseButtonAction(isLeft, isDown, x, y, wParam);

}

void Game::MouseWheelAction(int x, int y, int distance, WPARAM wParam)
{
}

void Game::MouseMove(int x, int y, WPARAM wParam)
{
    m_pGrid->MouseMove(x, y, wParam);

}

void Game::CheckKeyboard()
{
    // Here you can check if a key is pressed down
    // Is executed once per frame 

    /* Example:
    if (GAME_ENGINE->IsKeyDown(_T('L'))) yIcon += xSpeed;
    if (GAME_ENGINE->IsKeyDown(_T('M'))) xIcon += xSpeed;
    if (GAME_ENGINE->IsKeyDown(_T('O'))) yIcon -= ySpeed;
    */

}

void Game::KeyPressed(TCHAR key)
{
}

void Game::CallAction(Caller* callerPtr)
{
    if (callerPtr == m_pBtnSolveAStar.get())
    {
        m_pAStar->Reset();
        m_pAStar->Start();
        m_StartAStar = true;
        m_StartJumpPointSearch = false;
        m_StartThetaStar = false;
    }
    else if (callerPtr == m_pBtnSolveJumpPointSearch.get())
    {
        m_pJumpPointSearch->Reset();
        m_pJumpPointSearch->Start();
        m_StartJumpPointSearch = true;
        m_StartAStar = false;
        m_StartThetaStar = false;
    }
    else if (callerPtr == m_pBtnSolveThetaStar.get())
    {
        m_pThetaStar->Reset();
        m_pThetaStar->Start();
        m_StartThetaStar = true; 
        m_StartAStar = false;
        m_StartJumpPointSearch = false;
    }
    else if (callerPtr == m_pBtnReset.get())
    {
        m_pAStar->Reset();
        m_pJumpPointSearch->Reset();
        m_pThetaStar->Reset();
        m_pGrid->Reset();
        m_StartAStar = m_StartJumpPointSearch = m_StartThetaStar = false;

    }

}


