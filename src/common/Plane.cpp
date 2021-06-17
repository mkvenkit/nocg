#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/common.hpp>
#include <glm/matrix.hpp>

#include <math.h>
#include <iostream>

#include "Utils.h"

#include "Plane.h"


using namespace nocg;

Plane::Plane(const vec2& dims)
    :_dims(dims)
{
    // load program 
    vector<string> shaderFiles = { "plane.vert", "plane.frag" };
    _program = loadShaders(shaderFiles);

    // create geometry 
    _createGeometry();

    // load texture
    _textureID = loadTexture("text.png", true);
}

Plane::~Plane()
{

}

void Plane::render(const glm::mat4& vMat, const glm::mat4& pMat)
{
    glUseProgram(_program);

    // set model matrix
    GLint mMatLoc = glGetUniformLocation(_program, "mMat");
    glUniformMatrix4fv(mMatLoc, 1, GL_FALSE, &_modelMat[0][0]);


    // set view matrix
    GLint vMatLoc = glGetUniformLocation(_program, "vMat");
    glUniformMatrix4fv(vMatLoc, 1, GL_FALSE, &vMat[0][0]);

    // set projection matrix 
    GLint pMatLoc = glGetUniformLocation(_program, "pMat");
    glUniformMatrix4fv(pMatLoc, 1, GL_FALSE, &pMat[0][0]);

    // texture settings
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureID);
    GLint samplerLoc = glGetUniformLocation(_program, "sampler");
    glUniform1i(samplerLoc, 0);

    glBindVertexArray(_vao);

    // draw 
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertexCount);

    glBindVertexArray(0);
    glUseProgram(0);
}

void Plane::_createPlane()
{
    // Plane is centered on XY plane at Z=0
    _vertices = {   
        -_dims.x, _dims.y, 0.0f,
        _dims.x, _dims.y, 0.0f,
        -_dims.x, -_dims.y, 0.0f,
        _dims.x, -_dims.y, 0.0f,
    };
    _normals = { 
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };
    _texCoords = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };
}

void Plane::_createGeometry()
{
    glCreateVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    _createPlane();
    _vertexCount = _vertices.size() / 3;

    GLuint buffer[4];

    // create buffers
    glCreateBuffers(4, buffer);

    // vertices 

    // Initialize the first buffer
    glNamedBufferStorage(buffer[0], sizeof(float) * _vertices.size(), _vertices.data(), 0);
    // Bind it to the vertex array - offset zero, stride = sizeof(vec3)
    glVertexArrayVertexBuffer(_vao, 0, buffer[0], 0, 3 * sizeof(float));
    // Tell OpenGL what the format of the attribute is
    glVertexArrayAttribFormat(_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    // Tell OpenGL which vertex buffer binding to use for this attribute
    glVertexArrayAttribBinding(_vao, 0, 0);

    // Enable the attribute
    glEnableVertexArrayAttrib(_vao, 0);

    // normals 

    // Initialize the first buffer
    glNamedBufferStorage(buffer[1], sizeof(float) * _normals.size(), _normals.data(), 0);
    // Bind it to the vertex array - offset zero, stride = sizeof(vec3)
    glVertexArrayVertexBuffer(_vao, 1, buffer[1], 0, 3 * sizeof(float));
    // Tell OpenGL what the format of the attribute is
    glVertexArrayAttribFormat(_vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
    // Tell OpenGL which vertex buffer binding to use for this attribute
    glVertexArrayAttribBinding(_vao, 1, 1);

    // Enable the attribute
    glEnableVertexArrayAttrib(_vao, 1);

    // tex coords 

    // Initialize the first buffer
    glNamedBufferStorage(buffer[2], sizeof(float) * _texCoords.size(), _texCoords.data(), 0);
    // Bind it to the vertex array - offset zero, stride = sizeof(vec3)
    glVertexArrayVertexBuffer(_vao, 2, buffer[2], 0, 2 * sizeof(float));
    // Tell OpenGL what the format of the attribute is
    glVertexArrayAttribFormat(_vao, 2, 2, GL_FLOAT, GL_FALSE, 0);
    // Tell OpenGL which vertex buffer binding to use for this attribute
    glVertexArrayAttribBinding(_vao, 2, 2);

    // Enable the attribute
    glEnableVertexArrayAttrib(_vao, 2);

    // tangents 
#if 0
    // Initialize the first buffer
    glNamedBufferStorage(buffer[3], sizeof(float) * _tangents.size(), _tangents.data(), 0);
    // Bind it to the vertex array - offset zero, stride = sizeof(vec3)
    glVertexArrayVertexBuffer(_vao, 3, buffer[3], 0, 3 * sizeof(float));
    // Tell OpenGL what the format of the attribute is
    glVertexArrayAttribFormat(_vao, 3, 3, GL_FLOAT, GL_FALSE, 0);
    // Tell OpenGL which vertex buffer binding to use for this attribute
    glVertexArrayAttribBinding(_vao, 3, 3);

    // Enable the attribute
    glEnableVertexArrayAttrib(_vao, 3);
#endif

    glBindVertexArray(0);

    // clear memory
    clear();
}

