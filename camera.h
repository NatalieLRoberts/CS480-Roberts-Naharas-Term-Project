#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();

    Camera(glm::vec3 position = glm::vec3(0.0f, 20.0f, 50.0f));
  
    void Update(float dt);
    void ProcessKeyboard(int key, float dt);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
    glm::mat4 GetViewMatrix();
    float GetZoom() const { return zoom; }

  private:

    double x = 0.0;
    double y = 10.0;
    double z = -16.0;
    glm::mat4 projection;
    glm::mat4 view;

    void updateVectors();

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float yaw = -90.0f;
    float pitch = 0.0f;
    float movementSpeed = 5.0f;
    float mouseSensitivity = 0.1f;
    float zoom = 45.0f;
};

#endif /* CAMERA_H */
