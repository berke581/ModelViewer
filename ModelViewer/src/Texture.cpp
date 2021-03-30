#include "Texture.h"

#include <iostream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Texture::Texture(GLenum type, const char* path): type(type) {
	// creating textures
	glGenTextures(1, &ID);
	glBindTexture(type, ID);

	// flip image before loading
	stbi_set_flip_vertically_on_load(true);

	if (type == GL_TEXTURE_2D) {
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// load the image
		int width, height, nrChannels;
		unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

		if (data) {
			glTexImage2D(type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(type);
		}
		else {
			std::cout << "Failed to load texture." << std::endl;
		}
		stbi_image_free(data);
	}
	else if (type == GL_TEXTURE_CUBE_MAP) {
		// Load all faces from folder
		std::vector<std::string> faces {
			R"(src\res\skyboxes\right.jpg)",
			R"(src\res\skyboxes\left.jpg)",
			R"(src\res\skyboxes\top.jpg)",
			R"(src\res\skyboxes\bottom.jpg)",
			R"(src\res\skyboxes\front.jpg)",
			R"(src\res\skyboxes\back.jpg)"
		};

		for (unsigned int i = 0; i < faces.size(); i++)
		{
			int width, height, nrChannels;
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	glBindTexture(type, 0);
}

Texture::~Texture() {
	glDeleteTextures(1, &ID);
}

void Texture::bind() {
	glBindTexture(type, ID);
}

void Texture::unBind() {
	glBindTexture(type, 0);
}
