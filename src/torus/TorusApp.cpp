#include "Torus.h"
#include "Axis3D.h"
#include "TorusApp.h"

#define M_PI 3.14159265358979323846

TorusApp::TorusApp(int width, int height, const string& appName)
	:RenderApp(width, height, appName)
{
	_torus = std::make_unique<Torus>(0.5, 2, 24, 24);
	_axis = std::make_unique<Axis3D>(10.0);
}

TorusApp::~TorusApp()
{

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
	glm::vec3 eye(5, 5, 5);
	glm::vec3 center(0, 0, 0);
	glm::vec3 up(0, 0, 1);
	glm::mat4 vMat = glm::lookAt(eye, center, up);

	// render torus 
	_torus->render(vMat, pMat);

	// render axis
	_axis->render(vMat, pMat);
	
}
