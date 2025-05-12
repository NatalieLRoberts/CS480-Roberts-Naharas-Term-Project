#pragma once
#include "graphics_headers.h"
#include <SOIL2/SOIL2.h>

class Texture
{

public:
	Texture();
	Texture(const char* fileName);
	bool loadTexture(const char* texFile);
	GLuint getTextureID() { return m_TextureID; }
	// Texture unit management
	void bind(GLenum textureUnit = GL_TEXTURE0);
	// Configure texture parameters
	void setTextureWrapMode(GLint wrapS = GL_REPEAT, GLint wrapT = GL_REPEAT);
	void setTextureFilters(GLint minFilter = GL_LINEAR_MIPMAP_LINEAR, GLint magFilter = GL_LINEAR);


private:
	GLuint m_TextureID;

	bool initializeTexture();

};

