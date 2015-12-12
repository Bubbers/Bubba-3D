//
// Created by simon on 2015-12-12.
//

#ifndef BUBBA_3D_INPUTMANAGER_H
#define BUBBA_3D_INPUTMANAGER_H

#include <GL/freeglut_std.h>
#include <vector>

typedef void (*mouseMoveCallback) (int x,int y, int delta_x, int delta_y);
typedef void (*specialKeyListener) (int key, int x, int y);

class InputManager{
private:
    std::vector<bool> keysDown;
    bool leftMouseDown, rightMouseDown, middleMouseDown;
    std::vector<mouseMoveCallback> mouseMove;
    std::vector<specialKeyListener> specKeyListeners;
    InputManager();
    int prev_x,prev_y;
    static void handleKeyDown(unsigned char key, int x, int y);
    static void handleKeyUp(unsigned char key, int x, int y);
    static void handleSpecialKey(int key, int x, int y);
    static void handleMouseClick(int button, int state, int x, int y);
    static void handleMouseMove(int x, int y);
public:
    static InputManager* getInstance();
    enum mouseButton { MOUSE_LEFT = GLUT_LEFT_BUTTON, MOUSE_RIGHT = GLUT_RIGHT_BUTTON, MOUSE_MIDDLE = GLUT_MIDDLE_BUTTON};
    enum keyCodes {UP_ARROW = GLUT_KEY_UP, DOWN_ARROW = GLUT_KEY_DOWN,
                   LEFT_ARROW = GLUT_KEY_LEFT, RIGHT_ARROW = GLUT_KEY_RIGHT,
                   KEY_F1 = GLUT_KEY_F1, KEY_F2 = GLUT_KEY_F2, KEY_F3 = GLUT_KEY_F3, KEY_F4 = GLUT_KEY_F4,
                   KEY_F5 = GLUT_KEY_F5, KEY_F6 = GLUT_KEY_F6, KEY_F7 = GLUT_KEY_F7, KEY_F8 = GLUT_KEY_F8,
                   KEY_F9 = GLUT_KEY_F9, KEY_F10 = GLUT_KEY_F10, KEY_F11 = GLUT_KEY_F11, KEY_F12 = GLUT_KEY_F12,
                   KEY_INSERT = GLUT_KEY_INSERT, KEY_PAGE_UP = GLUT_KEY_PAGE_UP, KEY_PAGE_DOWN = GLUT_KEY_PAGE_DOWN,
                   KEY_HOME = GLUT_KEY_HOME, KEY_END = GLUT_KEY_END};
    bool isKeyDown(unsigned char keyCode, bool caseSensitive);
    void addSpecialKeyListener(specialKeyListener skl);
    bool isMouseButtonDown(mouseButton which);
    void addMouseMoveListener(mouseMoveCallback callback);
};

#endif //BUBBA_3D_INPUTMANAGER_H
