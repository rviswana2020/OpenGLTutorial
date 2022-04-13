#include <GL/glew.h>        // include GLEW and new version of GL on windows
#include <GLFW/glfw3.h>     // GLFW helper library

#include <iostream>

const uint32_t WIDTH = 600;
const uint32_t HEIGHT = 800;
const char * TITLE = "Hello Triangle";

int main() {
    if(!glfwInit()) {
        std::cerr<< "cannot start GLFW" << std::endl;
        return 1;
    }

    // set hint about gl versions/features that we will use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create GLFW window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);

    if(!window) {
        std::cerr << "failed to create GLFW window!!!!" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    // start extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // get version info
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);

    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    // tell GL to only draw onto a pixel if the shape is closer to viewer
    glEnable(GL_DEPTH_TEST);        // enable depth testing
    glDepthFunc(GL_LESS);           // depth-testing interprets a smaller value
                                    // as closer

    // Close GL context and free up GLFW resources
    glfwTerminate();

    return 0;
}
