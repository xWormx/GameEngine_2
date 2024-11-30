#include "GameEngine.h"


typedef void (*FuncPointer)(int, int);  

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

        template <typename T> 
        void RegisterKeyCallBack(char key, T* object, void(T::*memFunc)())
        {
            keyCallbacks[key].emplace_back(object, memFunc);

        }

        template <typename T>
        void ExecuteCallBacks(char key)
        {
            std::vector<std::pair<T*, MemFuncPointer>>& pairVec = keyCallbacks.at(key);

            for(std::pair<T*, MemFuncPointer> p : pairVec)
            {
                std::cout << p.first->p.second << "\n";
            }


        }
    private:
        template <typename T>
        typedef void (T::*MemFuncPointer)();
        std::unordered_map<char, std::vector<std::pair<T*, MemFuncPointer>>> keyCallbacks;
};


class Player
{
    public:
        static Player* GetInstance(Vec2i pos, Vec2i sz)
        { return new Player(pos, sz);}
        void Move(int x, int y) { position.x += x; position.y += y; }

    protected:
        Player(Vec2i pos, Vec2i sz) : position(pos), size(sz){}
    private:
        Vec2i position;
        Vec2i size;
};

#define FPS 60

int main(int argv, char **argc)
{
    GameEngine gameEngine(600, 400, FPS);

    FuncPointer fp1 = Add;
    fp1(2,3);
    FuncPointer fp2 = Sub;
    fp2(4,5);

    Player* player = Player::GetInstance({0, 0}, {100, 100});
    System sys = {};
    sys.RegisterKeyCallback('w', &player, &Player::Move);

    gameEngine.Run();

    return 0;
}