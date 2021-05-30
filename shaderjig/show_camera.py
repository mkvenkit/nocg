"""
simpleglfw.py

Simple Python OpenGL program that uses PyOpenGL + GLFW to get an 
OpenGL 3.2 context.

Author: Mahesh Venkitachalam
"""

import OpenGL
from OpenGL.GL import *

import numpy, math, sys, os
import glutils
from axes import Axes3D

import glfw

strVS = """
#version 330 core

layout(location = 0) in vec3 aVert;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;
uniform float uTheta;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vColor;
out vec3 fragPos;

mat4 getViewMatrix(vec3 c, vec3 at, vec3 u)
{
    vec3 dir = -normalize(at - c);
    vec3 side = normalize(cross(u, dir));
    vec3 up = normalize(cross(dir, side));

    mat4 T = mat4 (
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(c.x, c.y, c.z, 1.0)
    );

    mat4 R = mat4 (
        vec4(side.x, up.x, dir.x, 0.0),
        vec4(side.y, up.y, dir.y, 0.0),
        vec4(side.z, up.z, dir.z, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );

    return T * transpose(R);
}

void main() {
  
 mat4 rot =  mat4(
		vec4(1.0, 0.0,          0.0,         0.0),
		vec4(0.0, cos(uTheta),  sin(uTheta), 0.0),
	    vec4(0.0, -sin(uTheta), cos(uTheta), 0.0),
	    vec4(0.25, 0.25,          0.0,         1.0)
	    );
  mat4 trans = mat4(
      vec4(1.0, 0.0, 0.0, 0.0),
      vec4(0.0, 1.0, 0.0, 0.0),
      vec4(0.0, 0.0, 1.0, 0.0),
      vec4(0.5, 0.5, 0.0, 1.0)
  );
  // transform vertex
  //gl_Position = uPMatrix * uMVMatrix  * rot * vec4(aVert, 1.0); 

  mat4 M = getViewMatrix(vec3(5, 5, 5), vec3(0, 0, 0), vec3(0, 0, 1));
  gl_Position = uPMatrix * uMVMatrix * M * vec4(aVert, 1.0); 

  // set texture coord
  vTexCoord = aTexCoord;

  vColor = vec3(1.0, 0.0, 0.0);
  vNormal = aNormal; //normalize(aNormal);
  fragPos = aVert; //vec3(uMVMatrix * vec4(aVert, 1.0));
}
"""
strFS = """
#version 330 core

in vec2 vTexCoord;

uniform sampler2D tex2D;
uniform bool showCircle;

out vec4 fragColor;
in vec3 fragPos;
in vec3 vColor;
in vec3 vNormal;

void main() {
    vec3 lightPos = vec3(10.0, 10.0, 10.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(vNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    float ambient = 0.2;
    vec3 result = (ambient + diffuse) * vColor.xyz;

    vec4 col = texture(tex2D, vTexCoord);

    if (col.xyz != vec3(0.0, 0.0, 0.0)) {
        discard;
    }
    else {
        fragColor = vec4(result.xyz, 1.0); //texture(tex2D, vTexCoord);
    }
}
"""

class Scene:    
    """ OpenGL 3D scene class"""
    # initialization
    def __init__(self):
        # create shader
        self.program = glutils.loadShaders(strVS, strFS)

        glUseProgram(self.program)

        self.pMatrixUniform = glGetUniformLocation(self.program, 
                                                   b'uPMatrix')
        self.mvMatrixUniform = glGetUniformLocation(self.program, 
                                                  b'uMVMatrix')
        # texture 
        self.tex2D = glGetUniformLocation(self.program, b'tex2D')

        # define triangle strip vertices 
        vertexData = numpy.array(
            [
                -0.5, 0.5, 0.0,
                -0.5, -0.5, 0.0,
                0.5, 0.5, 0.0, 
                0.5, 0.5, 0.0, 
                -0.5, -0.5, 0.0,
                0.5, -0.5, 0.0
            ], numpy.float32)

        tcData = numpy.array(
            [
                0.0, 0.0, 
                0.0, 1.0,
                1.0, 0.0, 
                1.0, 0.0, 
                0.0, 1.0, 
                1.0, 1.0              
            ], numpy.float32)

        normData = numpy.array(
            [
                0.0, 0.0, 1.0, 
                0.0, 0.0, 1.0, 
                0.0, 0.0, 1.0, 
                0.0, 0.0, 1.0, 
                0.0, 0.0, 1.0, 
                0.0, 0.0, 1.0
            ], numpy.float32)

        # set up vertex array object (VAO)
        self.vao = glGenVertexArrays(1)
        glBindVertexArray(self.vao)
        
        # vertices
        self.vertexBuffer = glGenBuffers(1)
        glBindBuffer(GL_ARRAY_BUFFER, self.vertexBuffer)
        # set buffer data 
        glBufferData(GL_ARRAY_BUFFER, 4*len(vertexData), vertexData, 
                     GL_STATIC_DRAW)
        # enable vertex array
        glEnableVertexAttribArray(0)
        # set buffer data pointer
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, None)

        # texccords
        self.texCoordBuffer = glGenBuffers(1)
        glBindBuffer(GL_ARRAY_BUFFER, self.texCoordBuffer)
        # set buffer data 
        glBufferData(GL_ARRAY_BUFFER, 4*len(tcData), tcData, 
                     GL_STATIC_DRAW)
        # enable vertex array
        glEnableVertexAttribArray(1)
        # set buffer data pointer
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, None)

        # normals
        self.normBuffer = glGenBuffers(1)
        glBindBuffer(GL_ARRAY_BUFFER, self.normBuffer)
        # set buffer data 
        glBufferData(GL_ARRAY_BUFFER, 4*len(normData), normData, 
                     GL_STATIC_DRAW)
        # enable vertex array
        glEnableVertexAttribArray(2)
        # set buffer data pointer
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, None)

        # unbind VAO
        glBindVertexArray(0)

        # time
        self.t = 0 

        # texture
        self.texId = glutils.loadTexture('one.png')
        
    # step
    def step(self):
        # increment angle
        self.t = (self.t + 1) % 360
       
    # render 
    def render(self, pMatrix, mvMatrix):        
        # use shader
        glUseProgram(self.program)
        
        # set proj matrix
        glUniformMatrix4fv(self.pMatrixUniform, 1, GL_FALSE, pMatrix)

        # set modelview matrix
        glUniformMatrix4fv(self.mvMatrixUniform, 1, GL_FALSE, mvMatrix)

         # set shader angle in radians
        glUniform1f(glGetUniformLocation(self.program, 'uTheta'), 
                    math.radians(self.t))

        # enable texture 
        glActiveTexture(GL_TEXTURE0)
        glBindTexture(GL_TEXTURE_2D, self.texId)
        glUniform1i(self.tex2D, 0)

        # bind VAO
        glBindVertexArray(self.vao)
        # draw
        glDrawArrays(GL_TRIANGLES, 0, 6)
        # unbind VAO
        glBindVertexArray(0)


