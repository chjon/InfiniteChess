#pragma once
#include "../Common.h"

namespace qb {
    class KeyEventHandler {
        using KeyFunction = std::function<void(int x, int y)>;
    public:
        static void init(void(*handleKeyUp)(unsigned char key, int x, int y), void(*handleKeyDown)(unsigned char key, int x, int y));
        void handle(unsigned char key, int x, int y, bool state);
        void registerListener(unsigned char key, KeyFunction keyFunction);

        bool isDown(unsigned char key) const;
        bool isUp  (unsigned char key) const;

    private:
        bool isKeyState(unsigned char key, bool state) const;
        
        std::map<unsigned char, bool> m_keyMap; // True if key down
        std::map<unsigned char, std::unique_ptr<std::vector<KeyFunction>>> m_funcMap;
    };
}