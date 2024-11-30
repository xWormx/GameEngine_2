#include "GameEngine.h"


void Add(int a, int b)
{
    std::cout << a << " + " << b << " = " << a+b << "\n";
}

void Sub(int a, int b)
{
    std::cout << a << " - " << b << " = " << a-b << "\n";
}

void Shoot()
{
    std::cout << "SHOOT\n";
}

void Move()
{
    std::cout << "Move\n";
}


typedef void (*FpVoid)();
typedef void (*FuncPointer)(int, int);

class System
{
    public:
        System(){}
        
        void RegisterKeyCallback(char key, FpVoid callback)
        {
            keyCallbacks[key].emplace_back(callback);
        }

        void ExecuteCallBacksForKey(char key)
        {
            for(FpVoid callback : keyCallbacks.at(key))
            {
                std::cout << key << " was pressed: ";
                callback();
            }
        }

    private:
        std::unordered_map<char, std::vector<FpVoid>> keyCallbacks;
};

class Player
{
    public:
        static Player* GetInstance(Vec2i p)
        {
            return new Player(p);
        }
        void Move(Vec2i movement)
        {
            pos += movement;
        }
        
    protected:
        Player(Vec2i p) : pos(p){}

    private:
        Vec2i pos;

};

#define FPS 60

int main(int argv, char **argc)
{
    GameEngine gameEngine(600, 400, FPS);

    System sys = {};

    //Player* player = Player::GetInstance({2, 3});

    sys.RegisterKeyCallback('c', Shoot);
    sys.RegisterKeyCallback('c', Move);
    sys.RegisterKeyCallback('a', Shoot);


    sys.ExecuteCallBacksForKey('c');
    sys.ExecuteCallBacksForKey('a');
 
    gameEngine.Run();

    return 0;
}