#include "GameEngine.h"
#include "StaticSprite.h"
#include "MovableSprite.h"
#include "Level.h"
#include "TextFragment.h"
#include "TextField.h"
#include "Button.h"

#define FPS 60

class Particle;
class Player;
class Explosion : public MovableSprite
{
    public:
        Explosion(Vec2i p, Vec2i sz, std::string srcImage, Player* _owner) : MovableSprite(p, sz, srcImage), owner(_owner) 
        {
            velocity.x = gameEngine.GetRandomNumberInRange(-5, 8);
            velocity.y = gameEngine.GetRandomNumberInRange(-5, 5);
            lifeTime = gameEngine.GetRandomNumberInRange(5, 35);
        }

        Player* GetOwner() { return owner; }

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
        Player* owner;
};

class Particle : public MovableSprite
{
    public:
        Particle(Vec2i p, Vec2i sz, std::string srcImage, Player* _owner) : MovableSprite(p, sz, srcImage), owner(_owner)
        {
            velocity.x = 13;//gameEngine.GetRandomNumberInRange(3, 8);
            velocity.y = gameEngine.GetRandomNumberInRange(-2, 2);
        }

        Player* GetOwner() {return owner;}

        void Tick()
        {
            if(lifeTime-- < 0)
            {
                for(int i = 0; i < 5; i ++)
                {
                    Explosion* p = new Explosion({GetDestRect().x, GetDestRect().y}, {16,16}, "Particle.png", owner);
                    p->SetTag("explosion");
                    p->InstallCollider2D(p->GetDestRect(), false);
                    gameEngine.GetCurrentLevel()->AddSprite(p);
                    gameEngine.PlaySound("release", 1);
                }
                gameEngine.GetCurrentLevel()->RemoveSprite(this);
            }
                
            else
            {
                Move(velocity);
            }
        }

        void OnCollision2D(Sprite* other)
        {
            if(other->GetNameTag() == "enemy")
                lifeTime = -1;
        }
    private:
        int lifeTime = 40;
        Vec2i velocity;
        Player* owner;
};

class HealthBar : public MovableSprite
{
    public:
        HealthBar(Vec2i p, Vec2i sz, std::string srcImage, int maxHP) : MovableSprite(p, sz, srcImage),
                    maxHealth(maxHP), currentHealth(maxHP)
        {
            background = MovableSprite::GetInstance(p, sz, srcImage);
            gameEngine.GetLevelAtIndex(1)->AddSprite(background);
        }

        void ApplyDamage(int damageAmount) 
        { 
            currentHealth -= damageAmount; 
            if(currentHealth < 0)
                currentHealth = 0;
            
            double percentageToAdjust = (double)currentHealth / (double)maxHealth;
            // Multiplicera med background för att den inte ändras.
            double adjustWidth = (double)background->GetDestRect().w * percentageToAdjust;
            SetSize({(int)adjustWidth, background->GetDestRect().h});
            
        }

        void ApplyHealing(int healingAmount) 
        { 
            if(currentHealth < maxHealth)
                currentHealth += healingAmount; 
            if(currentHealth > maxHealth)
                currentHealth = maxHealth;
        }

        void MoveBar(Vec2i speed)
        {
            Move(speed);
            background->Move(speed);
        }

        void DeleteBar()
        {
            gameEngine.GetCurrentLevel()->RemoveSprite(background);
            gameEngine.GetCurrentLevel()->RemoveSprite(this);
        }

        int GetCurrentHealth() { return currentHealth; }

        ~HealthBar()
        {           
        }

    private:
        int maxHealth, currentHealth;
        MovableSprite* background;
};

class Player : public MovableSprite
{
    public:
        Player(Vec2i p, Vec2i sz, std::string srcImage, TextFragment* tfName, TextFragment* _tfScore, int health) : MovableSprite(p, sz, srcImage), 
                name(tfName), tfScore(_tfScore)
        {
            healthBar = new HealthBar({p.x - 30, p.y - 30}, {100, 10}, "HealthBar.png", health); 
            healthBar->SetColor({1, 255, 1, 255});
            gameEngine.GetLevelAtIndex(1)->AddSprite(healthBar);
            name->SetPosition({healthBar->GetDestRect().x, healthBar->GetDestRect().y - name->GetDestRect().h});
            
        }
        
        TextFragment* GetName() { return name; }
        int GetPoints() { return iPoints; }

