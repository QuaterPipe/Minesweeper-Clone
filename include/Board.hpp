#pragma once
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>
#include "SDL2/SDL.h"
class Game;
enum class TileState
{
    None,
    Tiled,
    MineHidden,
    MineExposed,
    MineExploded,
    Flagged,
    FlaggedIncorrectly,
    QuestionMarked,
    QuestionMarkedPressed,
    Empty,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight
};

struct Tile
{
    Tile(int x, int y, bool hasMine, TileState state);
    Tile();
    bool IsUnCovered() const;
    int x = 0;
    int y = 0;
    bool hasMine = false;
    bool isFlagged = false;
    bool selected = false;
    TileState state = TileState::Tiled;
    int adjacentMines = 0;
};

class Board
{
    public:
        Board(int width, int height, int mineCount, Game* parent);
        ~Board();
        bool AllNeighboursEmpty(int x, int y);
        bool AreaOpenEnabled() const;
        bool ColourEnabled() const;
        void Dig(int x, int y, int iter = 0);
        void FlagTile(int x, int y);
        std::vector<Tile*> GetAdjacentTiles(int x, int y) const;
        std::pair<int, int> GetPixelSize() const;
        int GetMineCount();
        Tile& GetTile(int x, int y) const;
        void GenerateMines(int clickX, int clickY);
        int GetHeight() const;
        int GetWidth() const;
        bool MarksEnabled() const;
        void Render() const;
        void RenderCounters() const;
        void RenderGuy() const;
        void Reset(int width, int height, int mineCount);
        void SelectTile(int x, int y);
        bool SufficientFlags(int x, int y);
        void ToggleAreaOpen();
        void ToggleColour();
        void ToggleQuestionMarks();
        void UnCoverTile(int x, int y);
        void Update(SDL_Event* e);
    private:
        int _width;
        int _height;
        int _mineCount;
        Tile* _tileMatrix = nullptr;
        Game* _parent;
        int _flagsLeft;
        std::chrono::steady_clock::time_point _start;
        std::chrono::steady_clock::time_point _now;
        int _timer;
        int _uncoveredTiles = 0;
        bool _areaOpenEnabled = true;
        bool _buttonPressed = false;
        bool _clickingTile = false;
        bool _colour = true;
        bool _lost = false;
        bool _mouseDown = false;
        bool _questionMarksEnabled = false;
        bool _started = false;
        bool _won = false;
        Tile* _selectedTile = nullptr;
        std::vector<Tile*> _selectedTileNeighbours;
};