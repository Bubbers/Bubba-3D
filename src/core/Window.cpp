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
#include "Window.h"
#include <glutil/glutil.h>
#include <Globals.h>
#include "Logger.h"
#include <JoystickTranslator.h>

Window::Window(int width, int height, std::string title) {
#	if defined(__linux__)
	linux_initialize_cwd();
#	endif // ! __linux__

	this->width = width;
	this->height = height;

	Globals::set(Globals::WINDOW_HEIGHT,height);
	Globals::set(Globals::WINDOW_WIDTH,width);
	
	sf::ContextSettings settings = sf::ContextSettings(32, 8, 0, 3, 3);

	settings.majorVersion = 4;
	settings.minorVersion = 2;
	settings.attributeFlags = sf::ContextSettings::Debug | sf::ContextSettings::Core;
	window = new sf::Window(sf::VideoMode(width, height), title, sf::Style::Default, settings);
	glEnable(GL_FRAMEBUFFER_SRGB);
	initGL();
}

Window::~Window()
{
}

void Window::initGL() {
	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();

	/* Print information about OpenGL and ensure that we've got at a context
	* that supports least OpenGL 3.0. Then setup the OpenGL Debug message
	* mechanism.
	*/
	startupGLDiagnostics();
	setupGLDebugMessages();
}

void Window::resize(unsigned int width, unsigned int height) {
	Globals::set(Globals::Key::WINDOW_HEIGHT, height);
	Globals::set(Globals::Key::WINDOW_WIDTH, width);
	resizeMethod(width, height);
}

void Window::start(unsigned int maxFPS) {
	window->setFramerateLimit(maxFPS);
	resize(this->width, this->height);
	bool running = true;
	sf::Vector2i pos = sf::Mouse::getPosition(*window);
	Globals::set(Globals::Key::MOUSE_WINDOW_X, pos.x);
	Globals::set(Globals::Key::MOUSE_WINDOW_Y, pos.y);

	if (idleMethod == nullptr)
		Logger::logWarning("Renderer: No idle method specified.");
	if (displayMethod == nullptr) {
		Logger::logError("Renderer: No display method specified.");
		return;
	}

	sf::Clock sinceStart, sinceLastIdleMethodCall;

	while (running)
	{
		// handle events
		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				// end the program
				running = false;
			}
			else if (event.type == sf::Event::MouseMoved) {
				Globals::set(Globals::Key::MOUSE_WINDOW_X, event.mouseMove.x);
				Globals::set(Globals::Key::MOUSE_WINDOW_Y, event.mouseMove.y);
			}
			else if (event.type == sf::Event::JoystickDisconnected || event.type == sf::Event::JoystickConnected) {
				JoystickTranslator::getInstance()->updateMapping();
			}
			else if (event.type == sf::Event::Resized)
			{
				resize(event.size.width, event.size.height);
			}

		}

		float dt = sinceLastIdleMethodCall.restart().asSeconds();
		float totTime = sinceStart.getElapsedTime().asSeconds();
		if (idleMethod != nullptr)
			idleMethod(totTime, dt);
		displayMethod(totTime, dt);

		// end the current frame (internally swaps the front and back buffers)
		window->display();
	}
}

void Window::setResizeMethod(void(*resize)(int, int)) {
	resizeMethod = resize;
}

void Window::setDisplayMethod(void(*display)(float, float)) {
	displayMethod = display;
}

void Window::setIdleMethod(void(*idle)(float, float)) {
	idleMethod = idle;
}

sf::Window* Window::getWindow() {
	return window;
}