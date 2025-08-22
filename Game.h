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

#include <vector>
#include <string>

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

    enum class CellType { Empty, Start, Destination, Obstacle };

    static const int GRID_COLS{ 20 };
    static const int GRID_ROWS{ 15 };
    static const int CELL_SIZE{ 32 };

    std::vector<CellType> mGrid;
    CellType mCurrentBrush{ CellType::Obstacle };
    POINT mStartCell{ -1, -1 };
    POINT mDestCell{ -1, -1 };

    std::wstring m_BrushType{ L"undefined" };

    // Buttons for brush selection and solving
    std::unique_ptr<Button> mBtnStart;
    std::unique_ptr<Button> mBtnDest;
    std::unique_ptr<Button> mBtnObstacle;
    std::unique_ptr<Button> mBtnEmpty; 
    std::unique_ptr<Button> mBtnSolve;
    std::unique_ptr<Button> mBtnStep;

    // Path handling 
    std::vector<POINT> mPath;
    size_t mPathStep{ 0 };

    // JPS helpers
    struct Node
    {
        int x;
        int y;
        float g;
        float f;
        Node* parent;
    };

    bool IsWalkable(int x, int y) const;
    POINT Jump(int x, int y, int dx, int dy) const;
    std::vector<std::pair<int, int>> GetNeighbors(const Node* node) const;
    float Heuristic(int x1, int y1, int x2, int y2) const;
    std::vector<POINT> JumpPointSearch();
    void SolvePath();
    void StepPath();
};
