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

template <typename T> 
class System
{
    public:
        System(){}
        
        void RegisterKeyCallBack(char key, T* object, void(T::*memFunc)())
        {
            keyCallbacks[key].emplace_back(object, memFunc);

        }

        void ExecureCallBacks(char key)
        {
        }
    private:
        typedef void (T::*MemFuncPointer)();
        std::unordered_map<char, std::vector<std::pair<T*, MemFuncPointer>>> keyCallbacks;
};

#define FPS 60

int main(int argv, char **argc)
{
    GameEngine gameEngine(600, 400, FPS);

    FuncPointer fp1 = Add;
    fp1(2,3);
    FuncPointer fp2 = Sub;
    fp2(4,5);

    gameEngine.Run();

    return 0;
}