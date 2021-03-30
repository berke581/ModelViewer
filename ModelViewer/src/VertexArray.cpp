#include "VertexArray.h"

#include "GL/glew.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &ID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &ID);
}

void VertexArray::bind()
{
	glBindVertexArray(ID);
}

void VertexArray::unBind()
{
	glBindVertexArray(0);
}
