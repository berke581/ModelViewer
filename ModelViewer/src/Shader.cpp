#include "Shader.h"

Shader::Shader(const char* shaderPath)
{
	std::pair<std::string, std::string> shaders = parseShader(shaderPath);

	const char* vertexCode = shaders.first.c_str();
	const char* fragmentCode = shaders.second.c_str();

	compileShader(vertexCode, fragmentCode);
}

Shader::~Shader() { glDeleteProgram(ID); }

void Shader::bind()
{
	glUseProgram(ID);
}

void Shader::unBind()
{
	glUseProgram(0);
}

void Shader::setUniform(const std::string& name, bool value) const
{
	int loc = glGetUniformLocation(ID, name.c_str());

	if (loc != -1)
		glUniform1i(loc, (int)value);
	else
		std::cout << "ERROR::SHADER::UNIFORM::LOCATION_NOT_FOUND\n" << std::endl;
}
void Shader::setUniform(const std::string& name, int value) const
{
	int loc = glGetUniformLocation(ID, name.c_str());

	if (loc != -1)
		glUniform1i(loc, value);
	else
		std::cout << "ERROR::SHADER::UNIFORM::LOCATION_NOT_FOUND\n" << std::endl;
}
void Shader::setUniform(const std::string& name, unsigned int value) const
{
	int loc = glGetUniformLocation(ID, name.c_str());

	if (loc != -1)
		glUniform1i(loc, value);
	else
		std::cout << "ERROR::SHADER::UNIFORM::LOCATION_NOT_FOUND\n" << std::endl;
}
void Shader::setUniform(const std::string& name, float value) const
{
	int loc = glGetUniformLocation(ID, name.c_str());

	if (loc != -1)
		glUniform1f(loc, value);
	else
		std::cout << "ERROR::SHADER::UNIFORM::LOCATION_NOT_FOUND\n" << std::endl;
}
void Shader::setUniform(const std::string& name, float v0, float v1, float v2, float v3) const
{
	int loc = glGetUniformLocation(ID, name.c_str());

	if (loc != -1)
		glUniform4f(loc, v0, v1, v2, v3);
	else
		std::cout << "ERROR::SHADER::UNIFORM::LOCATION_NOT_FOUND\n" << std::endl;
}
void Shader::setUniform(const std::string& name, const glm::mat4& trans) {
	int loc = glGetUniformLocation(ID, name.c_str());

	if (loc != -1)
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(trans));
	else
		std::cout << "ERROR::SHADER::UNIFORM::LOCATION_NOT_FOUND\n" << std::endl;
}

std::pair<std::string, std::string> Shader::parseShader(const char* shaderPath)
{
	std::ifstream file(shaderPath);
	if (file.fail())
		std::cout << "ERROR::SHADER::File stream failed to open!" << std::endl;

	enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
	ShaderType type = ShaderType::NONE;

	std::ifstream shaderFile;
	std::stringstream ss[2];

	std::string line;
	while (std::getline(file, line))
	{
		if (line.find("#SHADER") != std::string::npos)
		{
			if (line.find("VERTEX") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("FRAGMENT") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			if (type == ShaderType::NONE)
				std::cout << "ERROR::SHADER::Could not read shader!" << std::endl;

			if (type != ShaderType::NONE)
				ss[(int)type] << line << "\n";
		}
	}

	std::string vertexCode = ss[0].str();
	std::string fragmentCode = ss[1].str();

	shaderFile.close();
	return std::make_pair(vertexCode, fragmentCode);
}

void Shader::compileShader(const char* vertSource, const char* fragSource) {
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
