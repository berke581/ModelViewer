#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL/glew.h"

class Texture {
public:
	Texture(GLenum type, const char* path);
	~Texture();

	void bind();
	void unBind();

private:
	unsigned int ID;
	GLenum type;
};

#endif