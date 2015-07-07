

#include "Renderer.h"
#include "glutil/glutil.h"
#include "testa.h"


#define SCREEN_WIDTH			1028
#define SCREEN_HEIGHT			800

#define TICK_PER_SECOND  50

//Rendering
Renderer *renderer;
Logger *logger;
//Scene scene;
//Camera *playerCamera;
//Mesh world;

//Time
float currentTime = 0;
float timeSinceDraw = 0;

//Sun
float3 lightPosition;
Camera *sunCamera;

//Mouse interaction
bool leftDown = false;
bool middleDown = false;
bool rightDown = false;
int prev_x = 0;
int prev_y = 0;

float camera_theta = M_PI / 1.0f;
float camera_phi = M_PI / 4.0f;
float camera_r = 30.0;
float camera_target_altitude = 25.2;




//Main program

int main(int argc, char **argv) {

    glutInit(&argc, argv);

    /*Setting up  The Display
    /    -RGB color model + Alpha Channel = GLUT_RGBA
    */
    glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE);

    //Configure Window Postion
    glutInitWindowPosition(50, 25);

    //Configure Window Size
    glutInitWindowSize(480,480);

    //Create Window
    glutCreateWindow("Hello OpenGL");


    //Call to the drawing function
    glutDisplayFunc(draw);

    // Loop require by OpenGL
    glutMainLoop();
    return 0;
}
