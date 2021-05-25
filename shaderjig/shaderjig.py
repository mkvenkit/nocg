"""
shaderjig.py

A simple Python + OpenGL 4.1 setup to play with the GLSL 
fragment shader. Inspired by shadertoy.

Author: Mahesh Venkitachalam
Website: electronut.in
"""

import OpenGL
from OpenGL.GL import *

import numpy as np
import math, sys, os
import glutils  
import argparse 

import glfw

import os, time

strVS = """
#version 410 core

layout(location = 0) in vec3 aVert;

uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;
uniform float uTheta;

void main() {

  // transform vertex
  gl_Position = uPMatrix * uMVMatrix  * vec4(aVert, 1.0); 
}
"""

class Scene:    
    """ OpenGL 3D scene class"""
    # initialization
    def __init__(self, fsfile):
        # create shader
        self.fsfile = fsfile
        self.program = self.loadShaders(fsfile)

        self.moddate = os.stat(fsfile)[8]

        self.pMatrixUniform = glGetUniformLocation(self.program, 
                                                   b'uPMatrix')
        self.mvMatrixUniform = glGetUniformLocation(self.program, 
                                                  b'uMVMatrix')

        # define triangle strip vertices 
        vertexData = np.array(
            [
                -0.5, 0.5, 0.0,
                -0.5, -0.5, 0.0,
                0.5, 0.5, 0.0, 
                0.5, -0.5, 0.0
            ], np.float32)

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

        # unbind VAO
        glBindVertexArray(0)

        # time
        self.t = 0 

    def loadShaders(self, fsfile):

        # parse FS
        strFS = ""
        with open(fsfile, 'r') as f:
            strFS = f.read()

        program = glutils.loadShaders(strVS, strFS)
        
        return program

    # step
    def step(self):
        # increment angle
        self.t = self.t + 0.1

    # render 
    def render(self, ires, pMatrix, mvMatrix):   

        # has file changed ?
        moddate = os.stat(self.fsfile)[8]
        if moddate > self.moddate:
            self.moddate = moddate
            try:
                self.program = self.loadShaders(self.fsfile)
                print("Shader compilation success! Reloading shader...")
            except RuntimeError as e:
                print(">>>")
                print(e)
                print(">>>")
                print("skipping shader reload!")

        # use shader
        glUseProgram(self.program)
        
        # set resolution
        glUniform2fv(glGetUniformLocation(self.program, 'iRes'), 
                    1, ires)

        # set proj matrix
        glUniformMatrix4fv(self.pMatrixUniform, 1, GL_FALSE, pMatrix)

        # set modelview matrix
        glUniformMatrix4fv(self.mvMatrixUniform, 1, GL_FALSE, mvMatrix)

        glUniform1f(glGetUniformLocation(self.program, 'iTime'), self.t)

        # bind VAO
        glBindVertexArray(self.vao)
        # draw
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4)
        # unbind VAO
        glBindVertexArray(0)


class RenderWindow:
    """GLFW Rendering window class"""
    def __init__(self, fsfile):

        # save current working directory
        cwd = os.getcwd()

        # initialize glfw - this changes cwd
        glfw.glfwInit()
        
        # restore cwd
        os.chdir(cwd)

        # version hints
        glfw.glfwWindowHint(glfw.GLFW_CONTEXT_VERSION_MAJOR, 4)
        glfw.glfwWindowHint(glfw.GLFW_CONTEXT_VERSION_MINOR, 1)
        glfw.glfwWindowHint(glfw.GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)
        glfw.glfwWindowHint(glfw.GLFW_OPENGL_PROFILE, 
                            glfw.GLFW_OPENGL_CORE_PROFILE)
    
        # make a window
        self.width, self.height = 640, 480
        self.aspect = self.width/float(self.height)
        self.win = glfw.glfwCreateWindow(self.width, self.height, 
                                         b'shaderjig')
        # make context current
        glfw.glfwMakeContextCurrent(self.win)
        
        # initialize GL
        glViewport(0, 0, self.width, self.height)
        glEnable(GL_DEPTH_TEST)
        glClearColor(1.0, 1.0, 1.0, 1.0)

        # set window callbacks
        glfw.glfwSetMouseButtonCallback(self.win, self.onMouseButton)
        glfw.glfwSetKeyCallback(self.win, self.onKeyboard)

        # create 3D
        self.scene = Scene(fsfile)
        # exit flag
        self.exitNow = False

        
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

                ires = np.array([self.width, self.height], np.float32)
                
                glViewport(0, 0, self.width, self.height)

                # clear
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
                
                # build projection matrix
                #pMatrix = glutils.perspective(35.0, self.aspect, 0.1, 100.0)
                pMatrix = glutils.ortho(-0.5, 0.5, -0.5, 0.5, 0, 100)
                mvMatrix = np.identity(4, np.float32)
                # render
                self.scene.render(ires, pMatrix, mvMatrix)

                # step
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
    # Command line args are in sys.argv[1], sys.argv[2] ..
    # sys.argv[0] is the script name itself and can be ignored
    # parse arguments
    parser = argparse.ArgumentParser(description="Shaderjig.")
  # add arguments
    parser.add_argument('--fs', dest='fsfile', required=True)
    args = parser.parse_args()

    print("Starting shaderjig. "
          "Press any key to toggle cut. Press ESC to quit.")
    rw = RenderWindow(args.fsfile)
    rw.run()

# call main
if __name__ == '__main__':
    main()
