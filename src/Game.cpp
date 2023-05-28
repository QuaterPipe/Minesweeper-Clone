#include "../include/Game.hpp"
#include "../include/Board.hpp"
#include "resource.h"
HMENU hGame;
HMENU hOptions;
HMENU hMenuBar;
HWND hDialog = nullptr;
HINSTANCE hInst;
Game* game;
char szItemName[15];

bool fileExists(const char* fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

typedef struct
{
    TCHAR name[MAX_PATH];
    TCHAR desc[MAX_PATH];
} info;

info list[] = {
    {TEXT("Games Won"), TEXT("Beginner: %d\nIntermediate: %d\nExpert: %d\nSuper: %d\nCustom: %d\nTotal: %d")},
    {TEXT("Games Lost"), TEXT("Beginner: %d\nIntermediate: %d\nExpert: %d\nSuper: %d\nCustom: %d\nTotal: %d")},
    {TEXT("Games Played"), TEXT("Beginner: %d\nIntermediate: %d\nExpert: %d\nSuper: %d\nCustom: %d\nTotal: %d")},
    {TEXT("Best Game Times"), TEXT("Beginner: %d\nIntermediate: %d\nExpert: %d\nSuper: %d\nCustom: %d")},
    {TEXT("Average Game Times"), TEXT("Beginner: %0.2f\nIntermediate: %0.2f\nExpert: %0.2f\nSuper: %0.2f\nCustom: %0.2f")},
    {TEXT("Total Game Time"), TEXT("Total Time playing: \n%dh \n%0.2fm")},
    {TEXT("Mouse Clicks"), TEXT("Right Clicks: %d\nLeft Clicks: %d\nTotal Clicks: %d")},
    {TEXT("Flags Placed"), TEXT("%d flag(s) placed")},
    {TEXT("Tiles Dug"), TEXT("%d tile(s) dug")}
};
const Data* d;

void GenText(TCHAR buff[], int i, int size)
{
    switch (i)
    {
        case 0:
            StringCbPrintf(buff, size, list[i].desc, 
            d->gamesWon[0], d->gamesWon[1], d->gamesWon[2], d->gamesWon[3], d->gamesWon[4], d->TotalGamesWon());
            break;
        case 1:
            StringCbPrintf(buff, size, list[i].desc, 
            d->gamesLost[0], d->gamesLost[1], d->gamesLost[2], d->gamesLost[3], d->gamesLost[4], d->TotalGamesLost());
            break;
        case 2:
            StringCbPrintf(buff, size, list[i].desc, 
            d->gamesPlayed[0], d->gamesPlayed[1], d->gamesPlayed[2], d->gamesPlayed[3], d->gamesPlayed[4], d->TotalGamesPlayed());
            break;
        case 3:
            StringCbPrintf(buff, size, list[i].desc, 
            d->bestGameTimes[0], d->bestGameTimes[1], d->bestGameTimes[2], d->bestGameTimes[3], d->bestGameTimes[4]);
            break;
        case 4:
            StringCbPrintf(buff, size, list[i].desc, 
            d->avgGameTimes[0], d->avgGameTimes[1], d->avgGameTimes[2], d->avgGameTimes[3], d->avgGameTimes[4]);
            break;
        case 5:
            StringCbPrintf(buff, size, list[i].desc, (d->totalGameTime) / 3600, fmod(((double)d->totalGameTime) / 60.0, 60));
            break;
        case 6:
            StringCbPrintf(buff, size, list[i].desc, d->mouseClicks[0], d->mouseClicks[1], d->mouseClicks[0] + d->mouseClicks[1]);
            break;
        case 7:
            StringCbPrintf(buff, size, list[i].desc, d->flagsPlaced);
            break;
        case 8:
            StringCbPrintf(buff, size, list[i].desc, d->tilesDug);
            break;
        default:
            return;
    }
}
INT_PTR CALLBACK CreateListBox(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_INITDIALOG:
        {
            HWND hwndList = GetDlgItem(hwndDlg, IDC_STATS_LIST);
            for (int i = 0; i < 9; i++)
            {
                int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0, 
                    (LPARAM) list[i].name);
                SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM) i);
            }
            SetFocus(hwndList);
            return TRUE;
        }
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                case IDCANCEL:
                    EndDialog(hwndDlg, LOWORD(wParam));
                    return TRUE;

                case IDC_STATS_LIST:
                    {
                        switch (HIWORD(wParam)) 
                        { 
                        case LBN_SELCHANGE:
                            {
                                HWND hwndList = GetDlgItem(hwndDlg, IDC_STATS_LIST); 

                                // Get selected index.
                                int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0); 

                                // Get item data.
                                int i = (int)SendMessage(hwndList, LB_GETITEMDATA, lbItem, 0);

                                // Do something with the data from Roster[i]
                                TCHAR buff[MAX_PATH];
                                GenText(buff, i, MAX_PATH);
                                SetDlgItemText(hwndDlg, IDC_STATS_TEXT, buff); 
                                return TRUE; 
                            } 
                        }
                    }
                return TRUE;
            }
    };
    return FALSE;
}

