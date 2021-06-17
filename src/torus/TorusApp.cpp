#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

#include "Torus.h"
#include "Plane.h"
#include "Axis3D.h"
#include "Camera.h"
#include "TorusApp.h"

#define M_PI 3.14159265358979323846f

using std::cout;
using std::endl;

TorusApp::TorusApp(int width, int height, const string& appName)
    :RenderApp(width, height, appName)
{
    // set time step interval for animation
    setTStep(0.1);

    _torus = std::make_unique<Torus>(0.5, 2, 64, 64);
    _axis = std::make_unique<Axis3D>(10.0);
    _plane = std::make_unique<Plane>(glm::vec2(5.f, 5.f));

    _printHelp();
}

TorusApp::~TorusApp()
{

}

void TorusApp::_printHelp()
{
    cout << "Display options:" << endl;
    cout << "Press 1 for Gouraud shading." << endl;
    cout << "Press 2 for Phong shading." << endl;
    cout << "Press 3 for Texture." << endl;
    cout << "Press 4 for Procedural Texture." << endl;
    cout << "Press R to toggle rim lighting." << endl;
    cout << "Press A to toggle axis." << endl;
    cout << "Press ESC to exit." << endl;
}

void TorusApp::render()
{
    glViewport(0, 0, _width, _height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //_torus->render();

    // projection transform 
    float fov = 35 * M_PI / 180.0f;
    glm::mat4 pMat = glm::perspective(fov, _aspect, 1.0f, 100.0f);

    // view transform
    glm::vec3 eye(8, 8, 8);
    glm::vec3 center(0, 0, 0);
    glm::vec3 up(0, 0, 1);
    glm::mat4 vMat = glm::lookAt(eye, center, up);

    // render torus 
    _torus->render(vMat, pMat);

    // render plane
    _plane->render(vMat, pMat);

    // render axis
    if (_showAxis) {
        _axis->render(vMat, pMat);
    }

}

void TorusApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_A) {
            // toggle axis display
            _showAxis = !_showAxis;
        }
        if (key == GLFW_KEY_R) {
            // toggle rim lighting
            _torus->toggleRimLighting();
        }
        else {
            _torus->setDisplayMode((TorusDisplayMode)(key - GLFW_KEY_1));
        }
    }

    // call base class method
    RenderApp::keyCallback(window, key, scancode, action, mods);
}

void TorusApp::step()
{
    // compute angle
    static float theta = 0.0;
    theta = theta + 2.0 * M_PI / 180.0f;

    // set model transform
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0.0f, 1.0f, 0.0f));
    _torus->setModelMatrix(trans * rot);

}
