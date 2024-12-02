#include "Sprite.h"

Sprite::Sprite(Vec2i p, Vec2i sz, std::string srcImage) : destRect{p.x, p.y, sz.x, sz.y}
{
    std::string fullSrcPath = constants::gResPath + "images\\" + srcImage;

    SDL_Surface* surface = IMG_Load(fullSrcPath.c_str());
    std::string errMsg = SDL_GetError();
    if(!surface)
        throw std::runtime_error("Sprite: Could't load Surface from IMG_Load(): " + errMsg);
    texture = SDL_CreateTextureFromSurface(gameEngine.GetRenderer(), surface);

}

void Sprite::Draw() const
{
    SDL_RenderCopy(gameEngine.GetRenderer(), texture, &destRect, &srcRect);
}

Sprite::~Sprite()
{
    SDL_DestroyTexture(texture);
}