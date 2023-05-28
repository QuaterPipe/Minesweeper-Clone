#pragma once
#include "../include/SDL2/SDL.h"
#include "../include/SDL2/SDL_image.h"
#include <iostream>
#include <string>
class Game;
class Texture
{
    public:
        Texture(Game* parent);
        ~Texture();
        bool LoadFromFile(const std::string& path);
        void Free();
        void Render(int x, int y, SDL_Rect* clip = nullptr, int width = -1, int height = -1, double angle = 0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
        int GetWidth() const;
        int GetHeight() const;
    private:
        int _width;
        int _height;
        SDL_Texture* _texture;
        Game* _parent;
};