BOOL CALLBACK CreateCustomDimensionsMenu(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
    switch (message)
    {
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                    if (!GetDlgItemText(hwndDlg, IDC_WIDTH, szItemName, 5))
                        *szItemName = '\0';
                    if (!GetDlgItemText(hwndDlg, IDC_HEIGHT, szItemName + 5, 5))
                        *(szItemName + 5) = '\0';
                    if (!GetDlgItemText(hwndDlg, IDC_MINES, szItemName + 10, 5))
                        *(szItemName + 10) = '\0';
                case IDCANCEL:
                    EndDialog(hwndDlg, wParam);
                    return TRUE;
            }
    }
    return FALSE;
}

using SS = SpriteNames;
Game::Game()
: spriteSheet(this), BWspriteSheet(this), board(9, 9, 10, this), _spriteClips()
{
    game = this;
    _spriteClips.resize(SPRITE_SHEET_0_SPRITE_COUNT);
    _screenWidth = board.GetPixelSize().first;
    _screenHeight = board.GetPixelSize().second;
    if (stats.AreaOpenEnabled() != board.AreaOpenEnabled())
        board.ToggleAreaOpen();
    if (stats.ColourEnabled() != board.ColourEnabled())
        board.ToggleColour();
    if (stats.MarksEnabled() != board.MarksEnabled())
        board.ToggleQuestionMarks();
    d = &stats.GetData();
}

Game::~Game()
{
    if (stats.AreaOpenEnabled() != board.AreaOpenEnabled())
        stats.ToggleAreaOpen();
    if (stats.ColourEnabled() != board.ColourEnabled())
        stats.ToggleColour();
    if (stats.MarksEnabled() != board.MarksEnabled())
        stats.ToggleMarks();
    if (!_isClosed)
    {
        Close();
        _isClosed = true;
    }
}

HWND Game::_getSDLWinHandle(SDL_Window* win)
{
    SDL_SysWMinfo infoWindow;
    SDL_VERSION(&infoWindow.version);
    if (!SDL_GetWindowWMInfo(win, &infoWindow))
    {
        return NULL;
    }
    return (infoWindow.info.win.window);
}

