#include "GameEngine.h"
#include "StaticSprite.h"
#include "MovableSprite.h"
#include "Level.h"
#include "TextFragment.h"
#include "TextField.h"
#include "Button.h"

#define FPS 60

Level* levelMainMenu = Level::GetInstance(0);

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
                    Explosion* p = new Explosion({GetDestRect().x, GetDestRect().y}, {16,16}, "Particle.png");
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

        void Tick()
        {
            static int s = 3;
            if(gameEngine.GetKeyPressed(rightKey))
                speed.x = s; 
                
            if(gameEngine.GetKeyPressed(leftKey))
                speed.x = -s;
                
            if(gameEngine.GetKeyPressed(upKey))
                speed.y = -s;
                
            if(gameEngine.GetKeyPressed(downKey))
                speed.y = s;

            if(speed.x != 0 || speed.y != 0)
            {
                Move(speed);
                name->Move(speed);
                speed = {0,0};
            }
            AnimateSprite({0, 0}, {32, 80}, 4, 5);
            static int frameTick = 0;


            if(gameEngine.GetKeyPressedOnce('b'))
            {
                if(GetNameTag() == "player")
                {
                    Particle* p = new Particle({GetDestRect().x, GetDestRect().y}, {32,16}, "BulletTest.png");
                    gameEngine.GetCurrentLevel()->AddSprite(p);
                    gameEngine.PlaySound("shot", 6);
                }
            }

        }

        void SetMovementKeys(int l, int r, int u, int d) { leftKey = l; rightKey = r; upKey = u; downKey = d;}

        void OnCollision2D(Sprite* other)
        {
            if(other->GetNameTag() == "player TWO")
            {
                std::cout << "Im: " << GetNameTag() << " getting hit by: " << other->GetNameTag() << "\n";
            }
        }
    private:
        Vec2i speed = {};
        TextFragment* name;
        bool shooting = false;
        int leftKey, rightKey, upKey, downKey;
        
};



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
            if(gameEngine.GetKeyPressed(SDLK_SPACE))
            {
                /*
                    double currentTime = StartTimer(10.0);
                
                    StartTime(double timeCap)
                    {
                        // originStartTime måste sättas i en fast variabel,
                        // som originStartTime = fTimeElapsed;
                        InitializeTimer();
                        double currentTime = timeCap - (fTimeElapsed - originStartTime);
                        return currentTime;
                    }
                */
            }
        }

    private:
        Uint32 currentTime;
        double fTimeElapsed = 0.0f;
        std::string strElapsedTime;
};

class TestButton : public Button
{
    public: 
        TestButton(TextFragment* txt) : Button({600, 600}, {100, 100}, "StartGameButton.png"), text(txt)
        {
            SetSpriteRegion({0, 0}, {800, 600});
        }

        void OnMouseHover()
        {
            Vec2i mp = gameEngine.GetMousePosition();
            if( (mp.x > GetDestRect().x  &&  mp.x < GetDestRect().x + GetDestRect().w) &&
                    mp.y > GetDestRect().y && mp.y < GetDestRect().y + GetDestRect().h)
            {
                mouseHover = true;
                SetSpriteRegion({800, 0}, {800, 600});
                text->SetText("HOVERING");
            }
            else
            {
                mouseHover = false;
                SetSpriteRegion({0, 0}, {800, 600});
                text->SetText("NOT HOVERING");
            }
                
        }

        void OnMousePress()
        {
            if(mouseHover)
            {
                if(gameEngine.GetMousePressed(SDL_BUTTON_LEFT))
                {
                    text->SetText("PRESSED");
                    SetSpriteRegion({1600, 0}, {800, 600});
                }
            }

                
        }

        void OnMouseRelease()
        {

        }

    private:
        TextFragment* text;
        bool mouseHover = false;
    
};

void ChangeLevel()
{
    switch(gameEngine.GetCurrentLevelIndex())
    {
        case 0:
            {
                gameEngine.LoadLevel(1);
            } break;
        case 1:
            {   
                gameEngine.LoadLevel(0);
            } break;
    }
}

int main(int argv, char **argc)
{
    gameEngine.LoadSound("shot", "shot.wav");
    gameEngine.LoadSound("explosion", "explosion.wav");
    gameEngine.LoadSound("release", "release.wav");
    gameEngine.LoadMusic("mainTheme", "mainTheme.wav");
    gameEngine.PlayMusic("mainTheme", 3);
    Sprite* mainMenuBkg = StaticSprite::GetInstance({0, 0}, {gameEngine.GetWindowSize().x, gameEngine.GetWindowSize().y}, "MainMenuBackground.png");
    
    TextFragment* text1 = TextFragment::GetInstance({100, 100}, {100, 100}, "player 1", {255, 0, 0, 255});
    TextFragment* text2 = TextFragment::GetInstance({100, 100}, {100, 100}, "player 2", {255, 0, 0, 255});

    Player* player = new Player({100,100}, {50, 100}, "PersonIdle_Small.png", text1);
    player->SetTag("player");
    player->SetMovementKeys(SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN);
    player->InstallCollider2D({player->GetDestRect().x, player->GetDestRect().y + 3*(player->GetDestRect().h / 4),
                                player->GetDestRect().w, player->GetDestRect().h / 4}, false);


    Player* player2 = new Player({100,100}, {30, 80}, "PersonIdle_Small.png", text2);
    player2->SetTag("player TWO");
    player2->SetMovementKeys('a', 'd', 'w', 's');
    player2->InstallCollider2D({player2->GetDestRect().x, player2->GetDestRect().y + 3*(player2->GetDestRect().h / 4),
                                player2->GetDestRect().w, player2->GetDestRect().h / 4}, false);


    Timer* timer = new Timer({300, 300}, {100, 100}, "Timer", {0, 255, 0, 255});
    
    TextField* tf1 = TextField::GetInstance({500, 300}, {255, 0, 0, 255});
    TextField* tf2 = TextField::GetInstance({500, 400}, {0, 0, 255, 255});

    TextFragment* tfStartGame = TextFragment::GetInstance({600, 600}, {100, 100}, "START GAME", {255, 0, 255, 255});
    Button* testButton = new TestButton(tfStartGame);


    Level* level2 = Level::GetInstance(1);
    
    levelMainMenu->AddSprite(mainMenuBkg);
    levelMainMenu->AddSprite(testButton);
    levelMainMenu->AddSprite(tfStartGame);
    levelMainMenu->AddSprite(tf1);
    levelMainMenu->AddSprite(tf2);
    levelMainMenu->AddSprite(player);
    levelMainMenu->AddSprite(player2);
    
    levelMainMenu->AddSprite(text1);
    levelMainMenu->AddSprite(timer);
    level2->AddSprite(timer);
    gameEngine.SetFps(60);

    gameEngine.AddLevel(levelMainMenu);
    gameEngine.AddLevel(level2);
    gameEngine.RegisterKeyCallback('l', ChangeLevel);


    gameEngine.Run();

    return 0;
}