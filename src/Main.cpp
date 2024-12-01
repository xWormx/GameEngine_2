#include "GameEngine.h"


typedef void (*FuncPointer)(int, int);  
typedef void (*VoidFp)();

void Add(int a, int b)
{
    std::cout << a << " + " << b << " = " << a+b << "\n";
}

void Sub(int a, int b)
{
    std::cout << a << " - " << b << " = " << a-b << "\n";
}


class System
{
    public:
        System(){}

        void RegisterKeyCallBack(char key, VoidFp callback)
        {
            keyCallbacks[key].emplace_back(callback);
        }

        void ExecuteCallBacks(char key)
        {
            for(VoidFp callback : keyCallbacks.at(key))
            {
                callback();
            }
        }
    private:
        std::unordered_map<char, std::vector<VoidFp>> keyCallbacks;        
};


class Player
{
    public:
        static Player* GetInstance(Vec2i pos, Vec2i sz, std::string n)
        { return new Player(pos, sz, n);}
        void Move(int x, int y) { position.x += x; position.y += y; }
        void Move() { position += acceleration; }
        void SetAcceleration(Vec2i a) { acceleration = a; }
        void DoMemFp() { std::cout << name << " is executing: " << (obj->*memFp)() << "\n"; }
        void SetMemFp(void (Player::*uniqueFunction)()) { memFp = uniqueFunction;}
        void HasName() { std::cout << " " << name << " ";}
    protected:
        Player(Vec2i pos, Vec2i sz, std::string n) : position(pos), size(sz), name(n){}
    private:
        Vec2i position;
        Vec2i size;
        Vec2i acceleration;
        std::string name;
        Player* obj;        
        void (Player::*memFp)();
};


void Attack()
{
    std::cout << "Attack\n";
}
void Move()
{
    std::cout << "Move\n";
}

#define FPS 60

int main(int argv, char **argc)
{
    GameEngine gameEngine(600, 400, FPS);

    FuncPointer fp1 = Add;
    fp1(2,3);
    FuncPointer fp2 = Sub;
    fp2(4,5);

    Player* player1 = Player::GetInstance({0, 0}, {100, 100}, "MasterPlayer");
    Player* player2 = Player::GetInstance({0, 0}, {99, 9999}, "booPlayer");
    player1->SetMemFp(&player2->HasName);
    player2->SetMemFp(Shoot);

    System sys = {};
    //sys.RegisterKeyCallback('w', &player, &Player::Move);
    sys.RegisterKeyCallBack('a', Move);

    sys.ExecuteCallBacks('a');
    gameEngine.Run();

    return 0;
}