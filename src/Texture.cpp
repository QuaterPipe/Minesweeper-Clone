#include "../include/Texture.hpp"
#include "../include/Game.hpp"

Texture::Texture(Game* parent)
: _width(-1), _height(-1), _texture(nullptr), _parent(parent)
{
}

Texture::~Texture()
{
    Free();
}

bool Texture::LoadFromFile(const std::string& path)
{
    Free();
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface)
        std::cerr<<"Unable to load image "<<path<<" SDL_image Error: "<<IMG_GetError()<<"\n";
    else
    {
        //SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(_parent->renderer, loadedSurface);
        if (!newTexture)
            std::cerr<<"Unable to create texture from "<<path<<" SDL Error: "<<SDL_GetError()<<"\n";
        else
        {
            _width = loadedSurface->w;
            _height = loadedSurface->h;
            loadedSurface = nullptr;
        }
        SDL_FreeSurface(loadedSurface);
    }
    _texture = newTexture;
    return _texture;
}

void Texture::Free()
{
    if (_texture)
    {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
        _width = 0;
        _height = 0;
    }
}

int Texture::GetHeight() const
{
    return _height;
}

int Texture::GetWidth() const
{
    return _width;
}

void Texture::Render(int x, int y, SDL_Rect* clip, int width, int height, double angle, SDL_Point* center, SDL_RendererFlip flip) const
{
    SDL_Rect renderQuad = {x, y, _width, _height};
    if (clip)
    {
        renderQuad.w = width == -1 ? clip->w : width;
        renderQuad.h = height == -1 ? clip->h : height;
    }
    SDL_RenderCopyEx(_parent->renderer, _texture, clip, &renderQuad, angle, center, flip);
}