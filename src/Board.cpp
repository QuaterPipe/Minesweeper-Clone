#include "../include/Board.hpp"
#include "../include/Game.hpp"
int digOverflow = -1;
Board::Board(int width, int height, int mineCount, Game* parent)
: _width(width), _height(height), _mineCount(mineCount), _parent(parent), _flagsLeft(mineCount), _timer(0)
{
    srand((int) time(NULL));
    _tileMatrix = new Tile[width * height];
    for (int i = 0; i < height; i++)
    {
        for (int  j = 0; j < width; j++)
        {
            GetTile(j, i) = Tile();
            GetTile(j, i).x = j;
            GetTile(j, i).y = i;
        }
    }
}

Board::~Board()
{
    delete[] _tileMatrix;
}

bool Board::AllNeighboursEmpty(int x, int y)
{
    for (Tile* t: GetAdjacentTiles(x, y))
        if (t->state != TileState::Empty) return false;
    return true;
}

bool Board::AreaOpenEnabled() const
{
    return _areaOpenEnabled;
}

bool Board::ColourEnabled() const
{
    return _colour;
}

void Board::Dig(int x, int y, int iter)
{ 
    if (GetTile(x, y).isFlagged)
        return;
    UnCoverTile(x, y);
    if (_lost)
    {
        _parent->stats.GameLost(_timer, _parent->gamemode);
        return;
    }
    if (iter >= 500)
    {
        digOverflow = x + y * _width;
        return;
    }
    if (!iter)
        _parent->stats.UpdateTilesDug(1);
    if (GetTile(x, y).state == TileState::Empty)
    {
        if (AllNeighboursEmpty(x, y))
            return;
        for (Tile* t: GetAdjacentTiles(x, y))
        {
            if (t->state == TileState::Tiled && !t->adjacentMines)
                Dig(t->x, t->y, iter + 1);
            else
                UnCoverTile(t->x, t->y);
        }
        while (iter >= 4 && digOverflow != -1)
        {
            int newX = digOverflow % _width, newY = digOverflow / _width;
            digOverflow = -1;
            Dig(newX, newY, 1);
        }
    }
    else if (SufficientFlags(x, y) && _areaOpenEnabled)
    {
        for (Tile* t: GetAdjacentTiles(x, y))
        {
            if (!t->adjacentMines && t->state == TileState::Tiled)
                Dig(t->x, t->y, iter + 1);
            else if (t->state == TileState::Tiled || t->state == TileState::MineHidden || t->state == TileState::QuestionMarked)
                UnCoverTile(t->x, t->y);
        }
    }
}

void Board::FlagTile(int x, int y)
{
    if (_won)
        return;
    Tile& ref = GetTile(x, y);
    if (ref.IsUnCovered())
        return;
    if (ref.state == TileState::Empty)
        return;
    if (ref.isFlagged)
    {
        if (_questionMarksEnabled)
        {
            ref.state = TileState::QuestionMarked;
            ref.isFlagged = false;
        }
        else
        {
            ref.state = TileState::Tiled;
            ref.isFlagged = false;
        }
        _flagsLeft += 1;
    }
    else if(ref.state == TileState::QuestionMarked)
    {
        ref.state = TileState::Tiled;
        ref.isFlagged = false;
    }
    else if (_flagsLeft)
    {
        _parent->stats.UpdateFlagsPlaced(1);
        if (ref.hasMine)
            ref.state = TileState::Flagged;
        else
            ref.state = TileState::Flagged;
        ref.isFlagged = true;
        _flagsLeft--;
    }
}

int Board::GetMineCount()
{
    return _mineCount;
}


std::pair<int, int> Board::GetPixelSize() const
{
    std::pair<int, int> x;
    x.first = 20 + 16 * _width;
    x.second = 55 + 8 + 16 * _height;
    return x;
}

