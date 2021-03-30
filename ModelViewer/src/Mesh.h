#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"

#include "glm/glm.hpp"

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	std::string type;
};

class Mesh {
public:
	// mesh data
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader& shader);
private:
	//  render data
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};

#endif // !MESH_H