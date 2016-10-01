/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */
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