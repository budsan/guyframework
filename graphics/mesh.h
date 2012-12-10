#pragma once

#include "graphics.h"
#include "vertexattribute.h"

#include "math/bbox.h"

namespace Guy
{

class Mesh
{
public:
	enum PrimitiveType
	{
		TRIANGLES = GL_TRIANGLES,
		TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		LINES = GL_LINES,
		LINE_STRIP = GL_LINE_STRIP,
		POINTS = GL_POINTS
	};

	Mesh(const Vertex::Format& vertexFormat, unsigned int vertexCount, bool dynamic = false);

	const Vertex::Format& vertexFormat() const;
	unsigned int vertexCount() const;
	unsigned int vertexSize() const;
	bool isDynamic() const;
	GLuint vertexBuffer() const;

	PrimitiveType primitiveType() const;
	void setPrimitiveType(Mesh::PrimitiveType type);
	void setVertexData(float* vertexData, unsigned int vertexStart, unsigned int vertexCount);

protected:

	const Vertex::Format m_vertexFormat;
	unsigned int m_vertexCount;
	GLuint m_vertexBuffer;
	PrimitiveType m_primitiveType;
	bool m_dynamic;
};

} //namespace Guy
