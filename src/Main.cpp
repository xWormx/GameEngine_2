#include "GameEngine.h"
#include "MovableSprite.h"
#include "Level.h"
#include "TextFragment.h"
#include "TextField.h"

void Attack()
{
    std::cout << "Attack\n";
}

void Move()
{
    std::cout << "Move\n";
}

#define FPS 60


static int x = 1;
static int y = 0;
    
Level* level1 = Level::GetInstance(0);

void AddImageToLevel()
{
    MovableSprite* mvSpr = MovableSprite::GetInstance({x * 100, y * 20}, {64, 64}, "Particle.png");
    x++;
    if(x >= 5)
    {
        y++;
        x = 0;
    }
    const SDL_Rect& r = mvSpr->GetDestRect();
    mvSpr->InstallCollider2D({r.x, r.y, r.w, r.h}, false);   
    level1->AddSprite(mvSpr);
    gameEngine.PlaySound("shot", 10);
}

void PlayTestSound()
{
    gameEngine.PlaySound("shot", 10);
}

void RemoveImageFromLevel()
{
    std::vector<Sprite*> s = level1->GetSprites();
    level1->RemoveSprite(s.back());
    x--;
    if(x < 0)
    {
        y--;
        x = 5;
    }       
}

class Explosion : public MovableSprite
{
    public:
        Explosion(Vec2i p, Vec2i sz, std::string srcImage) : MovableSprite(p, sz, srcImage)
        {
            velocity.x = gameEngine.GetRandomNumberInRange(-5, 8);
            velocity.y = gameEngine.GetRandomNumberInRange(-5, 5);
            lifeTime = gameEngine.GetRandomNumberInRange(5, 35);
        }

        void Tick()
        {
            if(lifeTime-- < 0)
            {
                gameEngine.PlaySound("explosion", 10);
                gameEngine.GetCurrentLevel()->RemoveSprite(this);
            }
                
            else
            {
                Move(velocity);
            }
        }
    private:
        int lifeTime = 20;
        Vec2i velocity;
};

class Particle : public MovableSprite
{
    public:
        Particle(Vec2i p, Vec2i sz, std::string srcImage) : MovableSprite(p, sz, srcImage)
        {
            velocity.x = gameEngine.GetRandomNumberInRange(3, 8);
            velocity.y = gameEngine.GetRandomNumberInRange(-5, 5);
        }

        void Tick()
        {
            if(lifeTime-- < 0)
            {
                for(int i = 0; i < 5; i ++)
                {
                    Explosion* p = new Explosion({GetDestRect().x, GetDestRect().y}, {32,32}, "Particle.png");
                    gameEngine.GetCurrentLevel()->AddSprite(p);
                    gameEngine.PlaySound("release", 5);
                }
                gameEngine.GetCurrentLevel()->RemoveSprite(this);
            }
                
            else
            {
                Move(velocity);
            }
        }
    private:
        int lifeTime = 40;
        Vec2i velocity;
};

class Player : public MovableSprite
{
    public:
        Player(Vec2i p, Vec2i sz, std::string srcImage, TextFragment* tfName) : MovableSprite(p, sz, srcImage), name(tfName)
        {
        }

        void OnKeyDown(const SDL_Event& e)
        {
            int s = 3;
            if(e.key.keysym.sym == SDLK_d)
                speed.x = s;

            if(e.key.keysym.sym == SDLK_a)
                speed.x = -s;

            if(e.key.keysym.sym == SDLK_w)
                speed.y = -s;

            if(e.key.keysym.sym == SDLK_s)
                speed.y = s;
            
            if(e.key.keysym.sym == SDLK_SPACE)
                shooting = true;
        }

        void OnKeyUp(const SDL_Event& e)
        {
            if(e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_a)
                speed.x = 0;
            
            if(e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_s)
                speed.y = 0;
            
            if(e.key.keysym.sym == SDLK_SPACE)
                shooting = false;
        }

        void Tick()
        {
            if(speed.x != 0 || speed.y != 0)
            {
                Move(speed);
                name->Move(speed);
            }
            AnimateSprite({0, 0}, {32, 80}, 4, 5);
            static int frameTick = 0;

            if((frameTick++ % 5) == 0)
            {
                if(shooting)
                {
                    Particle* p = new Particle({GetDestRect().x, GetDestRect().y}, {32,32}, "BulletTest.png");
                    gameEngine.GetCurrentLevel()->AddSprite(p);
                    gameEngine.PlaySound("shot", 15);
                }
            }

        }

    private:
        Vec2i speed = {};
        TextFragment* name;
        bool shooting = false;
        
};


void ChangeLevel()
{
    if(gameEngine.GetCurrentLevelIndex() == 0)
        gameEngine.LoadLevel(1);
    else
        gameEngine.LoadLevel(0);
}


class Timer : public TextFragment
{
    public:
        Timer(Vec2i p, Vec2i sz, std::string txt, SDL_Color c) : TextFragment(p, sz, txt, c)
        {}

        void Tick()
        {
            currentTime = SDL_GetTicks();
            fTimeElapsed = currentTime / 1000.0f;
            strElapsedTime = "Time: " + std::to_string(fTimeElapsed);
            SetText(strElapsedTime);
        }

    private:
        Uint32 currentTime;
        double fTimeElapsed = 0.0f;
        std::string strElapsedTime;
};

int main(int argv, char **argc)
{
    gameEngine.LoadSound("shot", "shot.wav");
    gameEngine.LoadSound("explosion", "explosion.wav");
    gameEngine.LoadSound("release", "release.wav");
    gameEngine.LoadMusic("mainTheme", "mainTheme.wav");
    gameEngine.PlayMusic("mainTheme", 3);
    MovableSprite* mvSpr = MovableSprite::GetInstance({0, 0}, {300, 200}, "MainMenuBackground.png");
    TextFragment* text1 = TextFragment::GetInstance({100, 100}, {100, 100}, "YOOOO", {255, 0, 0, 255});

    MovableSprite* player = new Player({100,100}, {30, 80}, "PersonIdle_Small.png", text1);

    player->InstallCollider2D({player->GetDestRect().x, player->GetDestRect().y + 3*(player->GetDestRect().h / 4),
                                player->GetDestRect().w, player->GetDestRect().h / 4}, false);

    Timer* timer = new Timer({300, 300}, {100, 100}, "Timer", {0, 255, 0, 255});
    
    TextField* tf = TextField::GetInstance({500, 300}, {255, 0, 0, 255});

    Level* level2 = Level::GetInstance(1);
    level1->AddSprite(tf);
    level1->AddSprite(player);
    level1->AddSprite(mvSpr);
    level1->AddSprite(text1);
    level1->AddSprite(timer);
    level2->AddSprite(timer);
    gameEngine.SetFps(60);

    gameEngine.AddLevel(level1);
    gameEngine.AddLevel(level2);
    gameEngine.RegisterKeyCallback('c', Attack);
    gameEngine.RegisterKeyCallback('l', ChangeLevel);
    gameEngine.RegisterKeyCallback('a', mvSpr, &MovableSprite::Print);



    gameEngine.Run();

    return 0;
}