void Game::ActivateMenu(HWND windowRef)
{
    MENUINFO gameInfo = {0};
    gameInfo.cbSize = sizeof(gameInfo);
    hMenuBar = CreateMenu();
    gameInfo.dwStyle |= MNS_NOCHECK;
    gameInfo.fMask |= MIM_STYLE | MIM_APPLYTOSUBMENUS;
    hGame = CreateMenu();
    SetMenuInfo(hGame, &gameInfo);
    hOptions = CreateMenu();
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hGame, "Game");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hOptions, "Options");
    AppendMenu(hGame, MF_STRING, ID_NEW, "New (F2)");
    AppendMenu(hGame, MF_SEPARATOR, 0, nullptr);
    AppendMenu(hGame, MF_STRING, ID_BEGINNER, "Beginner");
    AppendMenu(hGame, MF_STRING, ID_INTERMEDIATE, "Intermediate");
    AppendMenu(hGame, MF_STRING, ID_EXPERT, "Expert");
    AppendMenu(hGame, MF_STRING, ID_SUPER, "Super");
    AppendMenu(hGame, MF_STRING, ID_CUSTOM, "Custom");
    AppendMenu(hGame, MF_SEPARATOR, 0, nullptr);
    AppendMenu(hGame, MF_STRING, ID_STATS, "Statistics (F5)");
    AppendMenu(hGame, MF_SEPARATOR, 0, nullptr);
    AppendMenu(hGame, MF_STRING, ID_EXIT, "Exit (Alt + F4)");
    AppendMenu(hOptions, MF_STRING, ID_QUESTION_MARKS, "Marks");
    if (board.MarksEnabled())
        CheckMenuItem(hOptions, ID_QUESTION_MARKS, MF_CHECKED);
    else
        CheckMenuItem(hOptions, ID_QUESTION_MARKS, MF_UNCHECKED);
    AppendMenu(hOptions, MF_STRING, ID_AREA_OPEN, "Area Open");
    if (board.AreaOpenEnabled())
        CheckMenuItem(hOptions, ID_AREA_OPEN, MF_CHECKED);
    else
        CheckMenuItem(hOptions, ID_AREA_OPEN, MF_UNCHECKED);
    AppendMenu(hOptions, MF_STRING, ID_COLOUR, "Colour");
    if (board.ColourEnabled())
        CheckMenuItem(hOptions, ID_COLOUR, MF_CHECKED);
    else
        CheckMenuItem(hOptions, ID_COLOUR, MF_UNCHECKED);
    AppendMenu(hOptions, MF_SEPARATOR, 0, nullptr);
    AppendMenu(hOptions, MF_STRING, ID_SS, "ScreenShot (F10)");
    SetMenu(windowRef, hMenuBar);
}


bool Game::Close()
{
    spriteSheet.Free();
    BWspriteSheet.Free();
    DestroyMenu(hGame);
    DestroyMenu(hOptions);
    DestroyMenu(hMenuBar);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = nullptr;
    renderer = nullptr;
    IMG_Quit();
    SDL_Quit();
    return true;
}

bool Game::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr<<"SDL failed to initialize: SDL Error: "<<SDL_GetError()<<"\n";
        return false;
    }
    _screenWidth = board.GetPixelSize().first;
    _screenHeight = board.GetPixelSize().second;
    window = SDL_CreateWindow( "Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _screenWidth, _screenHeight, SDL_WINDOW_SHOWN);
    if( window == nullptr)
    {
        std::cerr<<"Window could not be created! SDL_Error: "<<SDL_GetError()<<"\n";
        return false;
    }
    windowHandler = _getSDLWinHandle(window);
    hInst = GetModuleHandle(nullptr);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr<<"Renderer could not be created! SDL_Error: "<<SDL_GetError()<<"\n";
        return false;
    }
    SDL_SetRenderDrawColor(renderer, 0xC0, 0xC0, 0xC0, 0xFF);
    SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cerr<<"SDL_image failed to initialized: SDL_image Error: "<<IMG_GetError()<<"\n";
        return false;
    }
    screenSurface = SDL_GetWindowSurface(window);
    ActivateMenu(windowHandler);
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icc);
    SDL_SetWindowSize(window, _screenWidth, _screenHeight);
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    maxWidth = (DM.w - 20) / 16;
    maxHeight = (DM.h - 63) / 16;
    return true;
}

SDL_Rect& Game::GetClip(SpriteNames index)
{
    return _spriteClips[(int)index];
}

int Game::GetWinMenuHeight()
{
    MENUINFO lpcmi;
    lpcmi.cbSize = sizeof(MENUINFO);
    GetMenuInfo(hMenuBar, &lpcmi);
    return lpcmi.cyMax;
}

