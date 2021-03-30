#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void bind();
	void unBind();

private:
	unsigned int ID;
};

#endif // !VERTEX_ARRAY_H
