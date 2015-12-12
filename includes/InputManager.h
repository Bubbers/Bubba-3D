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
                   LEFT_ARROW = GLUT_KEY_LEFT, RIGHT_ARROW = GLUT_KEY_RIGHT};
    bool isKeyDown(unsigned char keyCode, bool caseSensitive);
    void addSpecialKeyListener(specialKeyListener skl);
    bool isMouseButtonDown(mouseButton which);
    void addMouseMoveListener(mouseMoveCallback callback);
};

#endif //BUBBA_3D_INPUTMANAGER_H
