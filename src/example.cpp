

#include "core\Renderer.h"


#define SCREEN_WIDTH			1028
#define SCREEN_HEIGHT			800

#define TICK_PER_SECOND  50

//Rendering
Renderer *renderer;
Scene scene;
Camera *playerCamera;
Mesh world;

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

// Helper function to turn spherical coordinates into cartesian (x,y,z)
float3 sphericalToCartesian(float theta, float phi, float r)
{
	return make_vector(r * sinf(theta)*sinf(phi),
		r * cosf(phi),
		r * cosf(theta)*sinf(phi));
}

void idle(int v)
{
	float time = (1000 / TICK_PER_SECOND) - (float(glutGet(GLUT_ELAPSED_TIME) - timeSinceDraw));
	if (time < 0) {
		glutTimerFunc(1000 / TICK_PER_SECOND, idle, 0);
		timeSinceDraw = float(glutGet(GLUT_ELAPSED_TIME));
		static float startTime = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
		// Here is a good place to put application logic.
		currentTime = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f - startTime;


		// rotate light around X axis, sunlike fashion.
		// do one full revolution every 20 seconds.
		float4x4 rotateLight = make_rotation_x<float4x4>(2.0f * M_PI * currentTime / 20.0f);
		// rotate and update global light position.
		lightPosition = make_vector3(rotateLight * make_vector(30.1f, 450.0f, 0.1f, 1.0f));
		sunCamera->setPosition(lightPosition);

		playerCamera->setLookAt(make_vector(0.0f, camera_target_altitude, 0.0f));
		playerCamera->setPosition(sphericalToCartesian(camera_theta, camera_phi, camera_r));

		renderer->render();
	}
	else {
		glutTimerFunc(time, idle, 0);
	}
}

void display(void)
{
	renderer->drawScene(*playerCamera, scene, currentTime);
	renderer->swapBuffer();
}

void loadMeshes() {
	world.loadMesh("scenes/world.obj");
	world.m_modelMatrix = make_identity<float4x4>();
	scene.shadowCasters.push_back(&world);
}

void loadCameras() {
	sunCamera = new PerspectiveCamera(lightPosition, make_vector(0.0f, 0.0f, 0.0f), make_vector(0.0f, 1.0f, 0.0f), 45.0f, 1.0f, 0.1f, 600.0f);
	scene.sun = sunCamera;

	playerCamera = new PerspectiveCamera(
		sphericalToCartesian(camera_theta, camera_phi, camera_r),
		make_vector(0.0f, camera_target_altitude, 0.0f),
		make_vector(0.0f, 1.0f, 0.0f),
	    45.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.1f, 1000.0f
		);
}

void mouse(int button, int state, int x, int y)
{
	// reset the previous position, such that we only get movement performed after the button
	// was pressed.
	prev_x = x;
	prev_y = y;

	bool buttonDown = state == GLUT_DOWN;

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		leftDown = buttonDown;
		break;
	case GLUT_MIDDLE_BUTTON:
		middleDown = buttonDown;
		break;
	case GLUT_RIGHT_BUTTON:
		rightDown = buttonDown;
	default:
		break;
	}
}

void motion(int x, int y)
{
	int delta_x = x - prev_x;
	int delta_y = y - prev_y;

	if (middleDown)
	{
		camera_r -= float(delta_y) * 0.3f;
		// make sure cameraDistance does not become too small
		camera_r = max(0.1f, camera_r);
	}
	if (leftDown)
	{
		camera_phi -= float(delta_y) * 0.3f * float(M_PI) / 180.0f;
		camera_phi = min(max(0.01f, camera_phi), float(M_PI) - 0.01f);
		camera_theta -= float(delta_x) * 0.3f * float(M_PI) / 180.0f;
	}

	if (rightDown)
	{
		camera_target_altitude += float(delta_y) * 0.1f;
	}
	prev_x = x;
	prev_y = y;
}

int main(int argc, char *argv[])
{
	int w = SCREEN_WIDTH;
	int h = SCREEN_HEIGHT;

	renderer = new Renderer(argc, argv, w, h);

	glutTimerFunc(50, idle, 0);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	renderer->initGL();

	loadCameras();
	loadMeshes();

	renderer->start();

	return 0;
}

