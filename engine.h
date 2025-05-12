#ifndef ENGINE_H
#define ENGINE_H


#include <assert.h>
#include "window.h"
#include "graphics.h"

static void cursorPositionCallBack(GLFWwindow*, double xpos, double ypos);

class Engine
{
private:
    // Window related variables
    Window* m_window;
    const char* m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;


    Graphics* m_graphics;

    bool m_running;

    friend void mouseButtonCallback(GLFWwindow*, int, int, int);
    friend void scrollCallback(GLFWwindow*, double, double);
    friend void cursorPositionCallBack(GLFWwindow*, double, double);

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);


  public:
    Engine(const char*  name, int width, int height);
    
    ~Engine();
    bool Initialize();
    void Run();
    void ProcessInput();
    unsigned int getDT();
    long long GetCurrentTimeMillis();
    void Display(GLFWwindow*, double);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

    static void SetWindowUserPointer();

    Camera m_camera;
   
    
};

#endif // ENGINE_H