void Board::GenerateMines(int clickX, int clickY)
{
    int randIndex = clickX + clickY * _height;
    std::vector<Tile> tiles;
    std::vector<int> takenValues;
    takenValues.reserve(_mineCount);
    for (int i = 0; i < _height; i++)
    {
        for (int j = 0; j < _width; j++)
        {
            GetTile(j, i).x = j;
            GetTile(j, i).y = i;
        }
    }
    for (int i = 0; i < _mineCount; i++)
    {
        while (randIndex == clickX + clickY * _width)
        {
            randIndex = rand() % (_width * _height);
        }
        while (std::find(takenValues.begin(), takenValues.end(), randIndex) != takenValues.end() && !takenValues.empty())
        {
            randIndex = rand() % (_width * _height);
            while (randIndex == clickX + clickY * _width)
                randIndex = rand() % (_width * _height);
        }
        Tile t(randIndex % _width, randIndex / _width, true, TileState::MineHidden);
        GetTile(t.x, t.y) = t;
        takenValues.push_back(randIndex);
        randIndex = rand() % (_width * _height);
    }
    for (int i = 0; i < _height; i++)
    {
        for (int j = 0; j < _width; j++)
        {
            if (GetTile(j, i).state == TileState::MineHidden)
                continue;
            GetTile(j, i).state = TileState::Tiled;
            int mineCount = 0;
            auto adjacent = GetAdjacentTiles(j, i);
            for (Tile* t: adjacent)
            {
                if (t->hasMine)
                    mineCount++;
            }
            GetTile(j, i).adjacentMines = mineCount;
        }
    }
    Dig(clickX, clickY);
    _started = true;
}

std::vector<Tile*> Board::GetAdjacentTiles(int x, int y) const
{
    std::vector<Tile*> tiles;
    std::pair<int, int> coords[8] = {
        std::make_pair(x - 1, y - 1),
        std::make_pair(x, y - 1),
        std::make_pair(x + 1, y - 1),
        std::make_pair(x + 1, y),
        std::make_pair(x + 1, y + 1),
        std::make_pair(x, y + 1),
        std::make_pair(x - 1, y + 1),
        std::make_pair(x - 1, y)
    };
    for (const auto& pr: coords)
    {
        if (pr.first >= 0 && pr.first < _width)
        {
            if (pr.second >= 0 && pr.second < _height)
                tiles.push_back(&GetTile(pr.first, pr.second));
        }
    }
    return tiles;
}

Tile& Board::GetTile(int x, int y) const
{
    return _tileMatrix[x + y * (_width)];
}

int Board::GetHeight() const
{
    return _height;
}

int Board::GetWidth() const
{
    return _width;
}

bool Board::MarksEnabled() const
{
    return _questionMarksEnabled;
}

void Board::Render() const
{
    // 12, 55;
    using SS = SpriteNames;
    Texture& sheet = _colour ? _parent->spriteSheet : _parent->BWspriteSheet;
    int tileWidth = _parent->GetClip(SS::Covered_Tile).w;
    for (int i = 0; i < _height * _width; i++)
    {
        Tile& ref = _tileMatrix[i];
        if (ref.selected && ref.state == TileState::Tiled)
            sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Empty_Tile));
        else if (ref.selected && ref.state == TileState::MineHidden)
            sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Empty_Tile));
        else if (ref.selected && ref.state == TileState::QuestionMarked)
            sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Question_Marked_Tile_Pressed));
        else {
            switch (ref.state)
            {
                case TileState::Empty:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Empty_Tile));
                    break;
                case TileState::One:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Tile_1));
                    break;
                case TileState::Two:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Tile_2));
                    break;
                case TileState::Three:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Tile_3));
                    break;
                case TileState::Four:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Tile_4));
                    break;
                case TileState::Five:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Tile_5));
                    break;
                case TileState::Six:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Tile_6));
                    break;
                case TileState::Seven:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Tile_7));
                    break;
                case TileState::Eight:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Tile_8));
                    break;
                case TileState::Tiled:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Covered_Tile));
                    break;
                case TileState::MineHidden:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Covered_Tile));
                    break;
                case TileState::MineExploded:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Mine_Exploded));
                    break;
                case TileState::MineExposed:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Mine));
                    break;
                case TileState::Flagged:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Flagged_Tile));
                    break;
                case TileState::FlaggedIncorrectly:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Mine_Crossed_Out));
                    break;
                case TileState::QuestionMarked:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Question_Marked_Tile));
                    break;
                case TileState::QuestionMarkedPressed:
                    sheet.Render(12 + ref.x * tileWidth, 55 + ref.y * tileWidth, &_parent->GetClip(SS::Question_Marked_Tile_Pressed));
                    break;
                default:
                    break;
            }
        }
    }
    RenderCounters();
    RenderGuy();
}

