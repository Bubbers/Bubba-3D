#Bubba-3D!
This is a small game engine programmed in C++
using Glut and Glew.  
Bubba-3D provides a helpful API for running OpenGL,
while leaving the programmer much freedom to customize.  
  
Currently under heavy development.
***
###Example
    #include "core\Renderer.h"
    
    #define SCREEN_WIDTH			1028
    #define SCREEN_HEIGHT			800
    
    //Rendering
    Renderer *renderer;
    Scene scene;
    Camera *playerCamera;
    Mesh world;
    
    //Sun
    float3 lightPosition;
    Camera *sunCamera;
    
    void display(void)
    {
    	renderer->drawScene(*playerCamera, scene, 0);
    	renderer->swapBuffer();
    }
    
    void loadCameras() {
    	sunCamera = new PerspectiveCamera(lightPosition, make_vector(0.0f, 0.0f, 0.0f), make_vector(0.0f, 1.0f, 0.0f), 45.0f, 1.0f, 0.1f, 600.0f);
    	scene.sun = sunCamera;
    
    	playerCamera = new PerspectiveCamera(
    		make_vector(150.0f, 30.0f, -20.0f),
    		make_vector(5.0f,   5.0f,   0.0f),
    		make_vector(0.0f,   1.0f,   0.0f),
    		45.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.1f, 1000.0f
    		);
    }
    
    int main(int argc, char *argv[])
    {
    	renderer = new Renderer(argc, argv, SCREEN_WIDTH, SCREEN_HEIGHT);
    	glutDisplayFunc(display);
    	renderer->initGL();
    
    	loadCameras();
    	
    	world.loadMesh("scenes/world.obj");
    	world.m_modelMatrix = make_identity<float4x4>();
    	scene.shadowCasters.push_back(&world);
    
    	renderer->start();
    
    	return 0;
    }
