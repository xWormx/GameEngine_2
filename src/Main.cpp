#include "GameEngine.h"
#include "StaticSprite.h"
#include "MovableSprite.h"
#include "Level.h"
#include "TextFragment.h"
#include "TextField.h"
#include "Button.h"

#define FPS 60


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
            velocity.x = 13;//gameEngine.GetRandomNumberInRange(3, 8);
            velocity.y = gameEngine.GetRandomNumberInRange(-2, 2);
        }

        void Tick()
        {
            if(lifeTime-- < 0)
            {
                for(int i = 0; i < 5; i ++)
                {
                    Explosion* p = new Explosion({GetDestRect().x, GetDestRect().y}, {16,16}, "Particle.png");
                    p->SetTag("explosion");
                    p->InstallCollider2D(p->GetDestRect(), false);
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

            if(gameEngine.GetKeyPressedOnce('b'))
            {
                if(GetNameTag() == "player")
                {
                    Particle* p = new Particle({GetDestRect().x, GetDestRect().y}, {32,16}, "BulletTest.png");
                    p->SetTag("particleShot");
                    SDL_Rect bounds = p->GetDestRect();
                    p->InstallCollider2D(bounds, false);
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
        Timer(Vec2i p, SDL_Color c, double timeCap, int fontIndex) : TextFragment(p, {1,1}, " ", c, fontIndex), 
            fTimeCap(timeCap), fCurrentTime(timeCap)
        {
        }

        void Tick()
        {
            if(!started)
            {
                fOriginTime = gameEngine.GetTimeElapsed();
                started = true;
            }

            if(fCurrentTime > 0.0f)
                fCurrentTime = fTimeCap - (gameEngine.GetTimeElapsed() - fOriginTime);
            else
                fCurrentTime = 0.0f;
            char str[10] = {};
            char time[64] = "Time: ";
            sprintf(str, "%0.2f", fCurrentTime);
            strcat(time, str);
            SetText(time);
        }

        const double& GetCurrentTime() { return fCurrentTime; }
        const double& GetTimeCap() { return fTimeCap; }

    private:
        std::string strCurrentTime;
        double fTimeCap, fCurrentTime, fOriginTime = 0.0f;
        bool started = false;
};

class UIButton : public Button
{
    public: 
        UIButton(Vec2i p, Vec2i sz, std::string srcImage, Vec2i normCoord, Vec2i hovCoord, Vec2i pressCoord, Vec2i texSize, void(*releaseCmd)()) : Button(p, sz, srcImage), 
            normalTexCoord(normCoord), hoverTexCoord(hovCoord), pressTexCoord(pressCoord), spriteTexSize(texSize), OnRealeaseCommand(releaseCmd)
        {
            SetSpriteRegion(normalTexCoord, spriteTexSize);
        }

        void OnMouseEnter()
        {
            SetSpriteRegion(hoverTexCoord, spriteTexSize);
        }

        void OnMouseExit()
        {
            SetSpriteRegion(normalTexCoord, spriteTexSize);
        }

        void OnMousePress()
        {
            SetSpriteRegion(pressTexCoord, spriteTexSize);
        }

        void OnMouseRelease()
        {
            OnRealeaseCommand();        
        }

    private:
        Vec2i normalTexCoord, hoverTexCoord, pressTexCoord;
        Vec2i spriteTexSize;
        void(*OnRealeaseCommand)();  

};

void LoadStarGame() { gameEngine.LoadLevel(1); }
void LoadCredits()  { gameEngine.LoadLevel(2); }
void QuitGame()     { gameEngine.QuitEngine(); }
void Back()         
{ 
    if(gameEngine.GetCurrentLevelIndex() == 2)
        gameEngine.LoadLevel(0);
}

class Enemy : public MovableSprite
{
    public:
        Enemy(Vec2i p, Vec2i sz, std::string srcImage, Vec2i speed) : MovableSprite(p, sz, srcImage), movSpeed(speed)
        {
        }

        void Tick()
        {
            AnimateSprite({0,0}, {100, 100}, 5, 3);
            Move(movSpeed);
        }

        void OnCollision2D(Sprite* other)
        {
            if(other->GetNameTag() == "particleShot" || other->GetNameTag() == "explosion")
                gameEngine.GetCurrentLevel()->RemoveSprite(this);
        }

        private:
            Vec2i movSpeed;
};

class EnemyHandler : public StaticSprite
{
    public:
        EnemyHandler(Level* lvl) : level(lvl)
        {
            enemyTimer = new Timer({400, 50}, {0, 255, 0, 255}, 60.0f, 5);
            level->AddSprite(enemyTimer);
            timeIntervalStep = enemyTimer->GetTimeCap();
        }

        void Tick()
        {
            if(enemyTimer->GetCurrentTime() <= timeIntervalStep)
            {
                timeIntervalStep -= stepSpeed;
                if(timeIntervalStep < (enemyTimer->GetTimeCap() / 6.0f))
                    stepSpeed = 1.0f;
                else if(timeIntervalStep < enemyTimer->GetTimeCap() / 2.0f)
                    stepSpeed = 2.0f;

                int ry = gameEngine.GetRandomNumberInRange(150, 800);
                Enemy* enemy = new Enemy({gameEngine.GetWindowSize().x, ry}, {150, 150}, "EnemySheet.png", {-1, 0});
                SDL_Rect bounds = enemy->GetDestRect();
                enemy->InstallCollider2D(bounds, false);
                level->AddSprite(enemy);
            }

        }

    private:
        Level* level;
        Timer* enemyTimer;
        double timeIntervalStep;
        double stepSpeed = 5.0f;
};

int main(int argv, char **argc)
{
    gameEngine.SetFps(60);

    gameEngine.LoadSound("shot", "shot.wav");
    gameEngine.LoadSound("explosion", "explosion.wav");
    gameEngine.LoadSound("release", "release.wav");
    gameEngine.LoadMusic("mainTheme", "mainTheme.wav");
    gameEngine.PlayMusic("mainTheme", 3);
    
    Level* levelMainMenu = Level::GetInstance(0);
    Level* levelStartGame = Level::GetInstance(1);
    Level* levelCredits = Level::GetInstance(2);

 
    Sprite* mainMenuBkg = StaticSprite::GetInstance({0, 0}, {gameEngine.GetWindowSize().x, gameEngine.GetWindowSize().y}, "MainMenuBackground.png");
    
    TextFragment* text1 = TextFragment::GetInstance({100, 100}, {100, 100}, "player 1", {255, 0, 0, 255}, 0);
    TextFragment* text2 = TextFragment::GetInstance({100, 100}, {100, 100}, "player 2", {255, 0, 0, 255}, 1);
    TextFragment* credits1 = TextFragment::GetInstance({(gameEngine.GetWindowSize().x / 2) - 150, 150}, {100, 100}, "CREDITS", {255, 0, 0, 255}, 2);
    TextFragment* credits2 = TextFragment::GetInstance({(gameEngine.GetWindowSize().x / 2) - 170, 250}, {100, 100}, "A Game made by", {255, 0, 0, 255}, 3);
    TextFragment* credits3 = TextFragment::GetInstance({(gameEngine.GetWindowSize().x / 2) - 200, 350}, {100, 100}, "Carl-Johan Larson Eliasson", {255, 0, 0, 255}, 4);

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

    TextField* tf1 = TextField::GetInstance({500, 300}, {255, 0, 0, 255}, 8);

    Button* _startButton    = new UIButton({500, 350}, {200, 100}, "MenuButtons.png", {0, 0},    {800, 0},    {1600, 0},    {800, 600}, LoadStarGame);
    Button* _creditsButton  = new UIButton({500, 500}, {200, 100}, "MenuButtons.png", {0, 600},  {800, 600},  {1600, 600},  {800, 600}, LoadCredits);
    Button* _quitButton     = new UIButton({500, 650}, {200, 100}, "MenuButtons.png", {0, 1200}, {800, 1200}, {1600, 1200}, {800, 600}, QuitGame);
    Button* _backButton     = new UIButton({500, 650}, {200, 100}, "MenuButtons.png", {0, 1800}, {800, 1800}, {1600, 1800}, {800, 600}, Back);
    
    EnemyHandler enemyHandler(levelStartGame);
   
    levelMainMenu->AddSprite(mainMenuBkg);
    levelMainMenu->AddSprite(_startButton);
    levelMainMenu->AddSprite(_creditsButton);
    levelMainMenu->AddSprite(_quitButton);
    levelStartGame->AddSprite(tf1);
    levelStartGame->AddSprite(player);
    levelStartGame->AddSprite(player2);
    levelStartGame->AddSprite(text1);
    levelStartGame->AddSprite(text2);
    levelStartGame->AddSprite(&enemyHandler);
    levelCredits->AddSprite(_backButton);
    levelCredits->AddSprite(credits1);
    levelCredits->AddSprite(credits2);
    levelCredits->AddSprite(credits3);

    gameEngine.AddLevel(levelMainMenu);
    gameEngine.AddLevel(levelCredits);
    gameEngine.AddLevel(levelStartGame);
    
    gameEngine.Run();

    return 0;
}