void Game::HandleWindowsEvent(SDL_Event* e)
{
    if (e->syswm.msg->msg.win.msg == WM_COMMAND)
    {
        UINT state = 0;
        switch (LOWORD(e->syswm.msg->msg.win.wParam))
        {
            case ID_QUESTION_MARKS:
                {
                    state = GetMenuState(hOptions, ID_QUESTION_MARKS, MF_BYCOMMAND);
                    if (state == MF_CHECKED)
                        CheckMenuItem(hOptions, ID_QUESTION_MARKS, MF_UNCHECKED);
                    else
                        CheckMenuItem(hOptions, ID_QUESTION_MARKS, MF_CHECKED);
                    board.ToggleQuestionMarks();
                }
                break;

            case ID_AREA_OPEN:
                {
                    state = GetMenuState(hOptions, ID_AREA_OPEN, MF_BYCOMMAND);
                    if (state == MF_CHECKED)
                        CheckMenuItem(hOptions, ID_AREA_OPEN, MF_UNCHECKED);
                    else
                        CheckMenuItem(hOptions, ID_AREA_OPEN, MF_CHECKED);
                    board.ToggleAreaOpen();
                }
                break;

            case ID_SS:
                {
                    ScreenShot();
                }
                break;
            
            case ID_COLOUR:
                {
                    state = GetMenuState(hOptions, ID_COLOUR, MF_BYCOMMAND);
                    if (state == MF_CHECKED)
                        CheckMenuItem(hOptions, ID_COLOUR, MF_UNCHECKED);
                    else
                        CheckMenuItem(hOptions, ID_COLOUR, MF_CHECKED);
                    board.ToggleColour();
                }
                break;

            case ID_NEW:
                {
                    board.Reset(board.GetWidth(), board.GetHeight(), board.GetMineCount());
                }
                break;

            case ID_BEGINNER:
                {
                    board.Reset(9, 9, 10);
                    std::pair<int, int> dimensions = board.GetPixelSize();
                    SDL_SetWindowSize(window, dimensions.first, dimensions.second);
                    _screenWidth = dimensions.first;
                    _screenHeight = dimensions.second;
                    gamemode = 0;
                }
                break;

            case ID_INTERMEDIATE:
                {
                    board.Reset(16, 16, 40);
                    std::pair<int, int> dimensions = board.GetPixelSize();
                    SDL_SetWindowSize(window, dimensions.first, dimensions.second);
                    _screenWidth = dimensions.first;
                    _screenHeight = dimensions.second;
                    gamemode = 1;
                }
                break;
        
            case ID_EXPERT:
                {
                    board.Reset(30, 16, 99);
                    std::pair<int, int> dimensions = board.GetPixelSize();
                    SDL_SetWindowSize(window, dimensions.first, dimensions.second);
                    _screenWidth = dimensions.first;
                    _screenHeight = dimensions.second;
                    gamemode = 2;
                }
                break;

            case ID_SUPER:
                {
                    board.Reset(80, 35, 599);
                    std::pair<int, int> dimensions = board.GetPixelSize();
                    SDL_SetWindowSize(window, dimensions.first, dimensions.second);
                    _screenWidth = dimensions.first;
                    _screenHeight = dimensions.second;
                    gamemode = 3;
                }
                break;

            case ID_CUSTOM:
                {
                    if (DialogBox(hInst,
                                MAKEINTRESOURCE(IDD_DIALOG1),
                                windowHandler,
                                (DLGPROC)CreateCustomDimensionsMenu)
                        ==IDOK)
                    {
                        if (szItemName[0] == '\0' || szItemName[5] == '\0' || szItemName[10] == '\0')
                            break;
                        try
                        {
                            int width, height, mines;
                            width = std::atoi(szItemName);
                            height = std::atoi(szItemName + 5);
                            mines = std::atoi(szItemName + 10);
                            board.Reset(width, height, mines);
                            std::pair<int, int> dimensions = board.GetPixelSize();
                            SDL_SetWindowSize(window, dimensions.first, dimensions.second);
                            _screenWidth = dimensions.first;
                            _screenHeight = dimensions.second;
                        }
                        catch (const std::exception& e)
                        {
                            break;
                        }
                    }
                    gamemode = 4;
                }
                break;
            case ID_STATS:
                {
                    DialogBox(hInst,
                        MAKEINTRESOURCE(IDD_DIALOG2),
                        windowHandler,
                        (DLGPROC)CreateListBox);
                }
                break;
            default:
                break;
        }
    }
}

