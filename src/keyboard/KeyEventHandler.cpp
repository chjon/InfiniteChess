#include "KeyEventHandler.h"

namespace ic {
    void KeyEventHandler::init(void(*handleKeyUp)(unsigned char key, int x, int y), void(*handleKeyDown)(unsigned char key, int x, int y)) {
        glutKeyboardFunc(handleKeyDown);
        glutKeyboardUpFunc(handleKeyUp);
    }

    void KeyEventHandler::handle(unsigned char key, int x, int y, bool state) {
        auto keyIt = m_keyMap.find(key);
        if (keyIt == m_keyMap.end()) {
            m_keyMap.emplace(key, state);
        } else {
            keyIt->second = state;
        }

        const auto it = m_funcMap.find(key);
        if (it != m_funcMap.end()) {
            for (KeyFunction keyFunction : *it->second.get()) {
                keyFunction(x, y);
            }
        }
    }

    void KeyEventHandler::registerListener(unsigned char key, KeyFunction keyFunction) {
        auto it = m_funcMap.find(key);
        std::unique_ptr<std::vector<KeyFunction>> keyFunctions = nullptr;
        if (it == m_funcMap.end()) {
            keyFunctions = std::make_unique<std::vector<KeyFunction>>();
        } else {
            keyFunctions = std::move(it->second);
        }
        keyFunctions->push_back(keyFunction);
        m_funcMap.emplace(key, std::move(keyFunctions));
    }

    bool KeyEventHandler::isKeyState(unsigned char key, bool state) const {
        const auto iter = m_keyMap.find(key);
        if (iter == m_keyMap.end()) return !state;
        return iter->second == state;
    }

    bool KeyEventHandler::isDown(unsigned char key) const { return isKeyState(key, true ); }
    bool KeyEventHandler::isUp  (unsigned char key) const { return isKeyState(key, false); }
}