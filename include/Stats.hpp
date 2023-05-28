#pragma once
#include <algorithm>
#include <fstream>
#include <limits>
#include <string>

struct Data
{
    Data();
    unsigned long mouseClicks[2] = {0, 0};
    unsigned long flagsPlaced = 0;
    unsigned long tilesDug = 0;
    unsigned long gamesWon[5] = {0, 0, 0, 0, 0};
    unsigned long gamesLost[5] = {0, 0, 0, 0, 0};
    unsigned long gamesPlayed[5] = {0, 0, 0, 0, 0};
    double avgGameTimes[5] = {0, 0, 0, 0, 0};
    unsigned long bestGameTimes[5] = {999, 999, 999, 999, 999};
    unsigned long totalGameTimes[5] = {0, 0, 0, 0, 0};
    unsigned long totalGameTime = 0;
    bool colour = true;
    bool areaOpenEnabled = true;
    bool questionMarksEnabled = false;
    unsigned long TotalGamesWon() const;
    unsigned long TotalGamesLost() const;
    unsigned long TotalGamesPlayed() const;
};

class Stats
{
private:
    Data _data;
public:
    Stats();
    ~Stats();
    void AddGameTime(unsigned long gameTime, int ind);
    bool AreaOpenEnabled();
    bool ColourEnabled();
    const Data& GetData() const;
    void GameWon(unsigned long gameTime, int ind);
    void GameLost(unsigned long gameTime, int ind);
    void GameStarted(int ind);
    bool MarksEnabled();
    void ToggleAreaOpen();
    void ToggleColour();
    void ToggleMarks();
    void UpdateFlagsPlaced(unsigned long count);
    void UpdateMouseClicks(unsigned long count, bool isLeft = true);
    void UpdateTilesDug(unsigned long count);
};