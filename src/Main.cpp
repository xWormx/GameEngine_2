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
        System(int data) : sysData(data){}
        
        void RegisterKeyCallBack(char key, T* object, void(T::*memFunc)())
        {
            std::cout << "op";
        }
    private:
        int std::unordered_map<char, void(T::*memFunC)()>;
};

int main(int argv, char **argc)
{
    GameEngine gameEngine(600, 400, 60);

    FuncPointer fp1 = Add;
    fp1(2,3);
    FuncPointer fp2 = Sub;
    fp2(4,5);

    gameEngine.Run();

    return 0;
}