void Board::RenderCounters() const
{
    using SS = SpriteNames;
    Texture& sheet = _colour ? _parent->spriteSheet : _parent->BWspriteSheet;
    Uint32 a, b, c;
    if (_flagsLeft <= 999)
    {
        a = (_flagsLeft / 100);
        b = (_flagsLeft - (a * 100)) / 10;
        c = (_flagsLeft - (a * 100) - (b * 10));
    }
    else
        a = b = c = 9;
    SS conv[10] = {
        SS::Timer_0, SS::Timer_1, SS::Timer_2, SS::Timer_3, SS::Timer_4,
        SS::Timer_5, SS::Timer_6, SS::Timer_7, SS::Timer_8, SS::Timer_9 
    };
    sheet.Render(17, 16, &_parent->GetClip(conv[a]));
    sheet.Render(17 + 12 + 1, 16, &_parent->GetClip(conv[b]));
    sheet.Render(17 + 24 + 2, 16, &_parent->GetClip(conv[c]));
    if (_timer <= 999)
    {
        a = (_timer / 100);
        b = (_timer - (a * 100)) / 10;
        c = _timer - (a * 100) - (b * 10);
    }
    else
        a = b = c = 9;
    int screenWidth = GetPixelSize().first;
    sheet.Render(screenWidth - 52, 16, &_parent->GetClip(conv[a]));
    sheet.Render(screenWidth - 52 + 12 + 1, 16, &_parent->GetClip(conv[b]));
    sheet.Render(screenWidth - 52 + 24 + 2, 16, &_parent->GetClip(conv[c]));
}

void Board::RenderGuy() const
{
    using SS = SpriteNames;
    Texture& sheet = _colour ? _parent->spriteSheet : _parent->BWspriteSheet;
    SDL_Rect* guy = nullptr;
    if (_won)
        guy = &_parent->GetClip(SS::Guy_Sunglasses);
    else if (_lost)
        guy = &_parent->GetClip(SS::Guy_Dead);
    else if (_clickingTile)
        guy = &_parent->GetClip(SS::Guy_Surprised);
    else if (_buttonPressed)
        guy = &_parent->GetClip(SS::Guy_Smiley_Pressed);
    else
        guy = &_parent->GetClip(SS::Guy_Smiley);

    int screenWidth = GetPixelSize().first;
    sheet.Render(screenWidth / 2 - 10, 15, guy);
}

void Board::Reset(int width, int height, int mineCount)
{
    if (_started && !_won && !_lost)
    {
        _parent->stats.AddGameTime(_timer, _parent->gamemode);
    }
    delete[] _tileMatrix;
    width = std::min(width, _parent->maxWidth);
    height = std::min(height, _parent->maxHeight);
    mineCount = std::min(mineCount, (int)(width * height * 0.8));
    _width = width;
    _height = height;
    _tileMatrix = new Tile[_width * _height];
    for (int i = 0; i < _height; i++)
    {
        for (int j = 0; j < _width; j++)
        {
            GetTile(j, i) = Tile();
            GetTile(j, i).x = j;
            GetTile(j, i).y = i;
        }
    }
    _mineCount = mineCount;
    _started = false;
    _selectedTile = nullptr;
    _lost = false;
    _won = false;
    _mouseDown = false;
    _buttonPressed = false;
    _start = std::chrono::steady_clock::now();
    _now = std::chrono::steady_clock::now();
    _timer = 0;
    _clickingTile = false;
    _flagsLeft = mineCount;
    _uncoveredTiles = 0;
}