        void IncreasePoints(int points) { iPoints += points; pointIncreased = true;}
        void UpdatePoints()
        {
            tfScore->SetText(name->GetText() + " " + std::to_string(iPoints));
            pointIncreased = false;
        }
        void Tick()
        {
            if(pointIncreased)
            {
                UpdatePoints();
            }

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
                healthBar->MoveBar(speed);
                speed = {0,0};
            }
            
            AnimateSprite({0, 0}, {32, 80}, 4, 5);

            if(gameEngine.GetKeyPressedOnce('p'))
            {
                if(GetNameTag() == "player1")
                {
                    SDL_Rect destRect = GetDestRect();
                    Particle* p = new Particle({destRect.x + destRect.w, destRect.y + (destRect.h / 2)}, {32,16}, "BulletTest.png", this);
                    p->SetTag("particleShot");
                    SDL_Rect bounds = p->GetDestRect();
                    p->InstallCollider2D(bounds, false);
                    gameEngine.GetCurrentLevel()->AddSprite(p);
                    gameEngine.PlaySound("shot", 3);
                }
            }

            if(gameEngine.GetKeyPressedOnce('b'))
            {
                if(GetNameTag() == "player2")
                {
                    SDL_Rect destRect = GetDestRect();
                    Particle* p = new Particle({destRect.x + destRect.w, destRect.y + (destRect.h / 2)}, {32,16}, "BulletTest.png", this);
                    p->SetTag("particleShot");
                    SDL_Rect bounds = p->GetDestRect();
                    p->InstallCollider2D(bounds, false);
                    gameEngine.GetCurrentLevel()->AddSprite(p);
                    gameEngine.PlaySound("shot", 3);
                }
            }
        }

        void SetMovementKeys(int l, int r, int u, int d) { leftKey = l; rightKey = r; upKey = u; downKey = d;}

        void OnCollision2D(Sprite* other)
        {

        }

    private:
        Vec2i speed = {};
        TextFragment* name;
        TextFragment* tfScore;
        bool shooting = false;
        int leftKey, rightKey, upKey, downKey;
        HealthBar* healthBar;
        
        int iPoints = 0;
        bool pointIncreased = true; // Just nu sätts den till true för att namnet ska uppdateras första gången genom loopen.s
        
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

            if(fCurrentTime >= 0.0f)
            {
                fCurrentTime = fTimeCap - (gameEngine.GetTimeElapsed() - fOriginTime);
                if(fCurrentTime < 0.0f)
                    fCurrentTime = 0.0f;    
                
                char str[10] = {};
                char time[64] = "Time: ";
                sprintf(str, "%0.2f", fCurrentTime);
                strcat(time, str);
                SetText(time);
            }


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
void LoadChooseName() { gameEngine.LoadLevel(3); }
void LoadCredits()  { gameEngine.LoadLevel(2); }
void QuitGame()     { gameEngine.QuitEngine(); }
void Back()         
{ 
    if(gameEngine.GetCurrentLevelIndex() == 2)
        gameEngine.LoadLevel(0);
    else if(gameEngine.GetCurrentLevelIndex() == 3)
        gameEngine.LoadLevel(0);
}

class Enemy : public MovableSprite
{
    public:
        Enemy(Vec2i p, Vec2i sz, std::string srcImage, Vec2i speed, int health) : MovableSprite(p, sz, srcImage), movSpeed(speed)
        {
            healthBar = new HealthBar({p.x, p.y + 50}, {sz.x, 10}, "HealthBar.png", health); 
            healthBar->SetColor({255, 0, 0, 255});
            // Detta funkar bara nu när fiender spawnas efter ett tag
            // För currentLevel från början är main menu.
            gameEngine.GetCurrentLevel()->AddSprite(healthBar);
        }

        void Tick()
        {
            AnimateSprite({0,0}, {100, 100}, 5, 3);
            Move(movSpeed);
            healthBar->MoveBar(movSpeed);
            int leftSide = GetDestRect().x + GetDestRect().w;
            if(leftSide < 0)
            {
                escaped = true;
            }
        }

