#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <stack>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "sphere.h"
#include "mesh.h"
#include "skybox.h"
#include"comet.h"

#define numVBOs 2;
#define numIBs 2;


class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void InitSaturnRing();
    void HierarchicalUpdate2(double dt);
    void Render();

    void SetCamera(Camera* camera);

    GLuint m_ringVAO, m_ringVBO, m_ringTBO;
    int m_ringVertexCount = 0;

    Camera* getCamera() { return m_camera; }

  private:
    std::string ErrorString(GLenum error);

    bool collectShPrLocs();
    void ComputeTransforms (double dt, std::vector<float> speed, std::vector<float> dist,
        std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, 
        glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat);

    stack<glm::mat4> modelStack;

    Camera *m_camera = nullptr;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_positionAttrib;
    GLint m_colorAttrib;
    GLint m_tcAttrib;
    GLint m_hasTexture;

    float MPI = 3.14159265358979323846;

    Sphere* m_sphere;
    Sphere* m_sphere2;
    Sphere* m_sphere3;

    Sphere* m_mercury;
    Sphere* m_venus;
    Sphere* m_mars;
    Sphere* m_jupiter;
    Sphere* m_saturn;
    Sphere* m_uranus;
    Sphere* m_neptune;

    Mesh* m_mesh;

    Skybox* m_skybox;

    Comet* m_halley;

    Texture* m_saturnRingTexture = nullptr; // Add this to hold the texture object
    GLuint m_saturnRingTextureID;

};

#endif /* GRAPHICS_H */
