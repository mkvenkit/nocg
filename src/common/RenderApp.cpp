#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::endl;

#include "RenderApp.h"

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

RenderApp::RenderApp(int width, int height, const string& appName)
    :_width(width), _height(height), _appName(appName)
{
    _glfwInit();
    _glInit();
}

RenderApp::~RenderApp()
{

}

void RenderApp::run()
{
    // initializer timer
    glfwSetTime(0);
    double t = 0.0;
    double DELTA = 0.05; // seconds 

    while (!glfwWindowShouldClose(_window)) {

        glfwGetFramebufferSize(_window, &_width, &_height);
        _aspect = _width / (float)_height;

        // update every x seconds
        double currT = glfwGetTime();

        if ((currT - t) > DELTA) {

            // save 
            t = currT;

            // render 
            render();

            glfwSwapBuffers(_window);
            glfwPollEvents();
        }
    }

    glfwDestroyWindow(_window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void RenderApp::render()
{

    glViewport(0, 0, _width, _height);
    glClearColor(1.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void RenderApp::_glfwInit()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    //glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(_width, _height, _appName.c_str(), NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(_window, key_callback);

    glfwMakeContextCurrent(_window);
    gladLoadGL();
    glfwSwapInterval(1);

}

void RenderApp::_glInit()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    cout << "GL_VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GL_RENDERER: " << glGetString(GL_RENDERER) << endl;
}
