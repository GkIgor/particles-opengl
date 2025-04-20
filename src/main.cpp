#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class WindowApp
{
public:
  WindowApp() : window(nullptr), monitor(nullptr), mode(nullptr) {}

  int run()
  {
    if (!glfwInit())
    {
      std::cerr << "GLFW init falhou\n";
      return -1;
    }

    if (configureWindowHints() != 0)
    {
      std::cerr << "Falha ao configurar hints GLFW\n";
      glfwTerminate();
      return -1;
    }

    window = glfwCreateWindow(mode->width - 100, mode->height - 100, "Particles", monitor, nullptr);

    if (!window)
    {
      std::cerr << "Falha ao criar janela GLFW\n";
      glfwTerminate();
      return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Failed to initialize GLAD" << '\n';
      return -1;
    }

    glViewport(0, 0, mode->width - 100, mode->height - 100);

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << '\n';

    render_setup();

    while (!glfwWindowShouldClose(window))
    {
      setCallbacks();
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
  }

private:
  GLFWwindow *window;
  GLFWmonitor *monitor;
  const GLFWvidmode *mode;

  int configureWindowHints()
  {

    monitor = glfwGetPrimaryMonitor();
    mode = glfwGetVideoMode(monitor);

    if (!mode)
    {
      std::cerr << "Falha ao obter modo de vídeo\n";
      return -1;
    }

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    return 0;
  }

  void setCallbacks()
  {
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window,
                       [](GLFWwindow *w, int key, int scancode, int action, int mods)
                       {
                         static_cast<WindowApp *>(
                             glfwGetWindowUserPointer(w))
                             ->handle_key(w, key, scancode, action, mods);
                       });

    glfwSetCharCallback(window,
                        [](GLFWwindow *w, unsigned int codepoint)
                        {
                          static_cast<WindowApp *>(
                              glfwGetWindowUserPointer(w))
                              ->character_callback(w, codepoint);
                        });

    glfwSetCursorPosCallback(window,
                             [](GLFWwindow *w, double xpos, double ypos)
                             {
                               static_cast<WindowApp *>(
                                   glfwGetWindowUserPointer(w))
                                   ->cursor_position_callback(w, xpos, ypos);
                             });

    glfwSetFramebufferSizeCallback(window,
                                   [](GLFWwindow *w, int width, int height)
                                   {
                                     static_cast<WindowApp *>(
                                         glfwGetWindowUserPointer(w))
                                         ->framebuffer_size_callback(w, width, height);
                                   });
  }

  void
  toggle_full_screen(GLFWwindow *w)
  {
    if (glfwGetWindowMonitor(w) == nullptr)
    {
      glfwSetWindowMonitor(w, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
      return;
    }

    int width, height;
    glfwGetWindowSize(w, &width, &height);

    if (width == mode->width && height == mode->height)
    {
      width = (mode->width / 2.5) + 80;
      height = (mode->height / 1.5) + 80;
      glfwSetWindowMonitor(w, nullptr, 0, 0, width, height, mode->refreshRate);
      return;
    }

    glfwSetWindowMonitor(w, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
  }

  void handle_key(GLFWwindow *w, int key, int scancode, int action, int mods)
  {
    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_F11) == GLFW_PRESS)
    {
      toggle_full_screen(w);
    }
  }

  void character_callback(GLFWwindow *window, unsigned int codepoint)
  {
  }

  void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
  {
  }

  void framebuffer_size_callback(GLFWwindow *window, int width, int height)
  {
    glViewport(0, 0, width, height);
  }

  void render_setup()
  {
    glfwMaximizeWindow(window);
  }
};

int main()
{
  WindowApp app;
  return app.run();
}