        void OnCollision2D(Sprite* other)
        {
            if(other->GetNameTag() == "particleShot" || other->GetNameTag() == "explosion")
            {
                gameEngine.GetCurrentLevel()->RemoveSprite(other);
                healthBar->ApplyDamage(1);

                if(healthBar->GetCurrentHealth() <= 0)
                {
                    Particle* p = dynamic_cast<Particle*>(other);
                    Explosion* e = dynamic_cast<Explosion*>(other);

                    // dead är till för att en död fiende inte ger poäng mer än 1 gång när den dör
                    // om partiklar och explosioner colliderar under samma frame.
                    if(!dead)
                    {
                        dead = true;
                        if(p)
                            p->GetOwner()->IncreasePoints(1);
                        else if(e)
                            e->GetOwner()->IncreasePoints(1);
                        
                        
                        gameEngine.GetCurrentLevel()->RemoveSprite(this);
                        healthBar->DeleteBar();
                    }

                }
            }
        }

        ~Enemy()
        {
        }

        const bool& Escaped() { return escaped; }
        
        private:
            Vec2i movSpeed;
            HealthBar* healthBar;
            bool dead = false, escaped = false;
};

class EnemyHandler : public StaticSprite
{
    public:
        EnemyHandler(Level* lvl) : StaticSprite(), level(lvl), stepSpeed(5.0f), currentNumEnemies(0), maxNumEnemies(30) 
        {
            enemyTimer = new Timer({400, 50}, {0, 255, 0, 255}, 20.0f, 5);
            level->AddSprite(enemyTimer);
            timeIntervalStep = enemyTimer->GetTimeCap();
        }

        void Tick()
        {   
            if(enemyTimer->GetCurrentTime() <= timeIntervalStep)
            {
                if(currentNumEnemies <= maxNumEnemies)
                {    
                    timeIntervalStep -= stepSpeed;
                    if(timeIntervalStep <= (enemyTimer->GetTimeCap() / 6.0f))
                        stepSpeed = 1.0f;
                    else if(timeIntervalStep <= (enemyTimer->GetTimeCap() / 2.0f))
                        stepSpeed = 2.0f;

                    AddEnemy();
                }
            }
            
            if((enemyTimer->GetCurrentTime() <= 0.0f) && (currentNumEnemies < maxNumEnemies))
            {
                for(int i = 0; i < maxNumEnemies - currentNumEnemies; i++)
                {
                    AddEnemy();
                }
            }
        }

        void AddEnemy()
        {
            int ry = gameEngine.GetRandomNumberInRange(100, 700);
            Enemy* enemy = new Enemy({gameEngine.GetWindowSize().x, ry}, {150, 150}, "EnemySheet.png", {-1, 0}, 12);
            enemy->SetTag("enemy");
            SDL_Rect rect = enemy->GetDestRect();
            SDL_Rect bounds = {rect.x + 50, rect.y + (rect.h / 2), 
                                50, rect.h / 2};
            enemy->InstallCollider2D(bounds, false);
            level->AddSprite(enemy);
            currentNumEnemies++;
        }

        int GetMaxNumEnemies() { return maxNumEnemies; }
        Timer* GetTimer() { return enemyTimer; }
    private:
        Level* level;
        Timer* enemyTimer;
        double timeIntervalStep;
        double stepSpeed;
        int currentNumEnemies;
        int maxNumEnemies;

        
};

class PlayerNameInputField : public TextField
{
    public:
        PlayerNameInputField(Vec2i p, Vec2i activationSize, SDL_Color c, int fontIndex, int maxInputLength, Player* _player, TextFragment* title) : TextField(p, activationSize, c, fontIndex, maxInputLength),
                                player(_player), titleDescription(title), strLabel(titleDescription->GetText())
        {
            SDL_Rect dr = GetDestRect();
            titleDescription->SetPosition({dr.x, dr.y - titleDescription->GetDestRect().h - 10});
        }

        void OnEnterExecute()
        {
            player->GetName()->SetText(GetCurrentTextInput());
            titleDescription->SetText(strLabel + ": " + player->GetName()->GetText());
            titleDescription->Blink();
        }

    private:
        Player* player;
        TextFragment* titleDescription;
        std::string strLabel;
};

class PlayerStatsHandler : public StaticSprite
{
    public:
        PlayerStatsHandler(Player* p1, Player* p2, EnemyHandler* eh) : StaticSprite(),
                            player1(p1), player2(p2), enemyHandler(eh)
        {
        }

