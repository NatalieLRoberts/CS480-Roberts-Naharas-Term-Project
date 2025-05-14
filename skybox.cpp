#include "Skybox.h"
#include <SOIL2/SOIL2.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

// Vertex shader source
const char* vertShaderSrc = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 TexCoords;
uniform mat4 view;
uniform mat4 projection;
void main() {
    TexCoords = aPos;
    mat4 viewNoTrans = mat4(mat3(view));
    gl_Position = projection * viewNoTrans * vec4(aPos, 1.0);
}
)";

// Fragment shader source
const char* fragShaderSrc = R"(
#version 330 core
in vec3 TexCoords;
out vec4 FragColor;
uniform samplerCube skybox;
void main() {
    FragColor = texture(skybox, TexCoords);
}
)";

Skybox::Skybox(const std::vector<std::string>& faces) {
    LoadCubemap(faces);
    SetupSkyboxGeometry();
    m_shaderProgram = CreateShaderProgram();
}

Skybox::~Skybox() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteTextures(1, &m_cubemapTex);
    glDeleteProgram(m_shaderProgram);
}

void Skybox::LoadCubemap(const std::vector<std::string>& faces) {
    glGenTextures(1, &m_cubemapTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTex);

    int width, height, channels;
    for (GLuint i = 0; i < faces.size(); i++) {
        unsigned char* data = SOIL_load_image(faces[i].c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
            );
            SOIL_free_image_data(data);
            printf("Loaded skybox face %d: %s (%dx%d)\n", i, faces[i].c_str(), width, height);
        }
        else {
            printf("Failed to load skybox face %d: %s\n", i, faces[i].c_str());
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::SetupSkyboxGeometry() {
    float skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f, 1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

GLuint Skybox::CreateShaderProgram() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertexShader);
    CheckShaderError(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragmentShader);
    CheckShaderError(fragmentShader, "FRAGMENT");

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    CheckShaderError(program, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void Skybox::CheckShaderError(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            printf("SHADER_COMPILATION_ERROR (%s): %s\n", type.c_str(), infoLog);
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            printf("PROGRAM_LINKING_ERROR: %s\n", infoLog);
        }
    }
}

void Skybox::Render(const glm::mat4& view, const glm::mat4& projection) {
    glDepthFunc(GL_LEQUAL);
    glUseProgram(m_shaderProgram);

    GLuint skyboxLoc = glGetUniformLocation(m_shaderProgram, "skybox");
    glUniform1i(skyboxLoc, 0); // Always set this

    GLuint viewLoc = glGetUniformLocation(m_shaderProgram, "view");
    GLuint projLoc = glGetUniformLocation(m_shaderProgram, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTex);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glUseProgram(0);
    glDepthFunc(GL_LESS);
}
