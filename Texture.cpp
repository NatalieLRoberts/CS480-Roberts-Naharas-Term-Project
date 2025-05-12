#include "Texture.h"

Texture::Texture(const char* fileName) {
    loadTexture(fileName);
    initializeTexture();
}

Texture::Texture() {
    m_TextureID = -1;
    printf("No Texture Data Provided.");
}

bool Texture::loadTexture(const char* texFile) {
    // Load texture using SOIL2 with automatic format detection and Y-inversion
    m_TextureID = SOIL_load_OGL_texture(texFile, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (!m_TextureID) {
        printf("Failed: Could not open texture file!\n");
        return false;
    }
    return true;
}

bool Texture::initializeTexture() {
    // Bind the texture before setting parameters
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    // Generate mipmaps for this texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set default texture filtering parameters
    setTextureFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    // Set default texture wrapping mode
    setTextureWrapMode(GL_REPEAT, GL_REPEAT);

    return true;
}

void Texture::bind(GLenum textureUnit) {
    // Activate specified texture unit
    glActiveTexture(textureUnit);

    // Bind this texture to the currently active texture unit
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::setTextureWrapMode(GLint wrapS, GLint wrapT) {
    // Bind the texture before changing parameters
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    // Set wrap parameters for S and T coordinates
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
}

void Texture::setTextureFilters(GLint minFilter, GLint magFilter) {
    // Bind the texture before changing parameters
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    // Set filtering for minification (when texture appears smaller than its size)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

    // Set filtering for magnification (when texture appears larger than its size)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}
