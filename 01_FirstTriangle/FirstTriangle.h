#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*------------------------------------------------------------------*/
// FirstTriangle Class
/*------------------------------------------------------------------*/

class FirstTriangle {
    public:
        void run();

    private:
        void initOpenGL();  // initialize OpenGL state
        void mainLoop();    // main render loop
        void cleanup();     // cleanup/release resources

    private:
        GLFWwindow *window;
        GLuint vbo;
        GLuint vao;
        GLuint shaderProgram;
};

/*------------------------------------------------------------------*/
