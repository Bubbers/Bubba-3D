#include <string>
#include <SFML/Window.hpp>

#ifndef BUBBA_3D_WINDOW_H
#define BUBBA_3D_WINDOW_H

class Window {
public:
	Window(int width, int height, std::string title);
	~Window();
	
	void initGL();
	void resize(unsigned int width, unsigned int height);

	void setResizeMethod(void(*resize)(int, int));

	void start(unsigned int maxFPS);

	sf::Window* getWindow();

	/**
	* The idle function is called between each frame. Both parameters
	* are time specified in seconds. The first parameter is the total
	* time since the call to Renderer::start(int maxFPS). The second
	* parameter is the time elapsed since the last idle method call.
	*
	* This function should contain all the game logic.
	*/
	void setIdleMethod(void(*idle)(float sinceStart, float sinceLastMethodCall));

	/**
	* The display function is called when a frame should render. Both parameters
	* are time specified in seconds. The first parameter is the total
	* time since the call to Renderer::start(int maxFPS). The second
	* parameter is the time elapsed since the idle method call for the previous frame.
	*
	* In this function Renderer::drawScene() should be called.
	*/
	void setDisplayMethod(void(*display)(float sinceStart, float sinceLastMethodCall));
private:
	sf::Window* window;
	int width, height;
	void(*resizeMethod)(int, int);
	void(*idleMethod)(float, float);
	void(*displayMethod)(float, float);
};

#endif //BUBBA_3D_WINDOW_H