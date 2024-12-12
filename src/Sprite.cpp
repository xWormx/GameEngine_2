#include "Sprite.h"

Sprite::Sprite(Vec2i p, Vec2i sz) : destRect{p.x, p.y, sz.x, sz.y}
{
}

Sprite::Sprite(Vec2i p, Vec2i sz, std::string srcImage) : destRect{p.x, p.y, sz.x, sz.y}
{
    std::string fullSrcPath = constants::gResPath + "images\\" + srcImage;

    SDL_Surface* surface = IMG_Load(fullSrcPath.c_str());
    
    std::string errMsg = SDL_GetError();
    if(!surface)
        throw std::runtime_error("Sprite: Could't load Surface from IMG_Load(): " + errMsg);
    
    texture = SDL_CreateTextureFromSurface(gameEngine.GetRenderer(), surface);
    
    if(!texture)
        throw std::runtime_error("Sprite: Could't load Texture from CreateTextureFromSurface(): " + errMsg);
    
    srcRect.x = 0;
    srcRect.y = 0;
    SDL_QueryTexture(texture, 0, 0, &srcRect.w, &srcRect.h);
    SDL_FreeSurface(surface);
}

void Sprite::Draw() const
{
    SDL_RenderCopy(gameEngine.GetRenderer(), texture, &srcRect, &destRect);
}

Collider2D& Sprite::GetCollider2D()
{
    if(HasCollider2D())
        return *collider2D;
    
    throw std::runtime_error("There's no Collider2D installed on this Sprite!");
}

bool Sprite::HasCollider2D()
{
    if(collider2D)
        return true;
    
    return false;
}

void Sprite::InstallCollider2D(SDL_Rect boundsRect, bool bStatic)
{
    if(collider2D != nullptr)
        throw std::runtime_error("Sprite already have a Collider2D installed and can only have one Collider2D instance for now!");    
    
    collider2D = Collider2D::GetInstance(boundsRect, bStatic);
}

Sprite::~Sprite()
{
    SDL_DestroyTexture(texture);
    delete collider2D;
}