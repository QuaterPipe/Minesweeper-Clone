#pragma once
#include "Board.hpp"
#include "Stats.hpp"
#include "Texture.hpp"
#include <cmath>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <vector>
#include <Shlobj.h>
#include <commctrl.h>
#include "../include/SDL2/SDL_syswm.h"
#define SPRITE_SHEET_0_SPRITE_COUNT 44
#define ID_EXIT                     1
#define ID_NEW                      2
#define ID_BEGINNER                 3
#define ID_INTERMEDIATE             4
#define ID_EXPERT                   5
#define ID_SUPER                    6
#define ID_CUSTOM                   7
#define ID_QUESTION_MARKS           8
#define ID_AREA_OPEN                9
#define ID_STATS                    10
#define ID_SS                       11
#define ID_COLOUR                   12

#define WIN_FILE_MENU_HEIGHT        19

enum class SpriteNames;
class Game
{
    public:
        Game();
        ~Game();
        void ActivateMenu(HWND windowRef);
        bool Close();
        SDL_Rect& GetClip(SpriteNames index);
        int GetWinMenuHeight();
        bool Init();
        bool LoadMedia();
        void HandleWindowsEvent(SDL_Event* e);
        int Run();
        void Render();
        void ScreenShot();
        Texture spriteSheet;
        Texture BWspriteSheet;
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Surface* screenSurface = nullptr;
        HWND windowHandler;
        Board board;
        Stats stats;
        int maxWidth = 0;
        int maxHeight = 0;
        int gamemode = 0;
        
    private:
        HWND _getSDLWinHandle(SDL_Window* win);
        bool _isClosed = false;
        std::vector<SDL_Rect> _spriteClips;
        int _screenHeight = 55 + 7 + 16 * 9 - 1 + WIN_FILE_MENU_HEIGHT;
        int _screenWidth = 181;
};


enum class SpriteNames
{
    Timer_1,
    Timer_2,
    Timer_3,
    Timer_4,
    Timer_5,
    Timer_6,
    Timer_7,
    Timer_8,
    Timer_9,
    Timer_0,
    Timer_Dash,
    Timer_Empty,
    Guy_Smiley,
    Guy_Smiley_Pressed,
    Guy_Surprised,
    Guy_Sunglasses,
    Guy_Dead,
    Timer_Box,
    Covered_Tile,
    Empty_Tile,
    Flagged_Tile,
    Question_Marked_Tile,
    Question_Marked_Tile_Pressed,
    Mine,
    Mine_Exploded,
    Mine_Crossed_Out,
    Tile_1,
    Tile_2,
    Tile_3,
    Tile_4,
    Tile_5,
    Tile_6,
    Tile_7,
    Tile_8,
    Outline_0,
    Outline_1,
    Outline_2,
    Outline_3,
    Outline_4,
    Outline_5,
    Outline_6,
    Outline_7,
    Border_0,
    Border_1
};