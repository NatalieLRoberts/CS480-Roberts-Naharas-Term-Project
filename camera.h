#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

enum class Direction { //direction for linear camera movement
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    //void Update(double dt);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    void MoveCamera(Direction direction); //linear movement/translation (WASD)
    void RotateCamera(double mouseX, double mouseY); //rotation (mouse)
    void UpdateViewMat();
    void UpdateCameraVectors();
  
  private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float yaw = -90.0f; //initially looking forward
    float pitch = 0.0f;
    float sensitivity = 0.01f;
    float speed;
};

#endif /* CAMERA_H */
