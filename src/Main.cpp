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

class TestSprite
{
    public:
        virtual void tick() = 0;
        virtual void DoMemFunc(){}
    protected:
        TestSprite(Vec2i pos, Vec2i sz) : position(pos), size(sz) {} 
    private:
        Vec2i position, size;
        
};
template <typename T>
class Player : public TestSprite
{
    public:
        static Player* GetInstance(Vec2i pos, Vec2i sz, std::string n)
        { return new Player(pos, sz, n);}
        void Move(int x, int y) { position.x += x; position.y += y; }
        void Move() { position += acceleration; }
        void SetAcceleration(Vec2i a) { acceleration = a; }
        void DoMemFunc() 
        { 
            std::cout << "\n" << name << " is executing: ";
            (obj->*memFp)();
            std::cout << "\n";
        }
        void SetMemFp(T* p, void (T::*uniqueFunction)()) { obj = p; memFp = uniqueFunction;}
        void HasName() { std::cout << " " << name << " ";}
        void tick()
        {
            DoMemFunc();
        }
    protected:
        Player(Vec2i pos, Vec2i sz, std::string n) : TestSprite(pos, sz), position(pos), size(sz), name(n){}
    private:
        Vec2i position; 
        Vec2i size;
        Vec2i acceleration;
        std::string name;
        T* obj;        
        void (T::*memFp)();
};

class Button
{
    public:
        Button(int d) : data(d) {}
        void Print() { std::cout << "BUTTON DATA: " << data; }
    private:
     int data;
};


class Tile
{
    public:
        Tile(int d) : data(d) {}
        void Print() { std::cout << "TILE DATA: " << data; }
    private:
     int data;
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

    Button* b1 = new Button(12);
    Tile* t1 = new Tile(999);

    Player<Button>* player1 = Player<Button>::GetInstance({0, 0}, {100, 100}, "MasterPlayer");
    Player<Tile>* player2 = Player<Tile>::GetInstance({0, 0}, {99, 9999}, "booPlayer");
    player1->SetMemFp(b1, &Button::Print);
    player2->SetMemFp(t1, &Tile::Print);
    player1->tick();
    player2->tick();
    System sys = {};
    //sys.RegisterKeyCallback('w', &player, &Player::Move);



    gameEngine.Run();

    return 0;
}