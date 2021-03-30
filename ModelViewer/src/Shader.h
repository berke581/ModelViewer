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
#include <filesystem>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath) {
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		// ensure ifstream objects can throw exceptions
		vShaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);

		try {
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch(std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		compileShader(vShaderCode, fShaderCode);
	}
	~Shader() { glDeleteProgram(ID); }

	void bind() { glUseProgram(ID); }
	void unBind() { glUseProgram(0); }

	void set(const std::string& name, bool value) const
	{
		int loc = glGetUniformLocation(ID, name.c_str());

		if (loc != -1)
			glUniform1i(loc, (int)value);
		else
			std::cout << "ERROR::SHADER::UNIFORM::LOCATION_NOT_FOUND\n" << std::endl;
	}
	void set(const std::string& name, int value) const
	{
		int loc = glGetUniformLocation(ID, name.c_str());

		if (loc != -1)
			glUniform1i(loc, value);
		else
			std::cout << "ERROR::SHADER::UNIFORM::LOCATION_NOT_FOUND\n" << std::endl;
	}
	void set(const std::string& name, unsigned int value) const
	{
		int loc = glGetUniformLocation(ID, name.c_str());

		if (loc != -1)
			glUniform1i(loc, value);
		else
			std::cout << "ERROR::SHADER::UNIFORM::LOCATION_NOT_FOUND\n" << std::endl;
	}
	void set(const std::string& name, float value) const
	{
		int loc = glGetUniformLocation(ID, name.c_str());

		if (loc != -1)
			glUniform1f(loc, value);
		else
			std::cout << "ERROR::SHADER::UNIFORM::LOCATION_NOT_FOUND\n" << std::endl;
	}
	void set(const std::string& name, float v0, float v1, float v2, float v3) const
	{
		int loc = glGetUniformLocation(ID, name.c_str());

		if (loc != -1)
			glUniform4f(loc, v0, v1, v2, v3);
		else
			std::cout << "ERROR::SHADER::UNIFORM::LOCATION_NOT_FOUND\n" << std::endl;
	}
	void set(const std::string& name, const glm::mat4& trans) {
		int loc = glGetUniformLocation(ID, name.c_str());

		if (loc != -1)
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(trans));
		else
			std::cout << "ERROR::SHADER::UNIFORM::LOCATION_NOT_FOUND\n" << std::endl;
	}

private:
	unsigned int ID;

	void compileShader(const char* vertSource, const char* fragSource) {
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertSource, NULL);
		glCompileShader(vertex);
		// print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragSource, NULL);
		glCompileShader(fragment);
		// print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		// print linking errors if any
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
};

#endif // !SHADER_H