        void Tick()
        {                    
            int totalPoints = player1->GetPoints() + player2->GetPoints();
            int maxPoints = enemyHandler->GetMaxNumEnemies();
         
            if(enemyHandler->GetTimer()->GetCurrentTime() <= 0.0f)
            {
                if(totalPoints == maxPoints)
                    gameEngine.LoadLevel(4);
                
                bool allEnemiesDead = true;
                for(Sprite* e : gameEngine.GetCurrentLevel()->GetSprites())
                {
                    Enemy* enemy = dynamic_cast<Enemy*>(e);
                    if(enemy)
                    {
                        if(!enemy->Escaped())
                            allEnemiesDead = false;
                    }
                }

                if(allEnemiesDead)
                    gameEngine.LoadLevel(4);
                
            }
            
            if(gameEngine.GetCurrentLevelIndex() == 4)
            {
                if(!resultPresented)
                {
                    resultPresented = true;
                    
                    std::string strTotalPoints = "Total points: " + std::to_string(totalPoints) + " / " + std::to_string(maxPoints);
                    std::string strEndMessage = "";
                    SDL_Color color = {};
                    if(totalPoints == maxPoints)
                    {
                        strEndMessage = "Congratulations, you won!";
                        color = {100, 255, 100, 255};
                    }
                    else
                    {
                        strEndMessage = "You let the robots through, NOW WE DIE!!";
                        color = {255, 100, 100, 255};
                    }

                    Vec2i p = {350, 350};                    
                    TextFragment* tfTotalPoints = TextFragment::GetInstance(p, {0, 0}, strTotalPoints, color, 5);
                    TextFragment* tfEndMessage = TextFragment::GetInstance({p.x, p.y + 50}, {0,0}, strEndMessage, color, 4);
                    gameEngine.GetCurrentLevel()->AddSprite(tfTotalPoints);
                    gameEngine.GetCurrentLevel()->AddSprite(tfEndMessage);

                }
            }
        }

    private:
        Player* player1, *player2;
        EnemyHandler* enemyHandler;
        bool resultPresented = false;
};

