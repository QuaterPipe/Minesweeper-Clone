#include "../include/Game.hpp"

bool Game::LoadMedia()
{
    if (!spriteSheet.LoadFromFile("assets/sprite_sheet.png"))
        return false;
    if (!BWspriteSheet.LoadFromFile("assets/sprite_sheet_bw.png"))
        return false;
    for (int i = 0; i < SPRITE_SHEET_0_SPRITE_COUNT; i++)
    {
        _spriteClips.push_back(SDL_Rect());
        switch (SpriteNames(i))
        {
            case SpriteNames::Timer_1:
            {
                _spriteClips[i].x = 0;
                _spriteClips[i].y = 0;
                _spriteClips[i].w = 13;
                _spriteClips[i].h = 23;
            }
            break;
            case SpriteNames::Timer_2:
            {
                _spriteClips[i].x = 14;
                _spriteClips[i].y = 0;
                _spriteClips[i].w = 13;
                _spriteClips[i].h = 23; 
            }
            break;
            case SpriteNames::Timer_3:
            {
                _spriteClips[i].x = 28;
                _spriteClips[i].y = 0;
                _spriteClips[i].w = 13;
                _spriteClips[i].h = 23; 
            }
            break;
            case SpriteNames::Timer_4:
            {
                _spriteClips[i].x = 42;
                _spriteClips[i].y = 0;
                _spriteClips[i].w = 13;
                _spriteClips[i].h = 23; 
            }
            break;
            case SpriteNames::Timer_5:
            {
                _spriteClips[i].x = 56;
                _spriteClips[i].y = 0;
                _spriteClips[i].w = 13;
                _spriteClips[i].h = 23; 
            }
            break;
            case SpriteNames::Timer_6:
            {
                _spriteClips[i].x = 70;
                _spriteClips[i].y = 0;
                _spriteClips[i].w = 13;
                _spriteClips[i].h = 23; 
            }
            break;
            case SpriteNames::Timer_7:
            {
                _spriteClips[i].x = 84;
                _spriteClips[i].y = 0;
                _spriteClips[i].w = 13;
                _spriteClips[i].h = 23; 
            }
            break;
            case SpriteNames::Timer_8:
            {
                _spriteClips[i].x = 98;
                _spriteClips[i].y = 0;
                _spriteClips[i].w = 13;
                _spriteClips[i].h = 23; 
            }
            break;
            case SpriteNames::Timer_9:
            {
                _spriteClips[i].x = 112;
                _spriteClips[i].y = 0;
                _spriteClips[i].w = 13;
                _spriteClips[i].h = 23; 
            }
            break;
            case SpriteNames::Timer_0:
            {
                _spriteClips[i].x = 126;
                _spriteClips[i].y = 0;
                _spriteClips[i].w = 13;
                _spriteClips[i].h = 23; 
            }
            break;
            case SpriteNames::Timer_Dash:
            {
                _spriteClips[i].x = 140;
                _spriteClips[i].y = 0;
                _spriteClips[i].w = 13;
                _spriteClips[i].h = 23; 
            }
            break;
            case SpriteNames::Timer_Empty:
            {
                _spriteClips[i].x = 154;
                _spriteClips[i].y = 0;
                _spriteClips[i].w = 13;
                _spriteClips[i].h = 23; 
            }
            break;
            case SpriteNames::Guy_Smiley:
            {
                _spriteClips[i].x = 0;
                _spriteClips[i].y = 24;
                _spriteClips[i].w = 24;
                _spriteClips[i].h = 24; 
            }
            break;
            case SpriteNames::Guy_Smiley_Pressed:
            {
                _spriteClips[i].x = 25;
                _spriteClips[i].y = 24;
                _spriteClips[i].w = 24;
                _spriteClips[i].h = 24; 
            }
            break;
            case SpriteNames::Guy_Surprised:
            {
                _spriteClips[i].x = 50;
                _spriteClips[i].y = 24;
                _spriteClips[i].w = 24;
                _spriteClips[i].h = 24; 
            }
            break;
            case SpriteNames::Guy_Sunglasses:
            {
                _spriteClips[i].x = 75;
                _spriteClips[i].y = 24;
                _spriteClips[i].w = 24;
                _spriteClips[i].h = 24; 
            }
            break;
            case SpriteNames::Guy_Dead:
            {
                _spriteClips[i].x = 100;
                _spriteClips[i].y = 24;
                _spriteClips[i].w = 24;
                _spriteClips[i].h = 24; 
            }
            break;
            case SpriteNames::Timer_Box:
            {
                _spriteClips[i].x = 125;
                _spriteClips[i].y = 24;
                _spriteClips[i].w = 41;
                _spriteClips[i].h = 25; 
            }
            break;
            case SpriteNames::Covered_Tile:
            {
                _spriteClips[i].x = 0;
                _spriteClips[i].y = 49;
                _spriteClips[i].w = 16;
                _spriteClips[i].h = 16; 
            }
            break;
            case SpriteNames::Empty_Tile:
            {
                _spriteClips[i].x = 17;
                _spriteClips[i].y = 49;
                _spriteClips[i].w = 16;
                _spriteClips[i].h = 16; 
            }
            break;
            case SpriteNames::Flagged_Tile:
            {
                _spriteClips[i].x = 34;
                _spriteClips[i].y = 49;
                _spriteClips[i].w = 16;
                _spriteClips[i].h = 16; 
            }
            break;
            case SpriteNames::Question_Marked_Tile:
            {
                _spriteClips[i].x = 51;
                _spriteClips[i].y = 49;
                _spriteClips[i].w = 16;
                _spriteClips[i].h = 16; 
            }
            break;
            case SpriteNames::Question_Marked_Tile_Pressed:
            {
                _spriteClips[i].x = 68;
                _spriteClips[i].y = 49;
                _spriteClips[i].w = 16;
                _spriteClips[i].h = 16; 
            }
            break;
            case SpriteNames::Mine:
            {
                _spriteClips[i].x = 85;
                _spriteClips[i].y = 49;
                _spriteClips[i].w = 16;
                _spriteClips[i].h = 16; 
            }
            break;
            case SpriteNames::Mine_Exploded:
            {
                _spriteClips[i].x = 102;
                _spriteClips[i].y = 49;
                _spriteClips[i].w = 16;
                _spriteClips[i].h = 16; 
            }
            break;
            case SpriteNames::Mine_Crossed_Out:
            {
                _spriteClips[i].x = 119;
                _spriteClips[i].y = 49;
                _spriteClips[i].w = 16;
                _spriteClips[i].h = 16; 
            }
            break;
            case SpriteNames::Tile_1:
            {
                _spriteClips[i].x = 0;
                _spriteClips[i].y = 66;
                _spriteClips[i].w = 16;
                _spriteClips[i].h = 16; 
            }
            break;
            case SpriteNames::Tile_2:
            {
                _spriteClips[i].x = 17;
                _spriteClips[i].y = 66;
                _spriteClips[i].w = 16;
                _spriteClips[i].h = 16; 
            }
            break;
            case SpriteNames::Tile_3:
            {
                _spriteClips[i].x = 34;
                _spriteClips[i].y = 66;
                _spriteClips[i].w = 16;
                _spriteClips[i].h = 16; 
            }
            break;
            case SpriteNames::Tile_4:
            {
                _spriteClips[i].x = 51;
                _spriteClips[i].y = 66;
                _spriteClips[i].w = 16;
                _spriteClips[i].h = 16; 
            }
            break;
            case SpriteNames::Tile_5:
            {
                _spriteClips[i].x = 68;
                _spriteClips[i].y = 66;
                _spriteClips[i].w = 16;
                _spriteClips[i].h = 16; 
            }
            break;
            case SpriteNames::Tile_6:
            {
                _spriteClips[i].x = 85;
                _spriteClips[i].y = 66;
                _spriteClips[i].w = 16;
                _spriteClips[i].h = 16; 
            }
            break;
            case SpriteNames::Tile_7:
            {
                _spriteClips[i].x = 102;
                _spriteClips[i].y = 66;
                _spriteClips[i].w = 16;
                _spriteClips[i].h = 16; 
            }
            break;
            case SpriteNames::Tile_8:
            {
                _spriteClips[i].x = 119;
                _spriteClips[i].y = 66;
                _spriteClips[i].w = 16;
                _spriteClips[i].h = 16; 
            }
            break;
            case SpriteNames::Outline_0:
            {
                _spriteClips[i].x = 136;
                _spriteClips[i].y = 58;
                _spriteClips[i].w = 11;
                _spriteClips[i].h = 11; 
            }
            break;
            case SpriteNames::Outline_1:
            {
                _spriteClips[i].x = 149;
                _spriteClips[i].y = 52;
                _spriteClips[i].w = 12;
                _spriteClips[i].h = 9; 
            }
            break;
            case SpriteNames::Outline_2:
            {
                _spriteClips[i].x = 149;
                _spriteClips[i].y = 62;
                _spriteClips[i].w = 8;
                _spriteClips[i].h = 8; 
            }
            break;
            case SpriteNames::Outline_3:
            {
                _spriteClips[i].x = 158;
                _spriteClips[i].y = 62;
                _spriteClips[i].w = 8;
                _spriteClips[i].h = 8; 
            }
            break;
            case SpriteNames::Outline_4:
            {
                _spriteClips[i].x = 167;
                _spriteClips[i].y = 62;
                _spriteClips[i].w = 8;
                _spriteClips[i].h = 8; 
            }
            break;
            case SpriteNames::Outline_5:
            {
                _spriteClips[i].x = 136;
                _spriteClips[i].y = 71;
                _spriteClips[i].w = 12;
                _spriteClips[i].h = 11; 
            }
            break;
            case SpriteNames::Outline_6:
            {
                _spriteClips[i].x = 149;
                _spriteClips[i].y = 71;
                _spriteClips[i].w = 10;
                _spriteClips[i].h = 11; 
            }
            break;
            case SpriteNames::Outline_7:
            {
                _spriteClips[i].x = 160;
                _spriteClips[i].y = 71;
                _spriteClips[i].w = 12;
                _spriteClips[i].h = 4; 
            }
            break;
            case SpriteNames::Border_0:
            {
                _spriteClips[i].x = 184;
                _spriteClips[i].y = 26;
                _spriteClips[i].w = 11;
                _spriteClips[i].h = 55; 
            }
            break;
            case SpriteNames::Border_1:
            {
                _spriteClips[i].x = 197;
                _spriteClips[i].y = 26;
                _spriteClips[i].w = 8;
                _spriteClips[i].h = 55; 
            }
        }
    }
    return true;
}