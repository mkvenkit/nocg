#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/common.hpp>
#include <glm/matrix.hpp>

#include "ProgramLoader.h"
#include "Torus.h"


Torus::Torus(float r, float R, int nr, int nR)
    :_r(r), _R(R), _nr(nr), _nR(nR),
    Render3D()
{
    // load program 
    vector<string> shaderFiles = { "torus.vert", "torus.frag" };
    _program = ProgramLoader::load(shaderFiles);

    // create geometry 
    _createGeometry();
}

Torus::~Torus()
{

}

void Torus::render(const glm::mat4& vMat, const glm::mat4& pMat)
{
    glUseProgram(_program);

    // set matrices
    GLint vMatLoc = glGetUniformLocation(_program, "vMat");
    glUniformMatrix4fv(vMatLoc, 1, GL_FALSE, &vMat[0][0]);

    GLint pMatLoc = glGetUniformLocation(_program, "pMat");
    glUniformMatrix4fv(pMatLoc, 1, GL_FALSE, &pMat[0][0]);


    glBindVertexArray(_vao);

    glPointSize(10.0);
    // draw 
    glDrawArrays(GL_TRIANGLES, 0, _vertexCount);

    glBindVertexArray(0);
    glUseProgram(0);
}

void Torus::_createGeometry()
{
    glCreateVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    static const float pos[] = { 0.0f, 0.0f, 0.0f,
                                    1.0f, 0.0f, 0.0f,
                                    1.0f, 1.0f, 0.0f
    };
    GLuint buffer[2];
    _vertexCount = 3;

    // Get create two buffers
    glCreateBuffers(2, &buffer[0]);
    // Initialize the first buffer
    glNamedBufferStorage(buffer[0], sizeof(pos), pos, 0);
    // Bind it to the vertex array - offset zero, stride = sizeof(vec3)
    glVertexArrayVertexBuffer(_vao, 0, buffer[0], 0, 3 * sizeof(float));
    // Tell OpenGL what the format of the attribute is
    glVertexArrayAttribFormat(_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    // Tell OpenGL which vertex buffer binding to use for this attribute
    glVertexArrayAttribBinding(_vao, 0, 0);

    // Enable the attribute
    glEnableVertexArrayAttrib(_vao, 0);


    glBindVertexArray(0);
}
