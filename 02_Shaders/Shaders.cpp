#include "Shaders.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>

/*------------------------------------------------------------------*/
// Constants
/*------------------------------------------------------------------*/
uint32_t GLWidth = 640;
uint32_t GLHeight = 480;
const char *TITLE = "First Triangle";
const char *LOGFILE = "gl.log";
const bool FULL_SCREEN_MODE = false;

/*------------------------------------------------------------------*/
// Static Function Definition
/*------------------------------------------------------------------*/

void
updateFPSCounter(GLFWwindow *window) {
    char tmp[128];

    static int frameCnt = 0;
    static double previousSec = 0;

    double currentSec = glfwGetTime();
    double elapsedSec = currentSec - previousSec;

    if(elapsedSec > .25) {
        previousSec = currentSec;
        double fps = (double)frameCnt / elapsedSec;
        sprintf(tmp, "opengl @fps: %.2f", fps);
        glfwSetWindowTitle(window, tmp);
        frameCnt = 0;
    }
    frameCnt++;
}

/*------------------------------------------------------------------*/

static
bool restartGLLog() {
    #ifndef NDEBUG
        FILE * file = fopen(LOGFILE, "w");

        if(!file) {
            std::cerr << "failed to open log file" << std::endl;
            return false;
        }

        time_t now = time(NULL);
        char *date = ctime(&now);

        fprintf(file, "LOGFILE log. local time %s", date);
        fprintf(file, "build version: %s %s\n\n",__DATE__, __TIME__);
        fclose(file);
    #endif
    return true;
}

/*------------------------------------------------------------------*/

static bool
glLog(const char * msg, ...) {
    #ifndef NDEBUG
       va_list argPtr;

       FILE *file = fopen(LOGFILE, "a");

       if(!file) {
           std::cerr << "failed to open log file" << std::endl;
           return false;
       }

       va_start(argPtr, msg);
       vfprintf(file, msg, argPtr);
       va_end(argPtr);

       va_start(argPtr, msg);
       vfprintf(stderr, msg, argPtr);
       va_end(argPtr);

       fclose(file);
    #endif
    return true;
}

/*------------------------------------------------------------------*/

static void
framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    GLWidth = width;
    GLHeight = height;
}

/*------------------------------------------------------------------*/

static void
initWindow(GLFWwindow * &window) {
    // create window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // We can render full screen and query monitor and video mode
    GLFWmonitor * monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode * videoMode = glfwGetVideoMode(monitor);

    uint32_t w = GLWidth;
    uint32_t h = GLHeight;
    GLFWmonitor *m = nullptr;

    if(FULL_SCREEN_MODE) {
        w = videoMode->width;
        h = videoMode->height;
        m = monitor;
    }

    window = glfwCreateWindow(
                        w,
                        h,
                        TITLE,
                        m,
                        nullptr);

    if(!window) {
        throw std::runtime_error("failed to create window");
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // make window current
    glfwMakeContextCurrent(window);
}

/*------------------------------------------------------------------*/

static std::string
readFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::in);

    // check if the file can be opened
    if(!file.is_open()) {
        throw std::runtime_error("failed to open shader file.!!!");
    }

    std::string buffer;
    std::string line;

    while(!file.eof()) {
        std::getline(file, line);
        buffer.append(line + "\n");
    }

    // close the file
    file.close();

    return buffer;
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
printShaderInfoLog(GLuint shaderIdx) {
    int maxLength = 2048;
    int actualLength = 0;

    char log[2048];
    glGetShaderInfoLog(shaderIdx, maxLength, &actualLength, log);
    std::cout << "Shader info log for GL index: " << shaderIdx << std::endl;
    std::cout << "Shader log: " <<  log << std::endl;
}

/*------------------------------------------------------------------*/

static GLuint
createShader(const char *shaderFile, int shaderType) {
    auto shaderStr = readFile(shaderFile);
    auto  shaderCode = reinterpret_cast<const char *>(shaderStr.c_str());

    // compile shaders
    GLuint shaderIdx= glCreateShader(shaderType);
    glShaderSource(shaderIdx, 1, &shaderCode, NULL);
    glCompileShader(shaderIdx);

    // check for compiler erros
    int params = -1;
    glGetShaderiv(shaderIdx, GL_COMPILE_STATUS, &params);
    if(GL_TRUE != params) {
        fprintf(stderr, "ERROR: GL Shader index %i did not compile\n",
                shaderIdx);
        printShaderInfoLog(shaderIdx);
        exit(1);
    }

    return shaderIdx;
}

/*------------------------------------------------------------------*/

static void
setupShaders(GLuint &shaderProgram) {
   GLuint vertShaderIdx = createShader("shaders/shader.vert", GL_VERTEX_SHADER);
   GLuint fragShaderIdx = createShader("shaders/shader.frag", GL_FRAGMENT_SHADER);

    // attach shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fragShaderIdx);
    glAttachShader(shaderProgram, vertShaderIdx);
    glLinkProgram(shaderProgram);

    // check for compiler erros
    int params = -1;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &params);
    if(GL_TRUE != params) {
        fprintf(stderr, "ERROR: GL  %i did not link\n",
                shaderProgram);
        printShaderInfoLog(shaderProgram);
        exit(1);
    }
}

/*------------------------------------------------------------------*/

static
void glfwErrorCallback(int error, const char *description) {
    glLog("GLFW_ERROR: code %i msg: %s\n", error, description);
}

/*------------------------------------------------------------------*/

static void
logGLParams() {
    GLenum params[] = {
        GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
        GL_MAX_CUBE_MAP_TEXTURE_SIZE,
        GL_MAX_DRAW_BUFFERS,
        GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
        GL_MAX_TEXTURE_IMAGE_UNITS,
        GL_MAX_TEXTURE_SIZE,
        GL_MAX_VARYING_FLOATS,
        GL_MAX_VERTEX_ATTRIBS,
        GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
        GL_MAX_VERTEX_UNIFORM_COMPONENTS,
        GL_MAX_VIEWPORT_DIMS,
        GL_STEREO,
    };

    const char* names[] = {
        "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
        "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
        "GL_MAX_DRAW_BUFFERS",
        "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
        "GL_MAX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_TEXTURE_SIZE",
        "GL_MAX_VARYING_FLOATS",
        "GL_MAX_VERTEX_ATTRIBS",
        "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
        "GL_MAX_VIEWPORT_DIMS",
        "GL_STEREO",
    };

    glLog("\n----------------------------------\n");
    glLog("GL Context Parameters\n");

    for(int i = 0; i < 10; ++i) {
        int v = 0;
        glGetIntegerv(params[i], &v);
        glLog("%s %i\n", names[i], v);
    }

    int v[2] = {};
    glGetIntegerv(params[10], v);
    glLog("%s %i\n", names[10], v[0], v[1]);

    unsigned char s = 0;
    glGetBooleanv(params[11], &s);
    glLog("%s %i\n", names[11], (unsigned int)s);
    glLog("----------------------------------\n");

}

/*------------------------------------------------------------------*/
// Private Function Definition
/*------------------------------------------------------------------*/

void
FirstTriangle::initOpenGL() {
    // (re)start logging
    restartGLLog();

    // store the version
    glLog("GLFW - Starting \n %s\n", glfwGetVersionString());

    // register the error callback function
    glfwSetErrorCallback(glfwErrorCallback);

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

    // get version info
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);

    glLog("Renderer: %s\n", renderer);
    glLog("Version: %s\n", version);

    // log gl parameters
    logGLParams();

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
        updateFPSCounter(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, GLWidth, GLHeight);
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwPollEvents();

        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, 1);
        }

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
