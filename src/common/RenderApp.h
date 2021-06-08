#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>
using std::string;


class RenderApp
{

public:

	RenderApp(int width, int height, const string& appName);
	virtual ~RenderApp();

	void run();

	// override this in base class
	virtual void render();

protected:

	int _width;
	int _height;
	float _aspect = 1.0;

	string _appName;


private:

	// hide
	RenderApp() {}

	void _glfwInit();
	void _glInit();

	GLFWwindow* _window = 0;
	
};