void Board::SelectTile(int x, int y)
{
    if (_won || _lost)
        return;
    if (_selectedTile)
    {
        _selectedTile->selected = false;
        for (Tile* t: _selectedTileNeighbours)
            t->selected = false;
        _selectedTileNeighbours.clear();
    }
    _selectedTile = &GetTile(x, y);
    _selectedTile->selected = true;
    if (_selectedTile->state != TileState::Tiled && _selectedTile->adjacentMines && _selectedTile->state != TileState::QuestionMarked)
    {
        for (Tile* t: GetAdjacentTiles(x, y))
        {
            if (t->state == TileState::Tiled || t->state == TileState::MineHidden || t->state == TileState::QuestionMarked)
            {
                t->selected = true;
                _selectedTileNeighbours.push_back(t);
            }
        }
    }
}

bool Board::SufficientFlags(int x, int y)
{
    if ((int)TileState::One > (int)GetTile(x, y).state && (int)GetTile(x, y).state > (int)TileState::Seven)
        return false;
    int flagCount = 0;
    for (Tile* t: GetAdjacentTiles(x, y))
        if (t->isFlagged) flagCount++;
    return flagCount == GetTile(x, y).adjacentMines;
}

void Board::ToggleAreaOpen()
{
    _areaOpenEnabled = !_areaOpenEnabled;
}

void Board::ToggleColour()
{
    _colour = !_colour;
}

void Board::ToggleQuestionMarks()
{
    _questionMarksEnabled = !_questionMarksEnabled;
}

void Board::UnCoverTile(int x, int y)
{
    if (GetTile(x, y).IsUnCovered())
        return;
    if (GetTile(x, y).hasMine)
    {
        GetTile(x, y).state = TileState::MineExploded;
        for (int i = 0; i < _height; i++)
        {
            for (int j = 0; j < _width; j++)
            {
                if (j == x && i == y)
                    continue;
                if (GetTile(j, i).hasMine && !GetTile(j, i).isFlagged)
                    GetTile(j, i).state = TileState::MineExposed;
                if (GetTile(j, i).isFlagged && !GetTile(j, i).hasMine)
                    GetTile(j, i).state = TileState::FlaggedIncorrectly;

            }
        }
        _lost = true;
        return;
    }
    switch (GetTile(x, y).adjacentMines)
    {
        case 0:
            GetTile(x, y).state = TileState::Empty;
            break;
        case 1:
            GetTile(x, y).state = TileState::One;
            break;
        case 2:
            GetTile(x, y).state = TileState::Two;
            break;
        case 3:
            GetTile(x, y).state = TileState::Three;
            break;
        case 4:
            GetTile(x, y).state = TileState::Four;
            break;
        case 5:
            GetTile(x, y).state = TileState::Five;
            break;
        case 6:
            GetTile(x, y).state = TileState::Six;
            break;
        case 7:
            GetTile(x, y).state = TileState::Seven;
            break;
        case 8:
            GetTile(x, y).state = TileState::Eight;
            break;
    }
    _uncoveredTiles += 1;
    if (_uncoveredTiles == ((_width * _height) - _mineCount))
        _won = true;
    if (_won)
    {
        for (int i = 0; i < _height; i++)
        {
            for (int j = 0; j < _width; j++)
            {
                if (GetTile(j, i).hasMine && GetTile(j, i).state != TileState::Flagged)
                {
                    GetTile(j, i).state = TileState::Flagged;
                    GetTile(j, i).isFlagged = true;
                }
            }
        }
        _parent->stats.GameWon(_timer, _parent->gamemode);
    }
}

