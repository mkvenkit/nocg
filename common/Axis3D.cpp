#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/common.hpp>
#include <glm/matrix.hpp>

#include "ProgramLoader.h"

#include "Axis3D.h"

Axis3D::Axis3D(float length)
	:_length(length)
{
	// load program 
	vector<string> shaderFiles = { "axis3d.vert", "axis3d.frag" };
	_program = ProgramLoader::load(shaderFiles);

	// create geometry 
	_createGeometry();

}

Axis3D::~Axis3D()
{

}

void Axis3D::_createGeometry()
{
	glCreateVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	static const float pos[] = {	0.0f, 0.0f, 0.0f,
									_length, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f,
									0.0f, _length, 0.0f,
									0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, _length
	};
	GLuint buffer;
	_vertexCount = 6;

	// Get create two buffers
	glCreateBuffers(1, &buffer);
	// Initialize the first buffer
	glNamedBufferStorage(buffer, sizeof(pos), pos, 0);
	// Bind it to the vertex array - offset zero, stride = sizeof(vec3)
	glVertexArrayVertexBuffer(_vao, 0, buffer, 0, 3 * sizeof(float));
	// Tell OpenGL what the format of the attribute is
	glVertexArrayAttribFormat(_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	// Tell OpenGL which vertex buffer binding to use for this attribute
	glVertexArrayAttribBinding(_vao, 0, 0);

	// Enable the attribute
	glEnableVertexArrayAttrib(_vao, 0);


	glBindVertexArray(0);

}

void Axis3D::render(const glm::mat4& vMat, const glm::mat4& pMat)
{
	glUseProgram(_program);

	// set matrices
	GLint vMatLoc = glGetUniformLocation(_program, "vMat");
	glUniformMatrix4fv(vMatLoc, 1, GL_FALSE, &vMat[0][0]);

	GLint pMatLoc = glGetUniformLocation(_program, "pMat");
	glUniformMatrix4fv(pMatLoc, 1, GL_FALSE, &pMat[0][0]);


	glBindVertexArray(_vao);

	glPointSize(10.0);
	// draw X axis
	glUniform3f(glGetUniformLocation(_program, "uColor"),
		1.0, 0.0, 0.0);
	glDrawArrays(GL_LINES, 0, 2);
	// draw Y axis
	glUniform3f(glGetUniformLocation(_program, "uColor"),
		0.0, 1.0, 0.0);
	glDrawArrays(GL_LINES, 2, 2);
	// draw Z axis
	glUniform3f(glGetUniformLocation(_program, "uColor"),
		0.0, 0.0, 1.0);
	glDrawArrays(GL_LINES, 4, 2);

	glBindVertexArray(0);
	glUseProgram(0);

}
