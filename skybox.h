#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces);
    ~Skybox();
    void Render(const glm::mat4& view, const glm::mat4& projection);

private:
    GLuint m_vao, m_vbo;
    GLuint m_cubemapTex;
    GLuint m_shaderProgram;

    void LoadCubemap(const std::vector<std::string>& faces);
    void SetupSkyboxGeometry();
    GLuint CreateShaderProgram();
    void CheckShaderError(GLuint shader, std::string type);
};

#endif
