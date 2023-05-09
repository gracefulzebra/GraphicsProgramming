#pragma once

#include <string>
#include <GL\glew.h>

class TextureHandler
{
public:
	TextureHandler();
	~TextureHandler();

	void InitializeTexture(const std::string& fileName);
	void BindTexture(unsigned int unit); // bind upto 32 textures
	GLuint getID() { return textureHandler; }

protected:
private:
	GLuint textureHandler = NULL;
};

