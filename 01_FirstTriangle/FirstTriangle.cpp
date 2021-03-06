#include "FirstTriangle.h"

#include <iostream>
#include <stdexcept>

/*------------------------------------------------------------------*/
// Constants
/*------------------------------------------------------------------*/
const uint32_t WIDTH = 600;
const uint32_t HEIGHT = 800;
const char * TITLE = "First Triangle";

/*------------------------------------------------------------------*/
// Static Function Definition
/*------------------------------------------------------------------*/

static void
initWindow(GLFWwindow * &window) {
    // create window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(
                        WIDTH,
                        HEIGHT,
                        TITLE,
                        nullptr,
                        nullptr);

    if(!window) {
        throw std::runtime_error("failed to create window");
    }

    // make window current
    glfwMakeContextCurrent(window);
}

/*------------------------------------------------------------------*/
static void
initializeVBO(GLuint &vbo) {

    // define vertices
    GLfloat points[] {
            0.0f,  0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
           -0.5f, -0.5f, 0.0f
            };

    // define vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);
}

/*------------------------------------------------------------------*/

static void
initializeVAO(GLuint &vao, GLuint &vbo) {
    // define vertex attribute object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

}

/*------------------------------------------------------------------*/

static void
setupShaders(GLuint &shaderProgram) {
    // define vertex shader
    const char * vertexShader =
        "#version 410 \n"
        "in vec3 vp;"
        "void main() {"
        "   gl_Position = vec4(vp, 1.0);"
        "}";

    // define fragment shader
    const char * fragmentShader =
        "#version 410\n"
        "out vec4 fragColor;"
        "void main() {"
        "   fragColor = vec4(0.5, 0.0, 0.5, 1.0);"
        "}";

    // compile shaders
    GLuint vertShader= glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertexShader, NULL);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragmentShader, NULL);
    glCompileShader(fragShader);

    // attach shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fragShader);
    glAttachShader(shaderProgram, vertShader);
    glLinkProgram(shaderProgram);
}

/*------------------------------------------------------------------*/
// Private Function Definition
/*------------------------------------------------------------------*/

void
FirstTriangle::initOpenGL() {
    // initialize GLFW library
    if(!glfwInit()) {
        std::cerr << "failed to initialize GLFW library" << std::endl;
        exit(1);
    }

    //initialize window
    initWindow(window);

    // setup glew
    glewExperimental = GL_TRUE;
    glewInit();

    // tell GL only to draw pixels closer to the viewer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);   // smaller depth value means closer

    initializeVBO(vbo);
    initializeVAO(vao, vbo);
    setupShaders(shaderProgram);
}

/*------------------------------------------------------------------*/

void
FirstTriangle::mainLoop() {
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

/*------------------------------------------------------------------*/

void
FirstTriangle::cleanup() {
    glfwTerminate();
}

/*------------------------------------------------------------------*/
// Public Class Function Definition
/*------------------------------------------------------------------*/

void
FirstTriangle::run() {
    initOpenGL();
    mainLoop();
    cleanup();
}

/*------------------------------------------------------------------*/
