#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <windows.h>

enum class CellType : int;

class JumpPointSearch
{
public:
    JumpPointSearch(const std::vector<CellType>& grid, int cols, int rows);

    void SetStart(POINT start);
    void SetGoal(POINT goal);

    // Solve the entire path at once
    std::vector<POINT> Solve();

    // Perform a single iteration of the algorithm
    // Returns true when finished (either path found or no path)
    bool Step();

    const std::vector<POINT>& GetPath() const { return mPath; }
    void Reset();

private:
    struct Node
    {
        int x{};
        int y{};
        double g{};
        double h{};
        Node* parent{ nullptr };
        double F() const { return g + h; }
    };

    struct NodeCompare
    {
        bool operator()(const Node* a, const Node* b) const { return a->F() > b->F(); }
    };

    Node* GetNode(int x, int y);
    Node* Jump(int x, int y, int dx, int dy);
    std::vector<std::pair<int, int>> GetNeighbors(Node* node);
    bool IsWalkable(int x, int y) const;
    double Heuristic(int x, int y) const;
    int Index(int x, int y) const { return y * mCols + x; }

    const std::vector<CellType>& mGrid;
    int mCols{};
    int mRows{};
    POINT mStart{ -1, -1 };
    POINT mGoal{ -1, -1 };

    std::priority_queue<Node*, std::vector<Node*>, NodeCompare> mOpen;
    std::unordered_map<int, Node*> mNodes;
    std::unordered_set<int> mClosed;
    std::vector<POINT> mPath;
    bool mFinished{ false };
};

