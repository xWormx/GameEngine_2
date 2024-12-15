#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "SDL2\SDL.h"
#include <unordered_map>

class InputComponent
{
      public:
        void SetKeyCodePressed(const Sint32 keyCode);
        void SetKeyCodeReleased(const Sint32 keyCode);
        void SetMousePressed(const Uint32 button);
        void SetMouseReleased(const Uint32 button);

        void ClearSingleInputKeys();

        SDL_Keycode GetKeyCodeFromEvent(const SDL_Event&) const;
        Uint8 GetMouseButtonFromEvent(const SDL_Event&) const;
        
        bool GetKeyPressed(const Sint32 keyCode) const;
        bool GetKeyPressedOnce(const Sint32 keyCode) const;

        void SetSingleKeyState(const Sint32 keyCode, const bool isPressed);
        bool GetSingleKeyState(const Sint32) const;

        bool GetMousePressed(const Uint8 button) const;

    private:

        std::unordered_map<Sint32, bool> keyStatesLarge;
        Sint32 keyCode_1_32   = 0;
        Sint32 keyCode_33_64  = 0;
        Sint32 keyCode_65_96  = 0;
        Sint32 keyCode_97_128 = 0;

        Sint32 keyCode_1_32_SinglePress   = 0;
        Sint32 keyCode_33_64_SinglePress  = 0;
        Sint32 keyCode_65_96_SinglePress  = 0;
        Sint32 keyCode_97_128_SinglePress = 0;

        Sint32 singlePress_State_1_32   = 0;
        Sint32 singlePress_State_33_64  = 0;
        Sint32 singlePress_State_65_96  = 0;
        Sint32 singlePress_State_97_128 = 0;

        Sint32 keyCode_1_32_ReapetPress   = 0;
        Sint32 keyCode_33_64_ReapetPress  = 0;
        Sint32 keyCode_65_96_ReapetPress  = 0;
        Sint32 keyCode_97_128_ReapetPress = 0;

        Uint32 buttonBitPattern = 0;
};

#endif