#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <iostream>
#include <vector>

#include "GL/glew.h"

struct VertexLayout {
	unsigned int location;
	size_t size;
};


class VertexBuffer {
public:
	VertexBuffer(const void* data, size_t size);
	~VertexBuffer();

	void addLayout(unsigned int location, size_t size);
	void setLayout();

	void bind();
	void unBind();

private:
	unsigned int ID;

	unsigned int stride;
	std::vector<VertexLayout> layouts;
};

#endif // !VERTEX_BUFFER_H
