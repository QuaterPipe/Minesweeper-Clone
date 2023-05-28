#include "../include/Stats.hpp"

Stats::Stats()
: _data()
{
    std::ifstream infile;
    infile.open("winmine.ini", std::ios_base::binary);
    std::string str;
    // if file exists load stats
    if (infile.good())
    {
        infile.read((char* )&_data, sizeof(_data));
        infile.close();
    }
    // else create new file
    else
    {
        std::ofstream fout;
        fout.open("winmine.ini", std::ios_base::binary);
        fout.write((char* ) &_data, sizeof(_data));
        fout.close();
    }
}

Stats::~Stats()
{
    std::ofstream fout;
    fout.open("winmine.ini", std::ios_base::binary);
    fout.write((char* ) &_data, sizeof(_data));
    fout.close();
}

void Stats::AddGameTime(unsigned long gameTime, int ind)
{
    _data.totalGameTime += gameTime;
}

bool Stats::AreaOpenEnabled()
{
    return _data.areaOpenEnabled;
}

bool Stats::ColourEnabled()
{
    return _data.colour;
}

const Data& Stats::GetData() const
{
    return _data;
}

void Stats::GameWon(unsigned long gameTime, int ind)
{
    _data.gamesWon[ind]++;
    _data.totalGameTime += gameTime;
    if (_data.bestGameTimes[ind] > gameTime)
        _data.bestGameTimes[ind] = gameTime;
    _data.totalGameTimes[ind] += gameTime;
    if (_data.avgGameTimes[ind])
        _data.avgGameTimes[ind] = _data.avgGameTimes[ind] + ((double)(gameTime - _data.avgGameTimes[ind]) / (double)_data.gamesPlayed[ind]);
    else
        _data.avgGameTimes[ind] = gameTime;
}

void Stats::GameLost(unsigned long gameTime, int ind)
{
    _data.gamesLost[ind]++;
    _data.totalGameTimes[ind] += gameTime;
    if (_data.avgGameTimes[ind])
        _data.avgGameTimes[ind] = _data.avgGameTimes[ind] + ((double)(gameTime - _data.avgGameTimes[ind]) / (double)_data.gamesPlayed[ind]);
    else
        _data.avgGameTimes[ind] = gameTime;
}

void Stats::GameStarted(int ind)
{
    _data.gamesPlayed[ind]++;
}

bool Stats::MarksEnabled()
{
    return _data.questionMarksEnabled;
}

void Stats::ToggleAreaOpen()
{
    _data.areaOpenEnabled = !_data.areaOpenEnabled;
}

void Stats::ToggleColour()
{
    _data.colour = !_data.colour;
}

void Stats::ToggleMarks()
{
    _data.questionMarksEnabled = !_data.questionMarksEnabled;
}

void Stats::UpdateFlagsPlaced(unsigned long count)
{
    _data.flagsPlaced += count;
}

void Stats::UpdateMouseClicks(unsigned long count, bool isLeft)
{
    _data.mouseClicks[isLeft] += count;
}

void Stats::UpdateTilesDug(unsigned long count)
{
    _data.tilesDug += count;
}

Data::Data()
{
}

unsigned long Data::TotalGamesWon() const
{
    return gamesWon[0] + gamesWon[1] + gamesWon[2] + gamesWon[3] + gamesWon[4];
}

unsigned long Data::TotalGamesLost() const
{
    return gamesLost[0] + gamesLost[1] + gamesLost[2] + gamesLost[3] + gamesLost[4];
}

unsigned long Data::TotalGamesPlayed() const
{
    return gamesPlayed[0] + gamesPlayed[1] + gamesPlayed[2] + gamesPlayed[3] + gamesPlayed[4];
}