

#include "engine.h"
#include "camera.h"
#include "glm/ext.hpp"

static bool m_firstMouse = true;
float m_lastX = 0.0f;
float m_lastY = 0.0f;
bool m_mouseRightPressed = false;

Engine::Engine(const char* name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;

}


Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

static bool rightMousePressed = false;
static double lastX = 0.0;
static double lastY = 0.0;

bool Engine::Initialize()
{
  // Start a window
  m_window = new Window(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT);
  glfwSetWindowUserPointer(m_window->getWindow(), this);
  if(!m_window->Initialize())
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  glfwSetCursorPosCallback(m_window->getWindow(), cursorPositionCallBack);
  glfwSetScrollCallback(m_window->getWindow(), scrollCallback);
  glfwSetMouseButtonCallback(m_window->getWindow(), mouseButtonCallback);


  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  while (!glfwWindowShouldClose(m_window->getWindow()))
  {
      ProcessInput();
      Display(m_window->getWindow(), glfwGetTime());
      glfwPollEvents();
  }
  m_running = false;

}

void Engine::ProcessInput()
{
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->getWindow(), true);


    // Update camera animation here.

    float dt = getDT();

    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        m_camera.ProcessKeyboard(GLFW_KEY_W, dt);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        m_camera.ProcessKeyboard(GLFW_KEY_S, dt);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        m_camera.ProcessKeyboard(GLFW_KEY_A, dt);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        m_camera.ProcessKeyboard(GLFW_KEY_D, dt);

}

void Engine::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
 
    glfwGetCursorPos(window, &xpos, &ypos);
    std::cout << "Position: (" << xpos << ":" << ypos << ")";
}

unsigned int Engine::getDT()
{
  //long long TimeNowMillis = GetCurrentTimeMillis();
  //assert(TimeNowMillis >= m_currentTimeMillis);
  //unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  //m_currentTimeMillis = TimeNowMillis;
  //return DeltaTimeMillis;
    return glfwGetTime();
}

long long Engine::GetCurrentTimeMillis()
{
  //timeval t;
  //gettimeofday(&t, NULL);
  //long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  //return ret;
    return (long long) glfwGetTime();
}

void Engine::Display(GLFWwindow* window, double time) {

    m_graphics->Render();
    m_window->Swap();
    m_graphics->HierarchicalUpdate2(time);
}

static void cursorPositionCallBack(GLFWwindow* window, double xpos, double ypos) {
    //cout << xpos << " " << ypos << endl;
    if (!rightMousePressed) return;

    if (m_firstMouse) {
        lastX = xpos;
        lastY = ypos;
        m_firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    engine->m_camera.ProcessMouseMovement(xoffset, yoffset);
}

// Add these new callback functions
void Engine::scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    engine->m_camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Engine::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rightMousePressed = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwGetCursorPos(window, &lastX, &lastY);
        }
        else if (action == GLFW_RELEASE) {
            rightMousePressed = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}
