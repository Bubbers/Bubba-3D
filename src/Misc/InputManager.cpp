//
// Created by simon on 2015-12-12.
//

#include <ctype.h>
#include <stdio.h>
#include "InputManager.h"

InputManager* InputManager::getInstance() {

    static InputManager instance; // Guaranteed to be destroyed.
    // Instantiated on first use.
    return &instance;

}

void InputManager::addSpecialKeyListener(specialKeyListener skl) {
    specKeyListeners.push_back(skl);
}

void InputManager::handleKeyUp(unsigned char key, int x, int y) {
    InputManager::getInstance()->keysDown[key] = false;
}

void InputManager::handleKeyDown(unsigned char key, int x, int y) {
    InputManager::getInstance()->keysDown[(int)key] = true;
}

void InputManager::handleSpecialKey(int key, int x, int y) {
    InputManager* im = InputManager::getInstance();
    for (auto it = im->specKeyListeners.begin() ; it != im->specKeyListeners.end(); ++it)
        (*it)(key,x,y);
}

void InputManager::handleMouseMove(int x, int y) {
    InputManager* im = InputManager::getInstance();
    for(auto it = im->mouseMove.begin() ; it != im->mouseMove.end(); ++it)
        (*it)(x,y,x - im->prev_x,y - im->prev_y);
    im->prev_x = x;
    im->prev_y = y;
}

void InputManager::handleMouseClick(int button, int state, int x, int y) {
    InputManager* im = InputManager::getInstance();
    switch(button){
        case GLUT_MIDDLE_BUTTON:
            im->middleMouseDown = state == GLUT_DOWN;
            break;
        case GLUT_LEFT_BUTTON:
            im->leftMouseDown = state == GLUT_DOWN;
            break;
        case GLUT_RIGHT_BUTTON:
            im->rightMouseDown = state == GLUT_DOWN;
            break;
        default:
            break;
    }
    im->prev_x = x;
    im->prev_y = y;
}

InputManager::InputManager() {
    keysDown = std::vector<bool>(256,false);
    leftMouseDown = rightMouseDown = middleMouseDown = false;
    mouseMove = std::vector<mouseMoveCallback>();
    specKeyListeners = std::vector<specialKeyListener>();
    prev_x = prev_y = 0;

    glutKeyboardFunc(InputManager::handleKeyDown);
    glutKeyboardUpFunc(InputManager::handleKeyUp);
    glutSpecialFunc(InputManager::handleSpecialKey);
    glutMouseFunc(InputManager::handleMouseClick);
    glutMotionFunc(InputManager::handleMouseMove);

}

bool InputManager::isKeyDown(unsigned char keyCode, bool caseSensitive){
    if(caseSensitive)
        return keysDown[(int)keyCode];
    else {
        int keyCodeU = toupper(keyCode), keyCodeL = tolower(keyCode);
        return keysDown[keyCodeL] || keysDown[keyCodeU];
    }
}

bool InputManager::isMouseButtonDown(mouseButton which) {
    switch(which){
    case MOUSE_LEFT:
        return leftMouseDown;
    case MOUSE_RIGHT:
        return rightMouseDown;
    case MOUSE_MIDDLE:
        return middleMouseDown;
    }
}

void InputManager::addMouseMoveListener(mouseMoveCallback callback) {
    mouseMove.push_back(callback);
}