int Game::Run()
{
    if (!Init())
        return -1;
    if (!LoadMedia())
        return -1; 
    SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xC0, 0xC0,0xC0));
    SDL_UpdateWindowSurface(window);
    SDL_Event e;
    bool quit = false;
    while(!quit)
    {
        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_WINDOWEVENT_CLOSE)
            {
                e.type = SDL_QUIT;
                SDL_PushEvent(&e);
            }
            else if (e.type == SDL_SYSWMEVENT)
            {
                if (e.syswm.msg->msg.win.msg == WM_COMMAND)
                {
                    if (LOWORD(e.syswm.msg->msg.win.wParam) == ID_EXIT)
                        quit = true;
                }
                HandleWindowsEvent(&e);
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F2)
                board.Reset(board.GetWidth(), board.GetHeight(), board.GetMineCount());
            if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_F5)
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), windowHandler, (DLGPROC)CreateListBox);
            if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_F10)
                ScreenShot();
            if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
                board.Update(&e);
            if (e.type == SDL_QUIT)
                board.Update(&e);
            if(e.type == SDL_QUIT)
                quit = true;
        }
        board.Update(nullptr);
        Render();
    }
    Close();
    return 0;
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 0xC0, 0xC0, 0xC0, 0xFF);
    SDL_RenderClear(renderer);
    spriteSheet.Render(0, 0, &GetClip(SS::Border_0));
    int leftBorderWidth = GetClip(SS::Border_0).w;
    int rightBorderWidth = GetClip(SS::Border_1).w;
    int leftBorderHeight = GetClip(SS::Border_0).h;
    int rightCornerWidth = GetClip(SS::Outline_4).w;
    spriteSheet.Render(leftBorderWidth - 1, 0, &GetClip(SS::Outline_6), _screenWidth - leftBorderWidth - rightBorderWidth + 1);
    spriteSheet.Render(_screenWidth - rightBorderWidth, 0, &GetClip(SS::Border_1));
    spriteSheet.Render(leftBorderWidth, 44, &GetClip(SS::Outline_0), _screenWidth - leftBorderWidth - rightBorderWidth);
    spriteSheet.Render(16, 15, &GetClip(SS::Timer_Box));
    spriteSheet.Render(_screenWidth - 53 , 15, &GetClip(SS::Timer_Box));
    spriteSheet.Render(0, _screenHeight - 8, &GetClip(SS::Outline_3), _screenWidth);
    spriteSheet.Render(0, leftBorderHeight, &GetClip(SS::Outline_7), -1, _screenHeight - leftBorderHeight);
    spriteSheet.Render(_screenWidth - rightCornerWidth, 55, &GetClip(SS::Outline_2), -1, _screenHeight - 55);
    spriteSheet.Render(_screenWidth - rightCornerWidth, _screenHeight - 8, &GetClip(SS::Outline_4));
    spriteSheet.Render(0, _screenHeight - 9, &GetClip(SS::Outline_1));
    board.Render();
    SDL_RenderPresent(renderer);
}

void Game::ScreenShot()
{
    SDL_Surface* sshot = SDL_CreateRGBSurface(0, _screenWidth, _screenHeight, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
    char file_name[100];
    time_t currTime;
    tm* currTm;
    time(&currTime);
    currTm = localtime(&currTime);
    strftime(file_name, 50, "screenshots/screenshot%Y-%m-%j-%H_%M_%S.png", currTm);
    std::string fName(file_name);
    if (fileExists(fName.c_str()))
    {
        int num = 1;
        std::string tmp = fName + "(" + std::to_string(num) + ")";
        while (fileExists(tmp.c_str()))
        {
            num++;
            tmp = fName + "(" + std::to_string(num) + ")";
            if (num > 40)
                return;
        }
    }
    IMG_SavePNG(sshot, fName.c_str());
}