class RenderWindow:
    """GLFW Rendering window class"""
    def __init__(self):

        # save current working directory
        cwd = os.getcwd()

        # initialize glfw - this changes cwd
        glfw.glfwInit()
        
        # restore cwd
        os.chdir(cwd)

        # version hints
        glfw.glfwWindowHint(glfw.GLFW_CONTEXT_VERSION_MAJOR, 3)
        glfw.glfwWindowHint(glfw.GLFW_CONTEXT_VERSION_MINOR, 3)
        glfw.glfwWindowHint(glfw.GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)
        glfw.glfwWindowHint(glfw.GLFW_OPENGL_PROFILE, 
                            glfw.GLFW_OPENGL_CORE_PROFILE)
    
        # make a window
        self.width, self.height = 640, 480
        self.aspect = self.width/float(self.height)
        self.win = glfw.glfwCreateWindow(self.width, self.height, 
                                         b'simpleglfw')
        # make context current
        glfw.glfwMakeContextCurrent(self.win)
        
        # initialize GL
        glEnable(GL_DEPTH_TEST)
        glClearColor(0.0, 0.0, 0.0, 1.0)

        # set window callbacks
        glfw.glfwSetMouseButtonCallback(self.win, self.onMouseButton)
        glfw.glfwSetKeyCallback(self.win, self.onKeyboard)

        # create 3D
        self.scene = Scene()
        self.axes = Axes3D(10)
        # exit flag
        self.exitNow = False

        print("GL_VENDOR: ", glGetString(GL_VENDOR))
        print("GL_RENDERER: ", glGetString(GL_RENDERER))
        print("GL_VERSION: ", glGetString(GL_VERSION))
        print("GL_SHADING_LANGUAGE_VERSION: ", glGetString(GL_SHADING_LANGUAGE_VERSION))

    def onMouseButton(self, win, button, action, mods):
        #print 'mouse button: ', win, button, action, mods
        pass

    def onKeyboard(self, win, key, scancode, action, mods):
        #print 'keyboard: ', win, key, scancode, action, mods
        if action == glfw.GLFW_PRESS:
            # ESC to quit
            if key == glfw.GLFW_KEY_ESCAPE: 
                self.exitNow = True

    def run(self):
        # initializer timer
        glfw.glfwSetTime(0)
        t = 0.0
        while not glfw.glfwWindowShouldClose(self.win) and not self.exitNow:
            # update every x seconds
            currT = glfw.glfwGetTime()
            if currT - t > 0.1:
                # update time
                t = currT

                # set viewport
                self.width, self.height = glfw.glfwGetFramebufferSize(self.win)
                self.aspect = self.width/float(self.height)                
                glViewport(0, 0, self.width, self.height)

                # clear
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
                
                # build projection matrix
                pMatrix = glutils.perspective(35.0, self.aspect, 0.1, 100.0)
                
                mvMatrix = glutils.lookAt([6.0, 6.0, 6.0], [0.0, 0.0, 0.0],
                                          [0.0, 0.0, 1.0])
                # render
                self.scene.render(pMatrix, mvMatrix)
                self.axes.render(pMatrix, mvMatrix)
                self.step()

                glfw.glfwSwapBuffers(self.win)
                # Poll for and process events
                glfw.glfwPollEvents()
        # end
        glfw.glfwTerminate()

    def step(self):
        # step 
        self.scene.step()

# main() function
def main():
    print("Starting simpleglfw. "
          "Press any key to toggle cut. Press ESC to quit.")
    rw = RenderWindow()
    rw.run()

# call main
if __name__ == '__main__':
    main()