void Board::Update(SDL_Event* e)
{
    if (_started && !_won && !_lost)
    {
        _now = std::chrono::steady_clock::now();
        _timer = std::chrono::duration_cast<std::chrono::seconds>(_now - _start).count();
    }
    if (!e)
        return;
    if (_selectedTile)
    {
        _selectedTile->selected = false;
        for (Tile* t: _selectedTileNeighbours)
            t->selected = false;
        _selectedTileNeighbours.clear();
    }
    _clickingTile = _buttonPressed = false;
    if (e->type == SDL_QUIT)
    {
        if (_started && !_won && !_lost)
            _parent->stats.AddGameTime(_timer, _parent->gamemode);
    }
    if (e->type == SDL_MOUSEMOTION)
    {
        if (!_mouseDown)
            return;
        int x, y;
        SDL_GetMouseState(&x, &y);
        x -= 12;
        y -= 55;
        if (e->button.button == SDL_BUTTON_LEFT)
        {
            if (x >= 0 && x < (16 * _width) && y >= 0 && y <= (16 * _height) && !_lost)
            {
                SelectTile(x / 16, y / 16);
                _clickingTile = true;
            }
            x += 12;
            y += 55;
            int pixWidth = GetPixelSize().first;
            if (x >= (pixWidth / 2 - 11) && x <= (pixWidth / 2) + 25)
            {
                if (y >= 15 && y <= 40)
                    _buttonPressed = true;
            }
        }
    }
    else if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        if (_mouseDown)
            return;
        _mouseDown = true;
        int x, y;
        SDL_GetMouseState(&x, &y);
        x -= 12;
        y -= 55;
        if (e->button.button == SDL_BUTTON_LEFT)
        {
            if (x >= 0 && x < (16 * _width) && y >= 0 && y <= (16 * _height) && !_lost)
            {
                SelectTile(x / 16, y / 16);
                _clickingTile = true;
            }
            x += 12;
            y += 55;
            int pixWidth = GetPixelSize().first;
            if (x >= (pixWidth / 2 - 11) && x <= (pixWidth / 2) + 25)
            {
                if (y >= 15 && y <= 40)
                    _buttonPressed = true;
            }
            _parent->stats.UpdateMouseClicks(1, true);
        }
        else if (e->button.button == SDL_BUTTON_RIGHT)
        {
            if (x >= 0 && x < (16 * _width) && y >= 0 && y <= (16 * _height) && !_lost)
                FlagTile(x / 16, y / 16);
            _parent->stats.UpdateMouseClicks(1, false);
        }
    }
    else if (e->type == SDL_MOUSEBUTTONUP)
    {
        if (!_mouseDown)
            return;
        _mouseDown = false;
        _buttonPressed = false;
        int x, y;
        SDL_GetMouseState(&x, &y);
        x -= 12;
        y -= 55;
        if (e->button.button == SDL_BUTTON_LEFT)
        {
            if (x >= 0 && x < (16 * _width) && y >= 0 && y <= (16 * _height) && !_lost)
            {
                if (!_started)
                {
                    GenerateMines(x / 16, y / 16);
                    _started = true;
                    _start = std::chrono::steady_clock::now();
                    _now = std::chrono::steady_clock::now();
                    _parent->stats.GameStarted(_parent->gamemode);
                }
                else if (_selectedTile)
                {
                    Dig(_selectedTile->x, _selectedTile->y);
                }
            }
            x += 12;
            y += 55;
            int pixWidth = GetPixelSize().first;
            if (x >= (pixWidth / 2 - 11) && x <= (pixWidth / 2) + 25)
            {
                if (y >= 15 && y <= 40)
                {
                    _buttonPressed = true;
                    Reset(_width, _height, _mineCount);
                }
            }
        }
    }
}

Tile::Tile(int x, int y, bool hasMine, TileState state)
: x(x), y(y), hasMine(hasMine), state(state)
{
}

Tile::Tile()
: state(TileState::Tiled)
{
}

bool Tile::IsUnCovered() const
{
    TileState states[] = {
        TileState::Empty, TileState::One, TileState::Two,
        TileState::Three, TileState::Four, TileState::Five,
        TileState::Six, TileState::Seven, TileState::Eight,
        TileState::MineExposed
    };
    for (const TileState& s: states)
    {
        if (state == s)
            return true;
    }
    return false;
}