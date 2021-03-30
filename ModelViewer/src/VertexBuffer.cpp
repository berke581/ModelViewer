#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, size_t size)
	:ID(0), stride(0) {

	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &ID);
}

void VertexBuffer::addLayout(unsigned int location, size_t size) {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	layouts.push_back(VertexLayout{location, size});
	stride += size * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setLayout() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);

	unsigned int start = 0;
	for (const auto& el : layouts) {
		glVertexAttribPointer(el.location, el.size, GL_FLOAT, GL_FALSE, stride, (void*)start);
		glEnableVertexAttribArray(el.location);
		start += el.size * sizeof(float);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBuffer::unBind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
