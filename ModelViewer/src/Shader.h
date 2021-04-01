#ifndef SHADER_H
#define SHADER_H

#include "GL/glew.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>

class Shader {
public:
	Shader(const char* shaderPath);
	~Shader();

	void bind();
	void unBind();

	void setUniform(const std::string& name, bool value) const;
	void setUniform(const std::string& name, int value) const;
	void setUniform(const std::string& name, unsigned int value) const;
	void setUniform(const std::string& name, float value) const;
	void setUniform(const std::string& name, float v0, float v1, float v2, float v3) const;
	void setUniform(const std::string& name, const glm::mat4& trans);

private:
	unsigned int ID;

	std::pair<std::string, std::string> parseShader(const char* shaderPath);
	void compileShader(const char* vertSource, const char* fragSource);
};

#endif // !SHADER_H
