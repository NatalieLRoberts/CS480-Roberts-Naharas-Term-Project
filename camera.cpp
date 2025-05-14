#include "camera.h"

Camera::Camera()
{
    position = glm::vec3(0.0f, 10.0f, -16.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = 90.0f;    // looking along +z
    pitch = 0.0f;
    speed = 5.0f;
    sensitivity = 0.1f;
    UpdateCameraVectors();
}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  view = glm::lookAt( glm::vec3(0.0, 10.0, -16.0), //Eye Position
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( glm::radians(40.f), //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane, 
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}

void Camera::MoveCamera(Direction direction){
  float velocity = speed * 0.1f;

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(front);

  //right vector is perpendicular to front and world up (0,1,0)
  glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

  if (direction == Direction::FORWARD) position += front * velocity;
  if (direction == Direction::BACKWARD) position -= front * velocity;
  if (direction == Direction::LEFT) position -= right * velocity;
  if (direction == Direction::RIGHT) position += right * velocity;

  UpdateViewMat();

}

void Camera::RotateCamera(double mouseX, double mouseY){
    mouseX *= sensitivity;
    mouseY *= sensitivity;

    yaw += mouseX;
    pitch += mouseY;

    // prevents out of bounds movement
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    UpdateCameraVectors();
    UpdateViewMat();
}

void Camera::UpdateViewMat(){
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    view = glm::lookAt(position, position + glm::normalize(front), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::UpdateCameraVectors(){
   glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // Recalculate right and up vectors
    right = glm::normalize(glm::cross(front, worldUp)); 
    up = glm::normalize(glm::cross(right, front));
}
