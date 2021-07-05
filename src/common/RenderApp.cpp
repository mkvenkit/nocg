#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::endl;

#include "RenderApp.h"

namespace nocg {

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void RenderApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// override as needed
void RenderApp::mouseBtnCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        cout << xpos << "," << ypos << endl;
    }
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
        _time = glfwGetTime();

        if ((_time - t) > DELTA) {

            // save 
            t = _time;

            // render 
            render();

            // call step if time elapsed
            if (_time - _lastStep > _tsInterval) {
                step();
                _lastStep = _time;
            }

            glfwSwapBuffers(_window);
            glfwPollEvents();
        }
    }

    glfwDestroyWindow(_window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

// Just clears the screen to a color.
void RenderApp::render()
{

    glViewport(0, 0, _width, _height);
    glClearColor(1.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

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

    // set user pointer
    // https://stackoverflow.com/questions/21799746/how-to-glfwsetkeycallback-for-different-classes/62972123#62972123

    // define a lambda for key press callback
    auto keyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto me = (RenderApp*)glfwGetWindowUserPointer(window);
        me->keyCallback(window, key, scancode, action, mods);
    };
    // set callback
    glfwSetWindowUserPointer(_window, this);
    glfwSetKeyCallback(_window, keyCallback);

    // define a lambda for mouse click callback
    auto mouseBtnCallback = [](GLFWwindow* window, int button, int action, int mods) {
        auto me = (RenderApp*)glfwGetWindowUserPointer(window);
        me->mouseBtnCallback(window, button, action, mods);
    };
    glfwSetMouseButtonCallback(_window, mouseBtnCallback);

    glfwMakeContextCurrent(_window);
    gladLoadGL();
    glfwSwapInterval(1);

}

void RenderApp::_glInit()
{
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glEnable(GL_DEPTH_TEST);

    cout << "GL_VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GL_RENDERER: " << glGetString(GL_RENDERER) << endl;
}

} // namespace nocg