int main(int argv, char **argc)
{
    gameEngine.SetFps(60);
    gameEngine.LoadSound("shot", "shot.wav");
    gameEngine.LoadSound("explosion", "explosion.wav");
    gameEngine.LoadSound("release", "release.wav");
    gameEngine.LoadMusic("mainTheme", "mainTheme.wav");
    gameEngine.PlayMusic("mainTheme", 3);
    

    // TODO: Ändra så att GetLevelAtIndex returnerar den leveln som har i som sitt interna index,
    // nu levererar den arrayen levels index vilket gör det hela beroende på vilken ordningen
    // gameEngine.AddLevel(somelevel); sker!!
    Level* levelMainMenu = Level::GetInstance(0);
    Level* levelStartGame = Level::GetInstance(1);
    Level* levelCredits = Level::GetInstance(2);
    Level* levelChooseName = Level::GetInstance(3);
    Level* levelEndGame = Level::GetInstance(4);

    gameEngine.AddLevel(levelMainMenu);
    gameEngine.AddLevel(levelStartGame);
    gameEngine.AddLevel(levelCredits);
    gameEngine.AddLevel(levelChooseName);
    gameEngine.AddLevel(levelEndGame);
    
 
    Sprite* mainMenuBkg = StaticSprite::GetInstance({0, 0}, {gameEngine.GetWindowSize().x, gameEngine.GetWindowSize().y}, "MainMenuBackground.png");
    
    TextFragment* text2 = TextFragment::GetInstance({100, 100}, {100, 100}, " ", {0, 255, 255, 255}, 2);
    TextFragment* text1 = TextFragment::GetInstance({100, 100}, {100, 100}, " ", {0, 255, 255, 255}, 2);

    TextFragment* credits1 = TextFragment::GetInstance({(gameEngine.GetWindowSize().x / 2) - 150, 150}, {100, 100}, "CREDITS", {255, 0, 0, 255}, 2);
    TextFragment* credits2 = TextFragment::GetInstance({(gameEngine.GetWindowSize().x / 2) - 170, 250}, {100, 100}, "A Game made by", {255, 0, 0, 255}, 3);
    TextFragment* credits3 = TextFragment::GetInstance({(gameEngine.GetWindowSize().x / 2) - 200, 350}, {100, 100}, "Carl-Johan Larson Eliasson", {255, 0, 0, 255}, 4);
    
    TextFragment* tfScore1 = TextFragment::GetInstance({10, 10}, {0,0}, "Score: ", {100, 255, 255, 255}, 3);
    TextFragment* tfScore2 = TextFragment::GetInstance({10, 50}, {0,0}, "Score: ", {100, 255, 255, 255}, 3);
            
    Player* player1 = new Player({100,100}, {50, 100}, "PersonIdle_Small.png", text1, tfScore1, 50);
    player1->SetTag("player1");
    player1->SetMovementKeys(SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN);
    player1->InstallCollider2D({player1->GetDestRect().x, player1->GetDestRect().y + 3*(player1->GetDestRect().h / 4),
                                player1->GetDestRect().w, player1->GetDestRect().h / 4}, false);


    StaticSprite* textFieldBkg1 = StaticSprite::GetInstance({200, 400}, {385, 60}, "TextField.png");
    TextFragment* fieldTitle1 = TextFragment::GetInstance({0,0}, {0,0}, "Player 1", {1, 255, 1, 255}, 3);
    TextField* tf1 = new PlayerNameInputField({textFieldBkg1->GetDestRect().x, textFieldBkg1->GetDestRect().y + 5}, {385, 60}, {180, 255, 180, 255}, 6, 8, player1, fieldTitle1);
    
    Player* player2 = new Player({100,100}, {30, 80}, "PersonIdle_Small.png", text2, tfScore2, 30);
    player2->SetTag("player2");
    player2->SetMovementKeys('a', 'd', 'w', 's');
    player2->InstallCollider2D({player2->GetDestRect().x, player2->GetDestRect().y + 3*(player2->GetDestRect().h / 4),
                                player2->GetDestRect().w, player2->GetDestRect().h / 4}, false);

    StaticSprite* textFieldBkg2 = StaticSprite::GetInstance({600, 400}, {385, 60}, "TextField.png");
    TextFragment* fieldTitle2 = TextFragment::GetInstance({0,0}, {0,0}, "Player 2", {1, 255, 1, 255}, 3);
    TextField* tf2 = new PlayerNameInputField({textFieldBkg2->GetDestRect().x, textFieldBkg2->GetDestRect().y + 5}, {385, 60}, {180, 255, 180, 255}, 6, 8, player2, fieldTitle2);
   
    Button* _newGameButton  = new UIButton({500, 350}, {200, 100}, "MenuButtons.png", {0, 0},    {800, 0},    {1600, 0},    {800, 600}, LoadChooseName);
    Button* _startButton    = new UIButton({200, 650}, {200, 100}, "MenuButtons.png", {0, 0},    {800, 0},    {1600, 0},    {800, 600}, LoadStarGame);
    Button* _creditsButton  = new UIButton({500, 500}, {200, 100}, "MenuButtons.png", {0, 600},  {800, 600},  {1600, 600},  {800, 600}, LoadCredits);
    Button* _quitButton     = new UIButton({500, 650}, {200, 100}, "MenuButtons.png", {0, 1200}, {800, 1200}, {1600, 1200}, {800, 600}, QuitGame);
    Button* _backButton     = new UIButton({800, 650}, {200, 100}, "MenuButtons.png", {0, 1800}, {800, 1800}, {1600, 1800}, {800, 600}, Back);
    
    EnemyHandler* enemyHandler = new EnemyHandler(levelStartGame);
    
    PlayerStatsHandler* playerStatsHandler = new PlayerStatsHandler(player1, player2, enemyHandler);

    
    levelMainMenu->AddSprite(mainMenuBkg);
    levelMainMenu->AddSprite(_newGameButton);
    levelMainMenu->AddSprite(_creditsButton);
    levelMainMenu->AddSprite(_quitButton);

    levelStartGame->AddSprite(player1);
    levelStartGame->AddSprite(player2);
    levelStartGame->AddSprite(text1);
    levelStartGame->AddSprite(text2);
    levelStartGame->AddSprite(tfScore1);
    levelStartGame->AddSprite(tfScore2);
    levelStartGame->AddSprite(enemyHandler);
    levelStartGame->AddSprite(playerStatsHandler);

    levelCredits->AddSprite(_backButton);
    levelCredits->AddSprite(credits1);
    levelCredits->AddSprite(credits2);
    levelCredits->AddSprite(credits3);

    levelChooseName->AddSprite(mainMenuBkg);
    levelChooseName->AddSprite(textFieldBkg1);
    levelChooseName->AddSprite(textFieldBkg2);
    levelChooseName->AddSprite(fieldTitle1);
    levelChooseName->AddSprite(fieldTitle2);
    levelChooseName->AddSprite(tf1);
    levelChooseName->AddSprite(tf2);
    levelChooseName->AddSprite(_startButton);
    levelChooseName->AddSprite(_backButton);

    levelEndGame->AddSprite(playerStatsHandler);
    levelEndGame->AddSprite(_quitButton);

    gameEngine.Run();

    return 0;
}